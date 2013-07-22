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

#include "Compression.hpp"
#include "Exception.hpp"
#include <iostream>

#include <zlib.h>

namespace zelda
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

} // Compression
} // io
} // zelda
