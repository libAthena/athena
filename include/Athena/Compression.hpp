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

#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

#include "Athena/Global.hpp"

namespace Athena
{
namespace io
{
namespace Compression
{
    // Zlib compression
    Int32 decompressZlib(Uint8* src, Uint32 srcLen, Uint8* dst, Uint32 dstLen);
    Int32 compressZlib(const Uint8* src, Uint32 srcLen, Uint8* dst, Uint32 dstLen);

    // lzo compression
    Int32 decompressLZO(Uint8* source, Int32 sourceSize, Uint8* dest, Int32& dstSize);

    // Yaz0 encoding
    Uint32 yaz0Decode(Uint8* src, Uint8* dst, Uint32 uncompressedSize);
    Uint32 yaz0Encode(Uint8* src, Uint32 srcSize, Uint8* data);

}
}
}
#endif // COMPRESSION_HPP
