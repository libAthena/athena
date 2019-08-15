#pragma once

#include "LZBase.hpp"

class LZType10 : public LZBase {
public:
  explicit LZType10(atInt32 minimumOffset = 1, atInt32 SlidingWindow = 4096, atInt32 MinimumMatch = 3,
                    atInt32 BlockSize = 8);
  atUint32 compress(const atUint8* src, atUint8** dstBuf, atUint32 srcLength);
  atUint32 decompress(const atUint8* src, atUint8** dst, atUint32 srcLen);
};
