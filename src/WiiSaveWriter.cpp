#include "WiiSaveWriter.hpp"

#include "WiiSave.h"
#include "WiiFile.h"
#include "WiiBanner.h"

#include "WiiSave.h"
#include "WiiFile.h"
#include "WiiBanner.h"
#include "BinaryWriter.hpp"
#include "IOException.hpp"
#include "aes.h"
#include "ec.h"
#include <utility.hpp>
#include "md5.h"
#include "sha1.h"

#include <stdio.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>


const Uint8 SD_KEY[16]      = {0xab, 0x01, 0xb9, 0xd8, 0xe1, 0x62, 0x2b, 0x08, 0xaf, 0xba, 0xd8, 0x4d, 0xbf, 0xc2, 0xa5, 0x5d};
const Uint8 SD_IV[16]       = {0x21, 0x67, 0x12, 0xe6, 0xaa, 0x1f, 0x68, 0x9f, 0x95, 0xc5, 0xa2, 0x23, 0x24, 0xdc, 0x6a, 0x98};
const Uint8 MD5_BLANKER[16] = {0x0e, 0x65, 0x37, 0x81, 0x99, 0xbe, 0x45, 0x17, 0xab, 0x06, 0xec, 0x22, 0x45, 0x1a, 0x57, 0x93};

WiiSaveWriter::WiiSaveWriter(const std::string &filename)
    : BinaryWriter(filename)
{
    this->setAutoResizing(true);
    this->setEndianess(Stream::BigEndian);
}


bool WiiSaveWriter::writeSave(WiiSave *save, Uint8 *macAddress, Uint32 ngId, Uint8 *ngPriv, Uint8 *ngSig, Uint32 ngKeyId,const std::string &filepath)
{
    if (filepath != "")
        m_filepath = filepath;

    writeBanner(save->banner());

    this->writeUInt32(0x70);
    this->writeUInt32(0x426B0001);
    this->writeUInt32(ngId); // NG-ID
    this->writeUInt32(save->fileList().size());
    this->writeUInt32(0); // Size of files;
    this->seek(8);
    this->writeUInt32(0); // totalSize
    this->seek(64);
    this->writeUInt64(save->banner()->gameID());
    this->writeBytes((Int8*)macAddress, 6);
    this->seek(2); // unknown;
    this->seek(0x10); // padding;
    Uint32 totalSize = 0;
    for (std::unordered_map<std::string, WiiFile*>::const_iterator iter = save->fileList().begin(); iter != save->fileList().end(); ++iter)
    {
        totalSize += writeFile(iter->second);
    }
    int pos = this->position();
    // Write size data
    this->seek(0xF0C0 + 0x10, Stream::Beginning);
    this->writeUInt32(totalSize);
    this->seek(0xF0C0 + 0x1C, Stream::Beginning);
    this->writeUInt32(totalSize + 0x3c0);
    this->seek(pos, Stream::Beginning);

    writeCerts(totalSize, ngId, ngPriv, ngSig, ngKeyId);

    this->save();

    return true;
}

