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
#include <cstring>

namespace athena
{

namespace io
{

WiiSaveReader::WiiSaveReader(const atUint8* data, atUint64 length)
    : MemoryCopyReader(data, length)
{
    setEndian(Endian::Big);
}

WiiSaveReader::WiiSaveReader(const std::string& filename)
    : MemoryCopyReader(filename)
{
    setEndian(Endian::Big);
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
    atUint32 bkVer = readUint32();

    if (bkVer != 0x00000070)
    {
        atError("Invalid BacKup header size");
        return nullptr;
    }

    atUint32 bkMagic = readUint32();

    if (bkMagic != 0x426B0001)
    {
        atError("Invalid BacKup header magic");
        return nullptr;
    }

    /*atUint32 ngId =*/ readUint32();
    atUint32 numFiles = readUint32();

    /*int fileSize =*/ readUint32();
    seek(8); // skip unknown data;

    atUint32 totalSize = readUint32();
    seek(64); // Unknown (Most likely padding)
    seek(8);
    seek(6);
    seek(2);
    seek(0x10);

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
    std::unique_ptr<atUint8[]> buf = readUBytes(0xF0C0);
    atUint8* oldData = data();
    atUint64 oldPos = position();
    atUint64 oldLen = length();
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
    aes->decrypt(tmpIV, buf.get(), dec, 0xF0C0);
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
        setData(oldData, oldLen);
        seek(oldPos, SeekOrigin::Begin);
        atError("MD5 Mismatch");
        return nullptr;
    }

    // Set the binary reader buffer;
    setData(dec, 0xF0C0);
    // Start reading the header
    gameId = readUint64();
    bannerSize = readUint32();
    permissions = readByte();
    /*    unk =*/ readByte();
    seek(0x10);
    // skip padding
    seek(2);

    int magic;
    int flags;
    short animSpeed;
    std::u16string gameTitle;
    std::u16string subTitle;

    magic = readUint32();

    // Ensure that the header magic is valid.
    if (magic != 0x5749424E)
    {
        // Make sure to reset m_reader values back to the old ones.
        setData(oldData, oldLen);
        seek(oldPos, SeekOrigin::Begin);
        atError("Invalid Header Magic");
        return nullptr;
    }

    flags = readUint32();
    animSpeed = readUint16();
    seek(22);

    gameTitle = readU16StringBig();

    if (position() != 0x0080)
        seek(0x0080, SeekOrigin::Begin);

    subTitle = readU16StringBig();

    if (position() != 0x00C0)
        seek(0x00C0, SeekOrigin::Begin);

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

    setData(oldData, oldLen);
    seek(oldPos, SeekOrigin::Begin);
    return banner;
}

WiiImage* WiiSaveReader::readImage(atUint32 width, atUint32 height)
{
    std::unique_ptr<atUint8[]> image = readUBytes(width * height * 2);

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

    atUint32 magic = readUint32();

    if (magic != 0x03adf17e)
    {
        std::cerr << "Not a valid File entry header: 0x" << std::hex << magic << std::endl;
        return NULL;
    }

    fileLen     = readUint32();
    permissions = readByte();
    attributes  = readByte();
    type        = (WiiFile::Type)readByte();
    name        = std::string((const char*)readBytes(0x45).get());
    ret         = new WiiFile(std::string(name));
    ret->setPermissions(permissions);
    ret->setAttributes(attributes);
    ret->setType((WiiFile::Type)type);
    std::unique_ptr<atUint8[]> iv = readUBytes(0x10);
    seek(0x20);

    if (type == WiiFile::File)
    {
        // Read file data
        int roundedLen = (fileLen + 63) & ~63;
        std::unique_ptr<atUint8[]> filedata = readUBytes(roundedLen);

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
    std::unique_ptr<atUint8[]> sig    = readUBytes(0x40);
    std::unique_ptr<atUint8[]> ngCert = readUBytes(0x180);
    std::unique_ptr<atUint8[]> apCert = readUBytes(0x180);
    seek(0xF0C0, SeekOrigin::Begin);
    std::unique_ptr<atUint8[]> data   = readUBytes(dataSize);
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
