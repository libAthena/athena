#pragma once

#include <string>
#include "LZ77/LZLookupTable.hpp"

class LZBase {
public:
  explicit LZBase(atInt32 minimumOffset = 1, atInt32 slidingWindow = 4096, atInt32 minimumMatch = 3,
                  atInt32 blockSize = 8);
  virtual ~LZBase();

  virtual atUint32 compress(const atUint8* src, atUint8** dest, atUint32 srcLength) = 0;
  virtual atUint32 decompress(const atUint8* src, atUint8** dest, atUint32 srcLength) = 0;

  void setSlidingWindow(atInt32 SlidingWindow);
  atInt32 slidingWindow() const;
  void setReadAheadBuffer(atInt32 ReadAheadBuffer);
  atInt32 readAheadBuffer() const;
  void setMinMatch(atInt32 minimumMatch);
  atInt32 minMatch() const;
  void setBlockSize(atInt32 BlockSize);
  atInt32 blockSize() const;
  void setMinimumOffset(atUint32 minimumOffset);
  atUint32 minimumOffset() const;

protected:
  LZLengthOffset search(const atUint8* posPtr, const atUint8* dataBegin, const atUint8* dataEnd) const;

  atInt32 m_slidingWindow;
  atInt32 m_readAheadBuffer;
  atInt32 m_minMatch; // Minimum number of bytes that have to matched to go through with compression
  atInt32 m_blockSize;
  atUint32 m_minOffset;
  LZLookupTable m_lookupTable;
};
