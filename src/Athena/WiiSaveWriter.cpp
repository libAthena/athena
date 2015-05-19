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

#include "Athena/WiiSaveWriter.hpp"
#include "Athena/WiiSave.hpp"
#include "Athena/WiiFile.hpp"
#include "Athena/WiiBanner.hpp"
#include "Athena/WiiImage.hpp"
#include "Athena/WiiSave.hpp"
#include "Athena/WiiFile.hpp"
#include "Athena/WiiBanner.hpp"
#include "Athena/MemoryWriter.hpp"
#include "Athena/Utility.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/InvalidDataException.hpp"

#include "aes.h"
#include "ec.h"
#include "md5.h"
#include "sha1.h"

#include <stdio.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>

namespace Athena
{

namespace io
{

WiiSaveWriter::WiiSaveWriter(const std::string& filename)
    : base(filename)
{
    base::setEndian(Endian::BigEndian);
}


bool WiiSaveWriter::writeSave(WiiSave* save, atUint8* macAddress, atUint32 ngId, atUint8* ngPriv, atUint8* ngSig, atUint32 ngKeyId, const std::string& filepath)
{
    if (!save)
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(false, "save cannot be NULL");

    if (filepath != "")
        m_filepath = filepath;

    writeBanner(save->banner());

    base::writeUint32(0x70);
    base::writeUint32(0x426B0001);
    base::writeUint32(ngId); // NG-ID
    base::writeUint32(save->fileCount());
    base::writeUint32(0); // Size of files;
    base::seek(8);
    base::writeUint32(0); // totalSize
    base::seek(64);
    base::writeUint64(save->banner()->gameID());
    base::writeBytes((atInt8*)macAddress, 6);
    base::seek(2); // unknown;
    base::seek(0x10); // padding;
    atUint32 totalSize = 0;

    for (WiiFile* file : save->allFiles())
    {
        totalSize += writeFile(file);
    }

    atUint64 pos = base::position();
    // Write size data
    base::seek(0xF0C0 + 0x10, SeekOrigin::Begin);
    base::writeUint32(totalSize);
    base::seek(0xF0C0 + 0x1C, SeekOrigin::Begin);
    base::writeUint32(totalSize + 0x3c0);
    base::seek(pos, SeekOrigin::Begin);

    writeCerts(totalSize, ngId, ngPriv, ngSig, ngKeyId);

    base::save();

    return true;
}

void WiiSaveWriter::writeBanner(WiiBanner* banner)
{
    base::setEndian(Endian::BigEndian);
    base::writeInt64(banner->gameID());
    base::writeInt32((0x60a0 + 0x1200) * (atUint32)banner->icons().size());
    base::writeByte((atInt8)banner->permissions());
    base::seek(1);
    base::writeBytes((atInt8*)MD5_BLANKER, 16);
    base::seek(2);
    base::writeInt32(0x5749424E); // WIBN
    base::writeInt32(banner->flags());
    base::writeInt16(banner->animationSpeed());
    base::seek(22);

    base::writeUnicode(banner->title());

    if (base::position() != 0x0080)
        base::seek(0x0080, SeekOrigin::Begin);

    base::writeUnicode(banner->subtitle());

    if (base::position() != 0x00C0)
        base::seek(0x00C0, SeekOrigin::Begin);

    WiiImage* bannerImage = banner->bannerImage();
    base::writeBytes((atInt8*)bannerImage->data(), bannerImage->width()*bannerImage->height() * 2);

    // For empty icons
    atUint8* tmpIcon = new atUint8[48 * 48 * 2];
    memset(tmpIcon, 0, 48 * 48 * 2);

    for (atUint32 i = 0; i < 8; ++i)
    {
        if (i < banner->icons().size())
        {
            writeImage(banner->icons()[i]);
        }
        else
        {
            base::writeBytes((atInt8*)tmpIcon, 48 * 48 * 2);
        }
    }

    delete[] tmpIcon; // delete tmp buffer;

    atUint8* hash = new atUint8[0x10];
    MD5Hash::MD5(hash, (atUint8*)base::data(), 0xF0C0);
    base::seek(0x0E, SeekOrigin::Begin);
    base::writeBytes((atInt8*)hash, 0x10);

    aes_set_key(SD_KEY);
    atUint8 data[0xF0C0];
    memcpy(data, base::data(), 0xF0C0);
    atUint8  tmpIV[26];
    memcpy(tmpIV, SD_IV, 16);
    aes_encrypt(tmpIV, data, data, 0xF0C0);

    base::seek(0, SeekOrigin::Begin);
    base::writeBytes((atInt8*)data, 0xF0C0);
    base::seek(0xF0C0, SeekOrigin::Begin);
}

atUint32 WiiSaveWriter::writeFile(WiiFile* file)
{
    atUint32 ret = 0x80;

    // Write the File magic
    base::writeUint32(0x03ADF17E);
    base::writeUint32(file->length());
    base::writeByte(file->permissions());
    base::writeByte(file->attributes());
    base::writeByte(file->type());

    atUint8 name[0x45];
    utility::fillRandom(name, 0x45);
    memcpy(name, file->fullpath().c_str(), file->fullpath().size());
    name[file->fullpath().size()] = '\0';
    base::writeBytes((atInt8*)name, 0x45);
    atUint8 iv[16];
    utility::fillRandom(iv, 0x10);
    base::writeBytes((atInt8*)iv, 0x10);
    atUint8 crap[0x20];
    utility::fillRandom(crap, 0x20);
    base::writeBytes((atInt8*)crap, 0x20);

    if (file->type() == WiiFile::File)
    {
        int roundedSize = (file->length() + 63) & ~63;
        atUint8* data = new atUint8[roundedSize];
        memset(data, 0, roundedSize);

        aes_set_key(SD_KEY);
        aes_encrypt(iv, file->data(), data, roundedSize);

        base::writeBytes((atInt8*)data, roundedSize);
        ret += roundedSize;
        delete[] data;
    }

    return ret;
}


void WiiSaveWriter::writeImage(WiiImage* image)
{
    atInt8* data = (atInt8*)image->data();
    base::writeBytes(data, image->width() * image->height() * 2);
}

void WiiSaveWriter::writeCerts(atUint32 filesSize, atUint32 ngId, atUint8* ngPriv, atUint8* ngSig, atUint32 ngKeyId)
{
    atUint8  sig[0x40];
    atUint8  ngCert[0x180];
    atUint8  apCert[0x180];
    atUint8* hash;
    atUint8  apPriv[30];
    atUint8  apSig[60];
    char   signer[64];
    char   name[64];
    atUint8* data;
    atUint32 dataSize;

    sprintf(signer, "Root-CA00000001-MS00000002");
    sprintf(name, "NG%08x", ngId);
    make_ec_cert(ngCert, ngSig, signer, name, ngPriv, ngKeyId);

    memset(apPriv, 0, 30);
    apPriv[10] = 1;

    memset(apSig, 81, 30);

    sprintf(signer, "Root-CA00000001-MS00000002-NG%08x", ngId);
    sprintf(name, "AP%08x%08x", 1, 2);
    make_ec_cert(apCert, apSig, signer, name, apPriv, 0);

    hash = getSha1(apCert + 0x80, 0x100);
    generate_ecdsa(apSig, apSig + 30, ngPriv, hash);
    make_ec_cert(apCert, apSig, signer, name, apPriv, 0);
    delete[] hash;

    dataSize = filesSize + 0x80;
    data = new atUint8[dataSize];
    atUint8* rawData = base::data();
    memcpy(data, rawData + 0xF0C0, dataSize);

    hash = getSha1(data, dataSize);
    atUint8* hash2 = getSha1(hash, 20);
    delete[] hash;
    delete[] data;

    generate_ecdsa(sig, sig + 30, apPriv, hash2);
    int stuff = 0x2f536969;

    if (!utility::isSystemBigEndian())
        stuff = utility::swap32(stuff);

    *(atUint32*)(sig + 60) = stuff;
    delete[] hash2;

    base::writeBytes((atInt8*)sig, 0x40);
    base::writeBytes((atInt8*)ngCert, 0x180);
    base::writeBytes((atInt8*)apCert, 0x180);
}

} // io
} // zelda
#endif // ATHENA_NO_SAVES
