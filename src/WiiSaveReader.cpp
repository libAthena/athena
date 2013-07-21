// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#include "WiiSaveReader.hpp"
#include "WiiSave.hpp"
#include "WiiFile.hpp"
#include "WiiImage.hpp"
#include "WiiBanner.hpp"
#include "md5.h"
#include "aes.h"
#include "ec.h"
#include "sha1.h"
#include <iostream>
#include <iomanip>
#include <utility.hpp>
#include <IOException.hpp>
#include <InvalidOperationException.hpp>
#include <string.h>

namespace zelda
{

namespace io
{

const Uint8 SD_KEY[16]      = {0xab, 0x01, 0xb9, 0xd8, 0xe1, 0x62, 0x2b, 0x08, 0xaf, 0xba, 0xd8, 0x4d, 0xbf, 0xc2, 0xa5, 0x5d};
const Uint8 SD_IV[16]       = {0x21, 0x67, 0x12, 0xe6, 0xaa, 0x1f, 0x68, 0x9f, 0x95, 0xc5, 0xa2, 0x23, 0x24, 0xdc, 0x6a, 0x98};
const Uint8 MD5_BLANKER[16] = {0x0e, 0x65, 0x37, 0x81, 0x99, 0xbe, 0x45, 0x17, 0xab, 0x06, 0xec, 0x22, 0x45, 0x1a, 0x57, 0x93};

WiiSaveReader::WiiSaveReader(const Uint8* data, Uint64 length)
    : base(data, length)
{
    setEndianess(BigEndian);
}

WiiSaveReader::WiiSaveReader(const std::string& filename)
    : base(filename)
{
    setEndianess(BigEndian);
}

WiiSave* WiiSaveReader::readSave()
{
    WiiSave* ret = new WiiSave;
    if (length() < 0xF0C0)
        throw error::InvalidOperationException("WiiSaveReader::readSave -> Not a valid WiiSave");

    WiiBanner* banner = this->readBanner();
    if (!banner)
        throw error::InvalidOperationException("WiiSaveReader::readSave -> Invalid banner");

    ret->setBanner(banner);
    Uint32 bkVer = base::readUInt32();

    if (bkVer != 0x00000070)
        throw error::InvalidOperationException("WiiSaveReader::readSave -> Invalid BacKup header size");

    Uint32 bkMagic = base::readUInt32();
    bkMagic = bkMagic;
    if (bkMagic != 0x426B0001)
        throw error::InvalidOperationException("WiiSaveReader::readSave -> Invalid BacKup header magic");

    Uint32 ngId = base::readUInt32();
    ngId = ngId;

    Uint32 numFiles = base::readUInt32();

    /*int fileSize =*/ base::readUInt32();
    base::seek(8); // skip unknown data;

    Uint32 totalSize = base::readUInt32();
    base::seek(64); // Unknown (Most likely padding)
    base::seek(8);
    base::seek(6);
    base::seek(2);
    base::seek(0x10);

    WiiFile* file;
    for (Uint32 i = 0; i < numFiles; ++i)
    {
        file = readFile();
        if (file)
            ret->addFile("/" + file->filename(), file);
    }

    readCerts(totalSize);
    return ret;
}

WiiBanner* WiiSaveReader::readBanner()
{
    Uint8* dec = new Uint8[0xf0c0];
    memset(dec, 0, 0xF0C0);
    Uint8* data = (Uint8*)base::readBytes(0xF0C0);
    Uint8* oldData = base::data();
    Uint64 oldPos = base::position();
    Uint64 oldLen = base::length();
    Uint64 gameId;
    Uint32 bannerSize;
    Uint8  permissions;
    Uint8  md5[16];
    Uint8  md5Calc[16];
    Uint8  tmpIV[26];
    memcpy(tmpIV, SD_IV, 16);

    aes_set_key(SD_KEY);
    aes_decrypt(tmpIV, data, dec, 0xF0C0);
    memset(md5, 0, 16);
    memset(md5Calc, 0, 16);
    // Read in the MD5 sum
    memcpy(md5, (dec + 0x0E), 0x10);
    // Write the blanker to the buffer
    memcpy((dec + 0x0E), MD5_BLANKER, 0x10);
    MD5(md5Calc, dec, 0xF0C0);

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
        base::seek(oldPos, Stream::Beginning);
        throw error::InvalidOperationException("WiiSaveReader::readBanner -> MD5 Mismatch");
    }
    // Set the binary reader buffer;
    base::setData(dec, 0xF0C0);
    // Start reading the header
    gameId = base::readUInt64();
    bannerSize = base::readUInt32();
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

