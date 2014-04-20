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

#include "Athena/Compression.hpp"
#include "Athena/Exception.hpp"
#include "lzo.h"
#include <iostream>
#include <zlib.h>

namespace Athena
{
namespace io
{
namespace Compression
{

Int32 decompressZlib(Uint8 *src, Uint32 srcLen, Uint8* dst, Uint32 dstLen)
{
    z_stream strm = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    Int32 err = -1;
    Int32 ret = -1;

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

Int32 compressZlib(const Uint8 *src, Uint32 srcLen, Uint8 *dst, Uint32 dstLen)
{
    z_stream strm = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    Int32 err = -1;
    Int32 ret = -1;

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

Int32 decompressLZO(Uint8* source, Int32 sourceSize, Uint8* dest, Int32& dstSize)
{
    int size = dstSize;
    int result = lzo1x_decode(dest, &size, source, &sourceSize);
    dstSize = size;
    return result;
}

//src points to the yaz0 source data (to the "real" source data, not at the header!)
//dst points to a buffer uncompressedSize bytes large (you get uncompressedSize from
//the second 4 bytes in the Yaz0 header).
Uint32 yaz0Decode(Uint8* src, Uint8* dst, Uint32 uncompressedSize)
{
    Uint32 srcPlace = 0, dstPlace = 0; //current read/write positions

    Int32 validBitCount = 0; //number of valid bits left in "code" byte
    Uint8 currCodeByte;
    while(dstPlace < uncompressedSize)
    {
        //read new "code" byte if the current one is used up
        if(validBitCount == 0)
        {
            currCodeByte = src[srcPlace];
            ++srcPlace;
            validBitCount = 8;
        }

        if((currCodeByte & 0x80) != 0)
        {
            //straight copy
            dst[dstPlace] = src[srcPlace];
            dstPlace++;
            srcPlace++;
        }
        else
        {
            //RLE part
            Uint8 byte1 = src[srcPlace];
            Uint8 byte2 = src[srcPlace + 1];
            srcPlace += 2;

            Uint32 dist = ((byte1 & 0xF) << 8) | byte2;
            Uint32 copySource = dstPlace - (dist + 1);

            Uint32 numBytes = byte1 >> 4;
            if(numBytes == 0)
            {
                numBytes = src[srcPlace] + 0x12;
                srcPlace++;
            }
            else
                numBytes += 2;

            //copy run
            for(Uint32 i = 0; i < numBytes; ++i)
            {
                dst[dstPlace] = dst[copySource];
                copySource++;
                dstPlace++;
            }
        }

        //use next bit from "code" byte
        currCodeByte <<= 1;
        validBitCount-=1;
    }

    return dstPlace;
}

// Yaz0 encode
typedef struct
{
    Uint32 srcPos, dstPos;
} yaz0_Ret;

Uint32 simpleEnc(Uint8* src, Int32 size, Int32 pos, Uint32 *pMatchPos);
Uint32 nintendoEnc(Uint8* src, Int32 size, Int32 pos, Uint32 *pMatchPos);

Uint32 yaz0Encode(Uint8* src, Uint32 srcSize, Uint8* data)
{
    yaz0_Ret r = { 0, 0 };
    Int32 pos = 0;
    Uint8 dst[24];    // 8 codes * 3 bytes maximum
    Uint32 dstSize = 0;
    Uint32 i;

    Uint32 validBitCount = 0; //number of valid bits left in "code" byte
    Uint8 currCodeByte = 0;
    while(r.srcPos < srcSize)
    {
        Uint32 numBytes;
        Uint32 matchPos;
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
            Uint32 dist = r.srcPos - matchPos - 1;
            Uint8 byte1, byte2, byte3;

            if (numBytes >= 0x12)  // 3 byte encoding
            {
                byte1 = 0 | (dist >> 8);
                byte2 = dist & 0xff;
                dst[r.dstPos++] = byte1;
                dst[r.dstPos++] = byte2;
                // maximum runlength for 3 byte encoding
                if (numBytes > 0xff+0x12)
                    numBytes = 0xff+0x12;
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
        if(validBitCount == 8)
        {
            data[pos] = currCodeByte;
            pos++;
            for (i=0;i</*=*/r.dstPos;pos++,i++)
                data[pos] = dst[i];
            dstSize += r.dstPos+1;

            currCodeByte = 0;
            validBitCount = 0;
            r.dstPos = 0;
        }
    }
    if(validBitCount > 0)
    {
        data[pos] = currCodeByte;
        pos++;
        for (i=0;i</*=*/r.dstPos;pos++,i++)
            data[pos] = dst[i];
        dstSize += r.dstPos+1;

        currCodeByte = 0;
        validBitCount = 0;
        r.dstPos = 0;
    }

    return dstSize;
}

// a lookahead encoding scheme for ngc Yaz0
Uint32 nintendoEnc(Uint8* src, Int32 size, Int32 pos, Uint32 *pMatchPos)
{
    Uint32 numBytes = 1;
    static Uint32 numBytes1;
    static Uint32 matchPos;
    static Int32 prevFlag = 0;

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
        numBytes1 = simpleEnc(src, size, pos+1, &matchPos);
        // if the next position encoding is +2 longer than current position, choose it.
        // this does not guarantee the best optimization, but fairly good optimization with speed.
        if (numBytes1 >= numBytes+2) {
            numBytes = 1;
            prevFlag = 1;
        }
    }
    return numBytes;
}

// simple and straight encoding scheme for Yaz0
Uint32 simpleEnc(Uint8* src, Int32 size, Int32 pos, Uint32 *pMatchPos)
{
    int startPos = pos - 0x1000, j, i;
    Uint32 numBytes = 1;
    Uint32 matchPos = 0;

    if (startPos < 0)
        startPos = 0;
    for (i = startPos; i < pos; i++)
    {
        for (j = 0; j < size-pos; j++)
        {
            if (src[i+j] != src[j+pos])
                break;
        }
        if ((Uint32)j > numBytes)
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



} // Compression
} // io
} // zelda
