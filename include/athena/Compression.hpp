#pragma once

#include "athena/Types.hpp"

namespace athena::io::Compression {
// Zlib compression
int32_t decompressZlib(const uint8_t* src, uint32_t srcLen, uint8_t* dst, uint32_t dstLen);
int32_t compressZlib(const uint8_t* src, uint32_t srcLen, uint8_t* dst, uint32_t dstLen);

#if AT_LZOKAY
// lzo compression
atInt32 decompressLZO(const atUint8* source, atInt32 sourceSize, atUint8* dst, atInt32& dstSize);
#endif

// Yaz0 encoding
uint32_t yaz0Decode(const uint8_t* src, uint8_t* dst, uint32_t uncompressedSize);
uint32_t yaz0Encode(const uint8_t* src, uint32_t srcSize, uint8_t* data);

uint32_t decompressLZ77(const uint8_t* src, uint32_t srcLen, uint8_t** dst);
uint32_t compressLZ77(const uint8_t* src, uint32_t srcLen, uint8_t** dst, bool extended = false);
} // namespace athena::io::Compression
