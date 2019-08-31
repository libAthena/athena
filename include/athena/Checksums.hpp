#pragma once

#include "athena/Types.hpp"

namespace athena::checksums {
atUint64 crc64(const atUint8* data, atUint64 length, atUint64 seed = 0xFFFFFFFFFFFFFFFF,
               atUint64 final = 0xFFFFFFFFFFFFFFFF);
atUint32 crc32(const atUint8* data, atUint64 length, atUint32 seed = 0xFFFFFFFF, atUint32 final = 0xFFFFFFFF);
atUint16 crc16CCITT(const atUint8* data, atUint64 length, atUint16 seed = 0xFFFF, atUint16 final = 0);
atUint16 crc16(const atUint8* data, atUint64 length, atUint16 seed = 0, atUint16 final = 0);
} // namespace athena::checksums
