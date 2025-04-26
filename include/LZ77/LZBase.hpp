#pragma once

#include "LZ77/LZLookupTable.hpp"

class LZBase {
public:
  explicit LZBase(int32_t minimumOffset = 1, int32_t slidingWindow = 4096, int32_t minimumMatch = 3,
                  int32_t blockSize = 8);
  virtual ~LZBase();

  virtual uint32_t compress(const uint8_t* src, uint8_t** dest, uint32_t srcLength) = 0;
  virtual uint32_t decompress(const uint8_t* src, uint8_t** dest, uint32_t srcLength) = 0;

  void setSlidingWindow(int32_t SlidingWindow);
  int32_t slidingWindow() const;
  void setReadAheadBuffer(int32_t ReadAheadBuffer);
  int32_t readAheadBuffer() const;
  void setMinMatch(int32_t minimumMatch);
  int32_t minMatch() const;
  void setBlockSize(int32_t BlockSize);
  int32_t blockSize() const;
  void setMinimumOffset(uint32_t minimumOffset);
  uint32_t minimumOffset() const;

protected:
  LZLengthOffset search(const uint8_t* posPtr, const uint8_t* dataBegin, const uint8_t* dataEnd) const;

  int32_t m_slidingWindow;
  int32_t m_readAheadBuffer;
  int32_t m_minMatch; // Minimum number of bytes that have to matched to go through with compression
  int32_t m_blockSize;
  uint32_t m_minOffset;
  LZLookupTable m_lookupTable;
};
