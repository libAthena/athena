#pragma once

#include "LZ77/LZBase.hpp"

class LZType10 : public LZBase {
public:
  explicit LZType10(int32_t minimumOffset = 1, int32_t SlidingWindow = 4096, int32_t MinimumMatch = 3,
                    int32_t BlockSize = 8);
  uint32_t compress(const uint8_t* src, uint8_t** dstBuf, uint32_t srcLength) override;
  uint32_t decompress(const uint8_t* src, uint8_t** dst, uint32_t srcLen) override;
};
