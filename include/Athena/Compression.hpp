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
    atInt32 decompressZlib(atUint8* src, atUint32 srcLen, atUint8* dst, atUint32 dstLen);
    atInt32 compressZlib(const atUint8* src, atUint32 srcLen, atUint8* dst, atUint32 dstLen);

    // lzo compression
    atInt32 decompressLZO(atUint8* source, atInt32 sourceSize, atUint8* dest, atInt32& dstSize);

    // Yaz0 encoding
    atUint32 yaz0Decode(atUint8* src, atUint8* dst, atUint32 uncompressedSize);
    atUint32 yaz0Encode(atUint8* src, atUint32 srcSize, atUint8* data);

}
}
}
#endif // COMPRESSION_HPP
