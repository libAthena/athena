#include "Athena/Compression.hpp"
#include "Athena/Exception.hpp"
#include <lzo/lzo1x.h>
#include <iostream>
#include <zlib.h>
#include "LZ77/LZType10.hpp"
#include "LZ77/LZType11.hpp"

namespace Athena
{
namespace io
{
namespace Compression
{

atInt32 decompressZlib(const atUint8* src, atUint32 srcLen, atUint8* dst, atUint32 dstLen)
{
    z_stream strm = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef*) src;
    strm.next_out  = (Bytef*) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    atInt32 err = -1;
    atInt32 ret = -1;

    err = inflateInit(&strm); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib

    if (err == Z_OK)
    {
        err = inflate(&strm, Z_FINISH);

        if (err == Z_STREAM_END)
            ret = strm.total_out;
        else
        {
            inflateEnd(&strm);
            return err;
        }
    }
    else
    {
        inflateEnd(&strm);
        return err;
    }

    inflateEnd(&strm);

    return ret;
}

atInt32 compressZlib(const atUint8* src, atUint32 srcLen, atUint8* dst, atUint32 dstLen)
{
    z_stream strm = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef*) src;
    strm.next_out  = (Bytef*) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    atInt32 err = -1;
    atInt32 ret = -1;

    err = deflateInit(&strm, Z_BEST_COMPRESSION);

    if (err == Z_OK)
    {
        err = deflate(&strm, Z_FINISH);

        if (err == Z_STREAM_END)
            ret = strm.total_out;
        else
        {
            deflateEnd(&strm);
            return err;
        }
    }
    else
    {
        deflateEnd(&strm);
        return err;
    }

    deflateEnd(&strm);

    return ret;
}

atInt32 decompressLZO(const atUint8* source, const atInt32 sourceSize, atUint8* dst, atInt32& dstSize)
{
    int srcSize = sourceSize;
    lzo_uint size = dstSize;
    int result = lzo1x_decompress_safe(source, srcSize, dst, &size, NULL);
    dstSize -= (atInt32)size;

    return result;
}

//src points to the yaz0 source data (to the "real" source data, not at the header!)
//dst points to a buffer uncompressedSize bytes large (you get uncompressedSize from
//the second 4 bytes in the Yaz0 header).
atUint32 yaz0Decode(const atUint8* src, atUint8* dst, atUint32 uncompressedSize)
{
    atUint32 srcPlace = 0, dstPlace = 0; //current read/write positions

    atInt32 validBitCount = 0; //number of valid bits left in "code" byte
    atUint8 currCodeByte;

    while (dstPlace < uncompressedSize)
    {
        //read new "code" byte if the current one is used up
        if (validBitCount == 0)
        {
            currCodeByte = src[srcPlace];
            ++srcPlace;
            validBitCount = 8;
        }

        if ((currCodeByte & 0x80) != 0)
        {
            //straight copy
            dst[dstPlace] = src[srcPlace];
            dstPlace++;
            srcPlace++;
        }
        else
        {
            //RLE part
            atUint8 byte1 = src[srcPlace];
            atUint8 byte2 = src[srcPlace + 1];
            srcPlace += 2;

            atUint32 dist = ((byte1 & 0xF) << 8) | byte2;
            atUint32 copySource = dstPlace - (dist + 1);

            atUint32 numBytes = byte1 >> 4;

            if (numBytes == 0)
            {
                numBytes = src[srcPlace] + 0x12;
                srcPlace++;
            }
            else
                numBytes += 2;

            //copy run
            for (atUint32 i = 0; i < numBytes; ++i)
            {
                dst[dstPlace] = dst[copySource];
                copySource++;
                dstPlace++;
            }
        }

        //use next bit from "code" byte
        currCodeByte <<= 1;
        validBitCount -= 1;
    }

    return dstPlace;
}

// Yaz0 encode
typedef struct
{
    atUint32 srcPos, dstPos;
} yaz0_Ret;

atUint32 simpleEnc(const atUint8* src, atInt32 size, atInt32 pos, atUint32* pMatchPos);
atUint32 nintendoEnc(const atUint8* src, atInt32 size, atInt32 pos, atUint32* pMatchPos);

atUint32 yaz0Encode(const atUint8* src, atUint32 srcSize, atUint8* data)
{
    yaz0_Ret r = { 0, 0 };
    atInt32 pos = 0;
    atUint8 dst[24];    // 8 codes * 3 bytes maximum
    atUint32 dstSize = 0;
    atUint32 i;

    atUint32 validBitCount = 0; //number of valid bits left in "code" byte
    atUint8 currCodeByte = 0;

    while (r.srcPos < srcSize)
    {
        atUint32 numBytes;
        atUint32 matchPos;
        numBytes = nintendoEnc(src, srcSize, r.srcPos, &matchPos);

        if (numBytes < 3)
        {
            //straight copy
            dst[r.dstPos] = src[r.srcPos];
            r.dstPos++;
            r.srcPos++;
            //set flag for straight copy
            currCodeByte |= (0x80 >> validBitCount);
        }
        else
        {
            //RLE part
            atUint32 dist = r.srcPos - matchPos - 1;
            atUint8 byte1, byte2, byte3;

            if (numBytes >= 0x12)  // 3 byte encoding
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
            }
            else  // 2 byte encoding
            {
                byte1 = ((numBytes - 2) << 4) | (dist >> 8);
                byte2 = dist & 0xff;
                dst[r.dstPos++] = byte1;
                dst[r.dstPos++] = byte2;
            }

            r.srcPos += numBytes;
        }

        validBitCount++;

        //write eight codes
        if (validBitCount == 8)
        {
            data[pos] = currCodeByte;
            pos++;

            for (i = 0; i </*=*/r.dstPos; pos++, i++)
                data[pos] = dst[i];

            dstSize += r.dstPos + 1;

            currCodeByte = 0;
            validBitCount = 0;
            r.dstPos = 0;
        }
    }