    magic = base::readUInt32();

    // Ensure that the header magic is valid.
    if (magic != 0x5749424E)
    {
        // Make sure to reset m_reader values back to the old ones.
        base::setData(oldData, oldLen);
        base::seek(oldPos, Stream::Beginning);
        throw error::InvalidOperationException("WiiSaveReader::readBanner -> Invalid Header Magic");
    }

    flags = base::readUInt32();
    animSpeed = base::readUInt16();
    base::seek(22);

    gameTitle = base::readUnicode();
    if (base::position() != 0x0080)
        base::seek(0x0080, Stream::Beginning);

    subTitle = base::readUnicode();
    if (base::position() != 0x00C0)
        base::seek(0x00C0, Stream::Beginning);

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
    base::seek(oldPos, Stream::Beginning);
    return banner;
}

WiiImage* WiiSaveReader::readImage(Uint32 width, Uint32 height)
{
    Uint8* image = (Uint8*)base::readBytes(width*height*2);

    if (!isEmpty((Int8*)image, width*height*2))
        return new WiiImage(width, height, image);

    return NULL;
}


WiiFile* WiiSaveReader::readFile()
{
    Uint32 fileLen;
    Uint8 permissions;
    Uint8 attributes;
    Uint8 type;
    std::string name;
    Uint8* filedata;
    WiiFile* ret;

    Uint32 magic = base::readUInt32();
    if (magic != 0x03adf17e)
    {
        std::cerr << "Not a valid File entry header: 0x" << std::hex << magic << std::endl;
        return NULL;
    }

    fileLen     = base::readUInt32();
    permissions = base::readByte();
    attributes  = base::readByte();
    type        = (WiiFile::Type)base::readByte();
    name        = std::string((const char*)base::readBytes(0x45));
    ret         = new WiiFile(std::string(name));
    ret->setPermissions(permissions);
    ret->setAttributes(attributes);
    ret->setType((WiiFile::Type)type);
    Uint8* iv = (Uint8*)base::readBytes(0x10);
    base::seek(0x20);

    if (type == WiiFile::File)
    {
        // Read file data
        int roundedLen = (fileLen + 63) & ~63;
        filedata = (Uint8*)base::readBytes(roundedLen);

        // Decrypt file
        Uint8* decData = new Uint8[roundedLen];
        aes_set_key(SD_KEY);
        aes_decrypt(iv, filedata, decData, roundedLen);
        delete filedata;
        ret->setData(decData);
        ret->setLength(fileLen);
    }

    return ret;
}


void WiiSaveReader::readCerts(Uint32 totalSize)
{
    Uint32 dataSize = totalSize - 0x340;
    Uint8* sig    = (Uint8*)base::readBytes(0x40);
    Uint8* ngCert = (Uint8*)base::readBytes(0x180);
    Uint8* apCert = (Uint8*)base::readBytes(0x180);
    base::seek(0xF0C0, Stream::Beginning);
    Uint8* data   = (Uint8*)base::readBytes(dataSize);
    Uint8* hash;

    hash = getSha1(data, dataSize);
    Uint8* hash2 = getSha1(hash, 20);

    check_ec(ngCert, apCert, sig, hash2);
}

} // io
} // zelda
