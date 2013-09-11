#ifndef CHECKSUMS_HPP
#define CHECKSUMS_HPP

#include <Types.hpp>

namespace zelda
{
namespace Checksums
{
Uint32 crc32(Uint8* data, Uint64 length);
}
}
#endif // CHECKSUMS_HPP
