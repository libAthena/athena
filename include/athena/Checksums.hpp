#pragma once

#include "athena/Types.hpp"

namespace athena::checksums {
uint64_t crc64(const uint8_t* data, uint64_t length, uint64_t seed = 0xFFFFFFFFFFFFFFFF,
               uint64_t final = 0xFFFFFFFFFFFFFFFF);
uint32_t crc32(const uint8_t* data, uint64_t length, uint32_t seed = 0xFFFFFFFF, uint32_t final = 0xFFFFFFFF);
uint16_t crc16CCITT(const uint8_t* data, uint64_t length, uint16_t seed = 0xFFFF, uint16_t final = 0);
uint16_t crc16(const uint8_t* data, uint64_t length, uint16_t seed = 0, uint16_t final = 0);
} // namespace athena::checksums
