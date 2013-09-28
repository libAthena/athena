#ifndef CHECKSUMS_HPP
#define CHECKSUMS_HPP

#include <Types.hpp>

namespace zelda
{
namespace Checksums
{
Uint32 crc32(const Uint8* data, Uint64 length, Uint32 seed = 0xFFFFFFFF);
Uint16 crc16CCITT(const Uint8* data, Uint64 length, Uint16 seed = 0xFFFF, Uint16 final = 0);
Uint16 crc16(const Uint8* data, Uint64 length);
}
}
#endif // CHECKSUMS_HPP
