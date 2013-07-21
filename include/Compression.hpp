#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

#include <Types.hpp>

namespace zelda
{
namespace io
{
namespace Compression
{
    Int32 decompressZlib(Uint8* src, Uint32 srcLen, Uint8* dst, Uint32 dstLen);
    void compressZlib(const Uint8* src, Uint32 srcLen, Uint8* dst, Uint32* dstLen);
}
}
}
#endif // COMPRESSION_HPP
