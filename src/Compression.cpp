#include "Compression.hpp"

#include <zlib.h>

namespace zelda
{
namespace io
{
namespace Compression
{

Int32 decompressZlib(Uint8 *src, Uint32 srcLen, Uint8* dst, Uint32 dstLen)
{
    z_stream strm;
    strm.total_in  = strm.avail_in  = srcLen;
    strm.total_out = strm.avail_out = dstLen;
    strm.next_in   = (Bytef *) src;
    strm.next_out  = (Bytef *) dst;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    Int32 err = -1;
    Int32 ret = -1;

    err = inflateInit2(&strm, (15 + 32)); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
    if (err == Z_OK) {
        err = inflate(&strm, Z_FINISH);
        if (err == Z_STREAM_END) {
            ret = strm.total_out;
        }
        else {
             inflateEnd(&strm);
             return err;
        }
    }
    else {
        inflateEnd(&strm);
        return err;
    }

    inflateEnd(&strm);

    return ret;
}

void compressZlib(const Uint8 *src, Uint32 srcLen, Uint8 *dst, Uint32* dstLen)
{
    compress(dst, (uLongf*)dstLen, src, srcLen);
}

} // Compression
} // io
} // zelda
