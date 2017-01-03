#include "athena/WiiSaveReader.hpp"
#include "athena/WiiSave.hpp"
#include "athena/WiiFile.hpp"
#include "athena/WiiImage.hpp"
#include "athena/WiiBanner.hpp"
#include "athena/Utility.hpp"
#include "athena/FileWriter.hpp"
#include "md5.h"
#include "aes.hpp"
#include "ec.hpp"
#include "sha1.h"
#include <iostream>
#include <iomanip>
#include <string.h>

namespace athena
{

namespace io
{

WiiSaveReader::WiiSaveReader(const atUint8* data, atUint64 length)
    : base(data, length)
{
    setEndian(Endian::BigEndian);
}

WiiSaveReader::WiiSaveReader(const std::string& filename)
    : base(filename)
{
    setEndian(Endian::BigEndian);
}

std::unique_ptr<WiiSave> WiiSaveReader::readSave()
{
    WiiSave* ret = new WiiSave;

    if (length() < 0xF0C0)
    {
        atError("Not a valid WiiSave");
        return nullptr;
    }

    WiiBanner* banner = this->readBanner();

    if (!banner)
    {
        atError("Invalid banner");
        return nullptr;
    }

    ret->setBanner(banner);
    atUint32 bkVer = base::readUint32();

    if (bkVer != 0x00000070)
    {
        atError("Invalid BacKup header size");
        return nullptr;
    }

    atUint32 bkMagic = base::readUint32();

    if (bkMagic != 0x426B0001)
    {
        atError("Invalid BacKup header magic");
        return nullptr;
    }

    /*atUint32 ngId =*/ base::readUint32();
    atUint32 numFiles = base::readUint32();

    /*int fileSize =*/ base::readUint32();
    base::seek(8); // skip unknown data;

    atUint32 totalSize = base::readUint32();
    base::seek(64); // Unknown (Most likely padding)
    base::seek(8);
    base::seek(6);
    base::seek(2);
    base::seek(0x10);

    std::vector<WiiFile*> files;

    for (atUint32 i = 0; i < numFiles; ++i)
    {
        WiiFile* file = readFile();

        if (file)
            files.push_back(file);
    }

    ret->setRoot(buildTree(files));

    readCerts(totalSize);
    return std::unique_ptr<WiiSave>(ret);
}

WiiBanner* WiiSaveReader::readBanner()
{
    atUint8* dec = new atUint8[0xF0C0];
    memset(dec, 0, 0xF0C0);
    std::unique_ptr<atUint8[]> data = base::readUBytes(0xF0C0);
    atUint8* oldData = base::data();
    atUint64 oldPos = base::position();
    atUint64 oldLen = base::length();
    atUint64 gameId;
    atUint32 bannerSize;
    atUint8  permissions;
    atUint8  md5[16];
    atUint8  md5Calc[16];
    atUint8  tmpIV[16];
    memcpy(tmpIV, SD_IV, 16);

    std::cout << "Decrypting: banner.bin...";
    std::unique_ptr<IAES> aes = NewAES();
    aes->setKey(SD_KEY);
    aes->decrypt(tmpIV, data.get(), dec, 0xF0C0);
    std::cout << "done" << std::endl;

    memset(md5, 0, 16);
    memset(md5Calc, 0, 16);
    // Read in the MD5 sum
    memcpy(md5, (dec + 0x0E), 0x10);
    // Write the blanker to the buffer
    memcpy((dec + 0x0E), MD5_BLANKER, 0x10);
    MD5Hash::MD5(md5Calc, dec, 0xF0C0);

    // Compare the Calculated MD5 to the one from the file.
    // This needs to be done incase the file is corrupted.
    if (memcmp(md5, md5Calc, 0x10))
    {
        std::cerr << "MD5 Mismatch" << std::endl;
        // Make sure to reset m_reader values back to the old ones.
        std::cerr << "MD5 provided:   ";

        for (int i = 0; i < 16; ++i)
            std::cerr << std::setw(2) << std::setfill('0') << std::hex << (int)(md5[i]);

        std::cerr << std::endl;

        std::cerr << "MD5 Calculated: ";

        for (int i = 0; i < 16; ++i)
            std::cerr << std::hex << (int)(md5Calc[i]);

        std::cerr << std::endl;
        base::setData(oldData, oldLen);
        base::seek(oldPos, SeekOrigin::Begin);
        atError("MD5 Mismatch");
        return nullptr;
    }

    // Set the binary reader buffer;
    base::setData(dec, 0xF0C0);
    // Start reading the header
    gameId = base::readUint64();
    bannerSize = base::readUint32();
    permissions = base::readByte();
    /*    unk =*/ base::readByte();
    base::seek(0x10);
    // skip padding
    base::seek(2);

    int magic;
    int flags;
    short animSpeed;
    std::string gameTitle;
    std::string subTitle;

    magic = base::readUint32();

    // Ensure that the header magic is valid.
    if (magic != 0x5749424E)
    {
        // Make sure to reset m_reader values back to the old ones.
        base::setData(oldData, oldLen);
        base::seek(oldPos, SeekOrigin::Begin);
        atError("Invalid Header Magic");
        return nullptr;
    }

    flags = base::readUint32();
    animSpeed = base::readUint16();
    base::seek(22);

    gameTitle = base::readWStringAsString();

    if (base::position() != 0x0080)
        base::seek(0x0080, SeekOrigin::Begin);

    subTitle = base::readWStringAsString();

    if (base::position() != 0x00C0)
        base::seek(0x00C0, SeekOrigin::Begin);

    WiiBanner* banner = new WiiBanner;
    banner->setGameID(gameId);
    banner->setTitle(gameTitle);
    banner->setSubtitle(subTitle);
    banner->setBannerSize(bannerSize);
    WiiImage* bannerImage = readImage(192, 64);
    banner->setBannerImage(bannerImage);
    banner->setAnimationSpeed(animSpeed);
    banner->setPermissions(permissions);
    banner->setFlags(flags);


    if (banner->bannerSize() == 0x72a0)
    {
        WiiImage* icon = readImage(48, 48);

        if (icon)
            banner->addIcon(icon);
        else
            std::cerr << "Warning: Icon empty, skipping" << std::endl;
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            WiiImage* icon = readImage(48, 48);

            if (icon)
                banner->addIcon(icon);
            else
                std::cerr << "Warning: Icon empty, skipping" << std::endl;
        }
    }

