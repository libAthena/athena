#ifndef CHECKSUMS_HPP
#define CHECKSUMS_HPP

#include "athena/Global.hpp"

namespace athena::Checksums
{
atUint64 crc64(const atUint8* data, atUint64 length, atUint64 mask = 0xFFFFFFFFFFFFFFFF, atUint64 seed = 0xFFFFFFFFFFFFFFFF);
atUint32 crc32(const atUint8* data, atUint64 length, atUint32 mask = 0xFFFFFFFF, atUint32 seed = 0xFFFFFFFF);
atUint16 crc16CCITT(const atUint8* data, atUint64 length, atUint16 seed = 0xFFFF, atUint16 final = 0);
atUint16 crc16(const atUint8* data, atUint64 length);
}
#endif // CHECKSUMS_HPP