    if (validBitCount > 0)
    {
        data[pos] = currCodeByte;
        pos++;

        for (i = 0; i </*=*/r.dstPos; pos++, i++)
            data[pos] = dst[i];

        dstSize += r.dstPos + 1;

        currCodeByte = 0;
        validBitCount = 0;
        r.dstPos = 0;
    }

    return dstSize;
}

// a lookahead encoding scheme for ngc Yaz0
atUint32 nintendoEnc(const atUint8* src, atInt32 size, atInt32 pos, atUint32* pMatchPos)
{
    atUint32 numBytes = 1;
    static atUint32 numBytes1;
    static atUint32 matchPos;
    static atInt32 prevFlag = 0;

    // if prevFlag is set, it means that the previous position was determined by look-ahead try.
    // so just use it. this is not the best optimization, but nintendo's choice for speed.
    if (prevFlag == 1)
    {
        *pMatchPos = matchPos;
        prevFlag = 0;
        return numBytes1;
    }

    prevFlag = 0;
    numBytes = simpleEnc(src, size, pos, &matchPos);
    *pMatchPos = matchPos;

    // if this position is RLE encoded, then compare to copying 1 byte and next position(pos+1) encoding
    if (numBytes >= 3)
    {
        numBytes1 = simpleEnc(src, size, pos + 1, &matchPos);

        // if the next position encoding is +2 longer than current position, choose it.
        // this does not guarantee the best optimization, but fairly good optimization with speed.
        if (numBytes1 >= numBytes + 2)
        {
            numBytes = 1;
            prevFlag = 1;
        }
    }

    return numBytes;
}

// simple and straight encoding scheme for Yaz0
atUint32 simpleEnc(const atUint8* src, atInt32 size, atInt32 pos, atUint32* pMatchPos)
{
    int startPos = pos - 0x1000, j, i;
    atUint32 numBytes = 1;
    atUint32 matchPos = 0;

    if (startPos < 0)
        startPos = 0;

    for (i = startPos; i < pos; i++)
    {
        for (j = 0; j < size - pos; j++)
        {
            if (src[i + j] != src[j + pos])
                break;
        }

        if ((atUint32)j > numBytes)
        {
            numBytes = j;
            matchPos = i;
        }
    }

    *pMatchPos = matchPos;

    if (numBytes == 2)
        numBytes = 1;

    return numBytes;
}

atUint32 decompressLZ77(const atUint8* src, atUint32 srcLen, atUint8** dst)
{
    if (*(atUint8*)src == 0x11)
        return LZType11().decompress(src, dst, srcLen);

    return LZType10(2).decompress(src, dst, srcLen);
}

atUint32 compressLZ77(const atUint8* src, atUint32 srcLen, atUint8** dst, bool extended)
{
    if (extended)
        return LZType11().compress(src, dst, srcLen);

    return LZType10(2).compress(src, dst, srcLen);
}

} // Compression
} // io
} // zelda