    base::setData(oldData, oldLen);
    base::seek(oldPos, SeekOrigin::Begin);
    return banner;
}

WiiImage* WiiSaveReader::readImage(atUint32 width, atUint32 height)
{
    std::unique_ptr<atUint8[]> image = base::readUBytes(width * height * 2);

    if (!utility::isEmpty((atInt8*)image.get(), width * height * 2))
        return new WiiImage(width, height, std::move(image));

    return NULL;
}


WiiFile* WiiSaveReader::readFile()
{
    atUint32 fileLen;
    atUint8 permissions;
    atUint8 attributes;
    atUint8 type;
    std::string name;
    WiiFile* ret;

    atUint32 magic = base::readUint32();

    if (magic != 0x03adf17e)
    {
        std::cerr << "Not a valid File entry header: 0x" << std::hex << magic << std::endl;
        return NULL;
    }

    fileLen     = base::readUint32();
    permissions = base::readByte();
    attributes  = base::readByte();
    type        = (WiiFile::Type)base::readByte();
    name        = std::string((const char*)base::readBytes(0x45).get());
    ret         = new WiiFile(std::string(name));
    ret->setPermissions(permissions);
    ret->setAttributes(attributes);
    ret->setType((WiiFile::Type)type);
    std::unique_ptr<atUint8[]> iv = base::readUBytes(0x10);
    base::seek(0x20);

    if (type == WiiFile::File)
    {
        // Read file data
        int roundedLen = (fileLen + 63) & ~63;
        std::unique_ptr<atUint8[]> filedata = base::readUBytes(roundedLen);

        // Decrypt file
        std::cout << "Decrypting: " << ret->filename() << "...";
        atUint8* decData = new atUint8[roundedLen];
        std::unique_ptr<IAES> aes = NewAES();
        aes->setKey(SD_KEY);
        aes->decrypt(iv.get(), filedata.get(), decData, roundedLen);
        ret->setData(decData);
        ret->setLength(fileLen);
        std::cout << "done" << std::endl;
    }

    return ret;
}

void WiiSaveReader::readCerts(atUint32 totalSize)
{
    std::cout << "Reading certs..." << std::endl;
    atUint32 dataSize = totalSize - 0x340;
    std::unique_ptr<atUint8[]> sig    = base::readUBytes(0x40);
    std::unique_ptr<atUint8[]> ngCert = base::readUBytes(0x180);
    std::unique_ptr<atUint8[]> apCert = base::readUBytes(0x180);
    base::seek(0xF0C0, SeekOrigin::Begin);
    std::unique_ptr<atUint8[]> data   = base::readUBytes(dataSize);
    atUint8* hash;

    std::cout << "validating..." << std::endl;
    hash = getSha1(data.get(), dataSize);
    atUint8* hash2 = getSha1(hash, 20);
    bool ngValid = false;
    bool apValid = false;
    ecc::checkEC(ngCert.get(), apCert.get(), sig.get(), hash2, apValid, ngValid);

    if (apValid)
        std::cout << "AP Certificate ok" << std::endl;
    if (ngValid)
        std::cout << "NG Certificate ok" << std::endl;
}

WiiFile* WiiSaveReader::buildTree(std::vector<WiiFile*> files)
{
    // This is simply a virtual root that will contain all the other nodes
    WiiFile* root = new WiiFile("");
    root->setType(WiiFile::Directory);

    for (WiiFile* f : files)
        root->addChild(f);

    return root;
}

} // io
} // zelda