void WiiSaveWriter::writeBanner(WiiBanner *banner)
{
    this->setEndianess(Stream::BigEndian);
    this->setAutoResizing(true);
    this->writeInt64(banner->gameID());
    this->writeInt32((0x60a0+0x1200)*banner->icons().size());
    this->writeByte((Int8)banner->permissions());
    this->seek(1);
    this->writeBytes((Int8*)MD5_BLANKER, 16);
    this->seek(2);
    this->writeInt32(0x5749424E); // WIBN
    this->writeInt32(banner->flags());
    this->writeInt16(banner->animationSpeed());
    this->seek(22);

    this->writeUnicode(banner->title());

    if (this->position() != 0x0080)
        this->seek(0x0080, Stream::Beginning);

    this->writeUnicode(banner->subtitle());

    if (this->position() != 0x00C0)
        this->seek(0x00C0, Stream::Beginning);

    WiiImage* bannerImage = banner->bannerImage();
    this->writeBytes((Int8*)bannerImage->data(), bannerImage->width()*bannerImage->height()*2);

    // For empty icons
    Uint8* tmpIcon = new Uint8[48*48*2];
    memset(tmpIcon, 0, 48*48*2);
    for (Uint32 i = 0; i < 8; ++i)
    {
        if (i < banner->icons().size())
        {
            writeImage(banner->icons()[i]);
        }
        else
        {
            this->writeBytes((Int8*)tmpIcon, 48*48*2);
        }
    }

    delete[] tmpIcon; // delete tmp buffer;

    Uint8* hash = new Uint8[0x10];
    MD5(hash, (Uint8*)this->data(), 0xF0C0);
    this->seek(0x0E, Stream::Beginning);
    this->writeBytes((Int8*)hash, 0x10);

    aes_set_key(SD_KEY);
    Uint8 data[0xF0C0];
    memcpy(data, this->data(), 0xF0C0);
    Uint8  tmpIV[26];
    memcpy(tmpIV, SD_IV, 16);
    aes_encrypt(tmpIV, data, data, 0xF0C0);

    this->seek(0, Stream::Beginning);
    this->writeBytes((Int8*)data, 0xF0C0);
    this->seek(0xF0C0, Stream::Beginning);
}

Uint32 WiiSaveWriter::writeFile(WiiFile *file)
{
    Uint32 ret = 0x80;

    // Write the File magic
    this->writeUInt32(0x03ADF17E);
    this->writeUInt32(file->length());
    this->writeByte(file->permissions());
    this->writeByte(file->attributes());
    this->writeByte(file->type());

    Uint8 name[0x45];
    fillRandom(name, 0x45);
    memcpy(name, file->filename().c_str(), file->filename().size());
    name[file->filename().size()] = '\0';
    this->writeBytes((Int8*)name, 0x45);
    Uint8 iv[16];
    fillRandom(iv, 0x10);
    this->writeBytes((Int8*)iv, 0x10);
    Uint8 crap[0x20];
    fillRandom(crap, 0x20);
    this->writeBytes((Int8*)crap, 0x20);

    if (file->type() == WiiFile::File)
    {
        int roundedSize = (file->length() + 63) & ~63;
        Uint8* data = new Uint8[roundedSize];
        memset(data, 0, roundedSize);

        aes_set_key(SD_KEY);
        aes_encrypt(iv, file->data(), data, roundedSize);

        this->writeBytes((Int8*)data, roundedSize);
        ret += roundedSize;
        delete[] data;
    }

    return ret;
}


void WiiSaveWriter::writeImage(WiiImage* image)
{
    Int8* data = (Int8*)image->data();
    this->writeBytes(data, image->width() * image->height() * 2);
}

void WiiSaveWriter::writeCerts(Uint32 filesSize, Uint32 ngId, Uint8 *ngPriv, Uint8 *ngSig, Uint32 ngKeyId)
{
    Uint8  sig[0x40];
    Uint8  ngCert[0x180];
    Uint8  apCert[0x180];
    Uint8* hash;
    Uint8  apPriv[30];
    Uint8  apSig[60];
    char   signer[64];
    char   name[64];
    Uint8* data;
    Uint32 dataSize;

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
    generate_ecdsa(apSig, apSig+30, ngPriv, hash);
    make_ec_cert(apCert, apSig, signer, name, apPriv, 0);
    delete[] hash;

    dataSize = filesSize + 0x80;
    data = new Uint8[dataSize];
    Uint8* rawData = this->data();
    memcpy(data, rawData + 0xF0C0, dataSize);

    hash = getSha1(data, dataSize);
    Uint8* hash2 = getSha1(hash, 20);
    delete[] hash;
    delete[] data;

    generate_ecdsa(sig, sig+30, apPriv, hash2);
    int stuff = 0x2f536969;
    if (!isSystemBigEndian())
        stuff = swap32(stuff);

    *(Uint32*)(sig+60) = stuff;
    delete[] hash2;

    this->writeBytes((Int8*)sig, 0x40);
    this->writeBytes((Int8*)ngCert, 0x180);
    this->writeBytes((Int8*)apCert, 0x180);
}
