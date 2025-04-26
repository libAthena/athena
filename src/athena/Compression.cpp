#include "athena/Compression.hpp"

#if AT_LZOKAY
#include <lzokay.hpp>
#endif

#include <zlib.h>
#include "LZ77/LZType10.hpp"
#include "LZ77/LZType11.hpp"

namespace athena::io::Compression {

void zlibInitZStrm(const uint8_t* src, uint32_t srcLen, uint8_t* dst, uint32_t dstLen, z_stream& strm) {
  strm.avail_in = srcLen;
  strm.avail_out = dstLen;
  strm.next_in = const_cast<Bytef*>(src);
  strm.next_out = dst;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
}
int32_t zlibInflate(z_stream& strm, int32_t bits) {
  int32_t ret = inflateInit2(&strm, bits);
  if (ret == Z_OK) {
    ret = inflate(&strm, Z_FINISH);
    if (ret == Z_STREAM_END) {
      ret = strm.total_out;
    }
  }
  return ret;
}
int32_t decompressZlib(const uint8_t* src, uint32_t srcLen, uint8_t* dst, uint32_t dstLen) {
  z_stream strm = {};
  zlibInitZStrm(src, srcLen, dst, dstLen, strm);

  // 15 window bits, and the | 16 tells zlib to to detect if using gzip or zlib
  int32_t ret = zlibInflate(strm, MAX_WBITS | 16);
  if (ret != Z_STREAM_END) {
    // Try again without gzip
    zlibInitZStrm(src, srcLen, dst, dstLen, strm);
    ret = zlibInflate(strm, MAX_WBITS);
    if (ret == Z_STREAM_END) {
      ret = strm.total_out;
    }
  } else {
    ret = strm.total_out;
  }
  inflateEnd(&strm);
  return ret;
}

int32_t compressZlib(const uint8_t* src, uint32_t srcLen, uint8_t* dst, uint32_t dstLen) {
  z_stream strm = {};
  strm.total_in = strm.avail_in = srcLen;
  strm.total_out = strm.avail_out = dstLen;
  strm.next_in = (Bytef*)src;
  strm.next_out = (Bytef*)dst;

  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;

  int32_t err = -1;
  int32_t ret = -1;

  err = deflateInit(&strm, Z_BEST_COMPRESSION);

  if (err == Z_OK) {
    err = deflate(&strm, Z_FINISH);

    if (err == Z_STREAM_END)
      ret = strm.total_out;
    else {
      deflateEnd(&strm);
      return err;
    }
  } else {
    deflateEnd(&strm);
    return err;
  }

  deflateEnd(&strm);

  return ret;
}

#if AT_LZOKAY
atInt32 decompressLZO(const atUint8* source, const atInt32 sourceSize, atUint8* dst, atInt32& dstSize) {
  size_t size = dstSize;
  auto result = lzokay::decompress(source, sourceSize, dst, size);
  dstSize -= (atInt32)size;

  return (atInt32)result;
}
#endif

// src points to the yaz0 source data (to the "real" source data, not at the header!)
// dst points to a buffer uncompressedSize bytes large (you get uncompressedSize from
// the second 4 bytes in the Yaz0 header).
uint32_t yaz0Decode(const uint8_t* src, uint8_t* dst, uint32_t uncompressedSize) {
  uint32_t srcPlace = 0, dstPlace = 0; // current read/write positions

  int32_t validBitCount = 0; // number of valid bits left in "code" byte
  uint8_t currCodeByte;

  while (dstPlace < uncompressedSize) {
    // read new "code" byte if the current one is used up
    if (validBitCount == 0) {
      currCodeByte = src[srcPlace];
      ++srcPlace;
      validBitCount = 8;
    }

    if ((currCodeByte & 0x80) != 0) {
      // straight copy
      dst[dstPlace] = src[srcPlace];
      dstPlace++;
      srcPlace++;
    } else {
      // RLE part
      uint8_t byte1 = src[srcPlace];
      uint8_t byte2 = src[srcPlace + 1];
      srcPlace += 2;

      uint32_t dist = ((byte1 & 0xF) << 8) | byte2;
      uint32_t copySource = dstPlace - (dist + 1);

      uint32_t numBytes = byte1 >> 4;

      if (numBytes == 0) {
        numBytes = src[srcPlace] + 0x12;
        srcPlace++;
      } else
        numBytes += 2;

      // copy run
      for (uint32_t i = 0; i < numBytes; ++i) {
        dst[dstPlace] = dst[copySource];
        copySource++;
        dstPlace++;
      }
    }

    // use next bit from "code" byte
    currCodeByte <<= 1;
    validBitCount -= 1;
  }

  return dstPlace;
}

// Yaz0 encode
typedef struct {
  uint32_t srcPos, dstPos;
} yaz0_Ret;

uint32_t simpleEnc(const uint8_t* src, int32_t size, int32_t pos, uint32_t* pMatchPos);
uint32_t nintendoEnc(const uint8_t* src, int32_t size, int32_t pos, uint32_t* pMatchPos);

uint32_t yaz0Encode(const uint8_t* src, uint32_t srcSize, uint8_t* data) {
  yaz0_Ret r = {0, 0};
  int32_t pos = 0;
  uint8_t dst[24]; // 8 codes * 3 bytes maximum
  uint32_t dstSize = 0;
  uint32_t i;

  uint32_t validBitCount = 0; // number of valid bits left in "code" byte
  uint8_t currCodeByte = 0;

  while (r.srcPos < srcSize) {
    uint32_t numBytes;
    uint32_t matchPos;
    numBytes = nintendoEnc(src, srcSize, r.srcPos, &matchPos);

    if (numBytes < 3) {
      // straight copy
      dst[r.dstPos] = src[r.srcPos];
      r.dstPos++;
      r.srcPos++;
      // set flag for straight copy
      currCodeByte |= (0x80 >> validBitCount);
    } else {
      // RLE part
      uint32_t dist = r.srcPos - matchPos - 1;
      uint8_t byte1, byte2, byte3;

      if (numBytes >= 0x12) // 3 byte encoding
      {
        byte1 = 0 | (dist >> 8);
        byte2 = dist & 0xff;
        dst[r.dstPos++] = byte1;
        dst[r.dstPos++] = byte2;

        // maximum runlength for 3 byte encoding
        if (numBytes > 0xff + 0x12)
          numBytes = 0xff + 0x12;

        byte3 = numBytes - 0x12;
        dst[r.dstPos++] = byte3;
      } else // 2 byte encoding
      {
        byte1 = ((numBytes - 2) << 4) | (dist >> 8);
        byte2 = dist & 0xff;
        dst[r.dstPos++] = byte1;
        dst[r.dstPos++] = byte2;
      }

      r.srcPos += numBytes;
    }

    validBitCount++;

    // write eight codes
    if (validBitCount == 8) {
      data[pos] = currCodeByte;
      pos++;

      for (i = 0; i < /*=*/r.dstPos; pos++, i++)
        data[pos] = dst[i];

      dstSize += r.dstPos + 1;

      currCodeByte = 0;
      validBitCount = 0;
      r.dstPos = 0;
    }
  }

  if (validBitCount > 0) {
    data[pos] = currCodeByte;
    pos++;

    for (i = 0; i < /*=*/r.dstPos; pos++, i++)
      data[pos] = dst[i];

    dstSize += r.dstPos + 1;

    currCodeByte = 0;
    validBitCount = 0;
    r.dstPos = 0;
  }

  return dstSize;
}

// a lookahead encoding scheme for ngc Yaz0
uint32_t nintendoEnc(const uint8_t* src, int32_t size, int32_t pos, uint32_t* pMatchPos) {
  uint32_t numBytes = 1;
  static uint32_t numBytes1;
  static uint32_t matchPos;
  static int32_t prevFlag = 0;

  // if prevFlag is set, it means that the previous position was determined by look-ahead try.
  // so just use it. this is not the best optimization, but nintendo's choice for speed.
  if (prevFlag == 1) {
    *pMatchPos = matchPos;
    prevFlag = 0;
    return numBytes1;
  }

  prevFlag = 0;
  numBytes = simpleEnc(src, size, pos, &matchPos);
  *pMatchPos = matchPos;

  // if this position is RLE encoded, then compare to copying 1 byte and next position(pos+1) encoding
  if (numBytes >= 3) {
    numBytes1 = simpleEnc(src, size, pos + 1, &matchPos);

    // if the next position encoding is +2 longer than current position, choose it.
    // this does not guarantee the best optimization, but fairly good optimization with speed.
    if (numBytes1 >= numBytes + 2) {
      numBytes = 1;
      prevFlag = 1;
    }
  }

  return numBytes;
}

// simple and straight encoding scheme for Yaz0
uint32_t simpleEnc(const uint8_t* src, int32_t size, int32_t pos, uint32_t* pMatchPos) {
  int startPos = pos - 0x1000, j, i;
  uint32_t numBytes = 1;
  uint32_t matchPos = 0;

  if (startPos < 0)
    startPos = 0;

  for (i = startPos; i < pos; i++) {
    for (j = 0; j < size - pos; j++) {
      if (src[i + j] != src[j + pos])
        break;
    }

    if ((uint32_t)j > numBytes) {
      numBytes = j;
      matchPos = i;
    }
  }

  *pMatchPos = matchPos;

  if (numBytes == 2)
    numBytes = 1;

  return numBytes;
}

uint32_t decompressLZ77(const uint8_t* src, uint32_t srcLen, uint8_t** dst) {
  if (*src == 0x11) {
    return LZType11().decompress(src, dst, srcLen);
  }

  return LZType10(2).decompress(src, dst, srcLen);
}

uint32_t compressLZ77(const uint8_t* src, uint32_t srcLen, uint8_t** dst, bool extended) {
  if (extended)
    return LZType11().compress(src, dst, srcLen);

  return LZType10(2).compress(src, dst, srcLen);
}

} // namespace athena::io::Compression
