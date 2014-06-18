#ifndef ATHENA_NO_SAVES
// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#include "Athena/WiiSaveReader.hpp"
#include "Athena/WiiSave.hpp"
#include "Athena/WiiFile.hpp"
#include "Athena/WiiImage.hpp"
#include "Athena/WiiBanner.hpp"
#include "Athena/Utility.hpp"
#include "Athena/IOException.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/InvalidDataException.hpp"
#include "md5.h"
#include "aes.h"
#include "ec.h"
#include "sha1.h"
#include <iostream>
#include <iomanip>
#include <string.h>

namespace Athena
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

WiiSave* WiiSaveReader::readSave()
{
    WiiSave* ret = new WiiSave;
    try
    {
        if (length() < 0xF0C0)
            THROW_INVALID_DATA_EXCEPTION("Not a valid WiiSave");

        WiiBanner* banner = this->readBanner();
        if (!banner)
            THROW_INVALID_DATA_EXCEPTION("Invalid banner");

        ret->setBanner(banner);
        atUint32 bkVer = base::readUint32();

        if (bkVer != 0x00000070)
            THROW_INVALID_DATA_EXCEPTION("Invalid BacKup header size");

        atUint32 bkMagic = base::readUint32();
        bkMagic = bkMagic;
        if (bkMagic != 0x426B0001)
            THROW_INVALID_DATA_EXCEPTION("Invalid BacKup header magic");

        atUint32 ngId = base::readUint32();
        ngId = ngId;

        atUint32 numFiles = base::readUint32();

        /*int fileSize =*/ base::readUint32();
        base::seek(8); // skip unknown data;

        atUint32 totalSize = base::readUint32();
        base::seek(64); // Unknown (Most likely padding)
        base::seek(8);
        base::seek(6);
        base::seek(2);
        base::seek(0x10);

        std::unordered_map<std::string, WiiFile*> files;
        for (atUint32 i = 0; i < numFiles; ++i)
        {
            WiiFile* file = readFile();
            if (file)
                files["/"+file->filename()] = file;
        }

        ret->setFiles(files);

        readCerts(totalSize);
    }
    catch(...)
    {
        delete ret;
        ret = NULL;
        throw;
    }

    return ret;
}

WiiBanner* WiiSaveReader::readBanner()
{
    atUint8* dec = new atUint8[0xF0C0];
    memset(dec, 0, 0xF0C0);
    atUint8* data = (atUint8*)base::readBytes(0xF0C0);
    atUint8* oldData = base::data();
    atUint64 oldPos = base::position();
    atUint64 oldLen = base::length();
    atUint64 gameId;
    atUint32 bannerSize;
    atUint8  permissions;
    atUint8  md5[16];
    atUint8  md5Calc[16];
    atUint8  tmpIV[26];
    memcpy(tmpIV, SD_IV, 16);

    std::cout << "Decrypting: banner.bin...";
    aes_set_key(SD_KEY);
    aes_decrypt(tmpIV, data, dec, 0xF0C0);
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
        THROW_INVALID_DATA_EXCEPTION("MD5 Mismatch");
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
        THROW_INVALID_DATA_EXCEPTION("Invalid Header Magic");
    }

    flags = base::readUint32();
    animSpeed = base::readUint16();
    base::seek(22);

    gameTitle = base::readUnicode();
    if (base::position() != 0x0080)
        base::seek(0x0080, SeekOrigin::Begin);

    subTitle = base::readUnicode();
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
        for(int i = 0; i < 8; i++)
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
    atUint8* image = (atUint8*)base::readBytes(width*height*2);

    if (!utility::isEmpty((atInt8*)image, width*height*2))
        return new WiiImage(width, height, image);

    return NULL;
}


WiiFile* WiiSaveReader::readFile()
{
    atUint32 fileLen;
    atUint8 permissions;
    atUint8 attributes;
    atUint8 type;
    std::string name;
    atUint8* filedata;
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
    name        = std::string((const char*)base::readBytes(0x45));
    ret         = new WiiFile(std::string(name));
    ret->setPermissions(permissions);
    ret->setAttributes(attributes);
    ret->setType((WiiFile::Type)type);
    atUint8* iv = (atUint8*)base::readBytes(0x10);
    base::seek(0x20);

    if (type == WiiFile::File)
    {
        // Read file data
        int roundedLen = (fileLen + 63) & ~63;
        filedata = (atUint8*)base::readBytes(roundedLen);

        // Decrypt file
        std::cout << "Decrypting: " << ret->filename() << "...";
        atUint8* decData = new atUint8[roundedLen];
        aes_set_key(SD_KEY);
        aes_decrypt(iv, filedata, decData, roundedLen);
        delete filedata;
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
    atUint8* sig    = (atUint8*)base::readBytes(0x40);
    atUint8* ngCert = (atUint8*)base::readBytes(0x180);
    atUint8* apCert = (atUint8*)base::readBytes(0x180);
    base::seek(0xF0C0, SeekOrigin::Begin);
    atUint8* data   = (atUint8*)base::readBytes(dataSize);
    atUint8* hash;

    hash = getSha1(data, dataSize);
    atUint8* hash2 = getSha1(hash, 20);

    std::cout << "validating..." << std::endl;
    std::cout << (check_ec(ngCert, apCert, sig, hash2) ? "ok" : "invalid") << "...";
    std::cout << "done" << std::endl;
}

} // io
} // zelda
#endif // ATHENA_NO_SAVES
