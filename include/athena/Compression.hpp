#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

#include "athena/Global.hpp"

namespace athena
{
namespace io
{
namespace Compression
{
// Zlib compression
atInt32 decompressZlib(const atUint8* src, atUint32 srcLen, atUint8* dst, atUint32 dstLen);
atInt32 compressZlib(const atUint8* src, atUint32 srcLen, atUint8* dst, atUint32 dstLen);

// lzo compression
atInt32 decompressLZO(const atUint8* source, atInt32 sourceSize, atUint8* dst, atInt32& dstSize);

// Yaz0 encoding
atUint32 yaz0Decode(const atUint8* src, atUint8* dst, atUint32 uncompressedSize);
atUint32 yaz0Encode(const atUint8* src, atUint32 srcSize, atUint8* data);

atUint32 decompressLZ77(const atUint8* src, atUint32 srcLen, atUint8** dst);
atUint32 compressLZ77(const atUint8* src, atUint32 srcLen, atUint8** dst, bool extended = false);
}
}
}
#endif // COMPRESSION_HPP
