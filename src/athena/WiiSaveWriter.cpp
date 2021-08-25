#include "athena/WiiSaveWriter.hpp"
#include "athena/WiiSave.hpp"
#include "athena/WiiFile.hpp"
#include "athena/WiiBanner.hpp"
#include "athena/WiiImage.hpp"
#include "athena/WiiSave.hpp"
#include "athena/WiiFile.hpp"
#include "athena/WiiBanner.hpp"
#include "athena/MemoryWriter.hpp"
#include "athena/Utility.hpp"

#include "aes.hpp"
#include "ec.hpp"
#include "md5.h"
#include "sha1.h"

#include <cstdio>
#include <vector>
#include <cstring>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>

namespace athena {

namespace io {

WiiSaveWriter::WiiSaveWriter(const std::string& filename) : MemoryCopyWriter(filename) { setEndian(Endian::Big); }

bool WiiSaveWriter::writeSave(WiiSave* save, atUint8* macAddress, atUint32 ngId, atUint8* ngPriv, atUint8* ngSig,
                              atUint32 ngKeyId, const std::string& filepath) {
  if (!save) {
    atError(fmt("save cannot be NULL"));
    return false;
  }

  if (filepath != "")
    m_filepath = filepath;

  writeBanner(save->banner());

  writeUint32(0x70);
  writeUint32(0x426B0001);
  writeUint32(ngId); // NG-ID
  writeUint32(save->fileCount());
  writeUint32(0); // Size of files;
  seek(8);
  writeUint32(0); // totalSize
  seek(64);
  writeUint64(save->banner()->gameID());
  writeBytes((atInt8*)macAddress, 6);
  seek(2);    // unknown;
  seek(0x10); // padding;
  atUint32 totalSize = 0;

  for (WiiFile* file : save->allFiles()) {
    totalSize += writeFile(file);
  }

  atUint64 pos = position();
  // Write size data
  seek(0xF0C0 + 0x10, SeekOrigin::Begin);
  writeUint32(totalSize);
  seek(0xF0C0 + 0x1C, SeekOrigin::Begin);
  writeUint32(totalSize + 0x3c0);
  seek(pos, SeekOrigin::Begin);

  writeCerts(totalSize, ngId, ngPriv, ngSig, ngKeyId);

  this->save();

  return true;
}

void WiiSaveWriter::writeBanner(WiiBanner* banner) {
  setEndian(Endian::Big);
  writeInt64(banner->gameID());
  writeInt32((0x60a0 + 0x1200) * (atUint32)banner->icons().size());
  writeByte((atInt8)banner->permissions());
  seek(1);
  writeBytes((atInt8*)MD5_BLANKER, 16);
  seek(2);
  writeInt32(0x5749424E); // WIBN
  writeInt32(banner->flags());
  writeInt16(banner->animationSpeed());
  seek(22);

  for (char16_t c : banner->title())
    writeUint16(c);
  writeUint16(0);

  if (position() != 0x0080)
    seek(0x0080, SeekOrigin::Begin);

  for (char16_t c : banner->subtitle())
    writeUint16(c);
  writeUint16(0);

  if (position() != 0x00C0)
    seek(0x00C0, SeekOrigin::Begin);

  WiiImage* bannerImage = banner->bannerImage();
  writeBytes((atInt8*)bannerImage->data(), bannerImage->width() * bannerImage->height() * 2);

  // For empty icons
  atUint8* tmpIcon = new atUint8[48 * 48 * 2];
  memset(tmpIcon, 0, 48 * 48 * 2);

  for (atUint32 i = 0; i < 8; ++i) {
    if (i < banner->icons().size()) {
      writeImage(banner->icons()[i]);
    } else {
      writeBytes((atInt8*)tmpIcon, 48 * 48 * 2);
    }
  }

  delete[] tmpIcon; // delete tmp buffer;

  atUint8* hash = new atUint8[0x10];
  MD5Hash::MD5(hash, (atUint8*)data(), 0xF0C0);
  seek(0x0E, SeekOrigin::Begin);
  writeBytes((atInt8*)hash, 0x10);

  std::unique_ptr<IAES> aes = NewAES();
  aes->setKey(SD_KEY);
  atUint8 data[0xF0C0];
  memcpy(data, this->data(), 0xF0C0);
  atUint8 tmpIV[26];
  memcpy(tmpIV, SD_IV, 16);
  aes->encrypt(tmpIV, data, data, 0xF0C0);

  seek(0, SeekOrigin::Begin);
  writeBytes((atInt8*)data, 0xF0C0);
  seek(0xF0C0, SeekOrigin::Begin);
}

atUint32 WiiSaveWriter::writeFile(WiiFile* file) {
  atUint32 ret = 0x80;

  // Write the File magic
  writeUint32(0x03ADF17E);
  writeUint32(file->length());
  writeByte(file->permissions());
  writeByte(file->attributes());
  writeByte(file->type());

  atUint8 name[0x45];
  utility::fillRandom(name, 0x45);
  memcpy(name, file->fullpath().c_str(), file->fullpath().size());
  name[file->fullpath().size()] = '\0';
  writeBytes((atInt8*)name, 0x45);
  atUint8 iv[16];
  utility::fillRandom(iv, 0x10);
  writeBytes((atInt8*)iv, 0x10);
  atUint8 crap[0x20];
  utility::fillRandom(crap, 0x20);
  writeBytes((atInt8*)crap, 0x20);

  if (file->type() == WiiFile::File) {
    int roundedSize = (file->length() + 63) & ~63;
    atUint8* data = new atUint8[roundedSize];
    memset(data, 0, roundedSize);

    std::unique_ptr<IAES> aes = NewAES();
    aes->setKey(SD_KEY);
    aes->encrypt(iv, file->data(), data, roundedSize);

    writeBytes((atInt8*)data, roundedSize);
    ret += roundedSize;
    delete[] data;
  }

  return ret;
}

void WiiSaveWriter::writeImage(WiiImage* image) {
  atInt8* data = (atInt8*)image->data();
  writeBytes(data, image->width() * image->height() * 2);
}

void WiiSaveWriter::writeCerts(atUint32 filesSize, atUint32 ngId, atUint8* ngPriv, atUint8* ngSig, atUint32 ngKeyId) {
  atUint8 sig[0x40];
  atUint8 ngCert[0x180];
  atUint8 apCert[0x180];
  atUint8* hash;
  atUint8 apPriv[30];
  atUint8 apSig[60];
  char signer[64];
  char name[64];
  atUint8* buf;
  atUint32 dataSize;

  sprintf(signer, "Root-CA00000001-MS00000002");
  sprintf(name, "NG%08x", ngId);
  ecc::makeECCert(ngCert, ngSig, signer, name, ngPriv, ngKeyId);

  memset(apPriv, 0, 30);
  apPriv[10] = 1;

  memset(apSig, 81, 30);

  sprintf(signer, "Root-CA00000001-MS00000002-NG%08x", ngId);
  sprintf(name, "AP%08x%08x", 1, 2);
  ecc::makeECCert(apCert, apSig, signer, name, apPriv, 0);

  hash = getSha1(apCert + 0x80, 0x100);
  ecc::createECDSA(apSig, apSig + 30, ngPriv, hash);
  ecc::makeECCert(apCert, apSig, signer, name, apPriv, 0);
  delete[] hash;

  dataSize = filesSize + 0x80;
  buf = new atUint8[dataSize];
  atUint8* rawData = data();
  memcpy(buf, rawData + 0xF0C0, dataSize);

  hash = getSha1(buf, dataSize);
  atUint8* hash2 = getSha1(hash, 20);
  delete[] hash;
  delete[] buf;

  ecc::createECDSA(sig, sig + 30, apPriv, hash2);
  int stuff = 0x2f536969;

  if (!utility::isSystemBigEndian())
    stuff = utility::swap32(stuff);

  *(atUint32*)(sig + 60) = stuff;
  delete[] hash2;

  writeBytes((atInt8*)sig, 0x40);
  writeBytes((atInt8*)ngCert, 0x180);
  writeBytes((atInt8*)apCert, 0x180);
}

} // namespace io
} // namespace athena
