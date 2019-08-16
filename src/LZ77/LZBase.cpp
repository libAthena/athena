#include "LZ77/LZLookupTable.hpp"
#include "LZ77/LZBase.hpp"

namespace {
// Returns the full length of string2 if they are equal else
// Return the number of characters that were equal before they weren't equal
int subMatch(const uint8_t* str1, const uint8_t* str2, const int len) {
  for (int i = 0; i < len; ++i)
    if (str1[i] != str2[i])
      return i;

  return len;
}

// Normally a search for one byte is matched, then two, then three, all the way up
// to the size of the LookAheadBuffer. So I decided to skip the incremental search
// and search for the entire LookAheadBuffer and if the function doesn't find the bytes are
// equal the function return the next best match(which means if the function look for 18 bytes and they are not found,
// return the number of bytes that did match before it failed to match. The submatch is function returns the number of
// bytes that were equal, which can result up to the bytes total length if both byte strings are equal.
//
//
//  ...[][][][][][][][][][][][]|[][][][][][][][][][][][][][]
//                             |
//     Search Window       Current Pos   LookAheadBuffer
//    Up to 4096 bytes                   Up to 18 bytes
//                Sliding Window
//                Up to 4114 bytes
LZLengthOffset windowSearch(const atUint8* beginSearchPtr, const atUint8* searchPosPtr, const atUint8* endLABufferPtr,
                            const atUint8* startLBPtr) {
  atInt32 size = (atUint32)(endLABufferPtr - beginSearchPtr); // Size of the entire sliding window
  atInt32 n = (atUint32)(endLABufferPtr - searchPosPtr);
  LZLengthOffset result = {0, 0};
  atInt32 temp = 0;

  if (n > size) // If the string that is being looked for is bigger than the string that is being searched
    return result;

  // This makes sure that search for the searchPosPtr can be searched if an invalid position is given
  // An invalid position occurs if the amount of characters to search in_beginSearchPtr is less than the size
  // of searchPosPtr. In other words there has to be at least n characters left in the string
  // to have a chance to find n characters

  do {
    temp = subMatch(startLBPtr, searchPosPtr, n);

    if (result.length < (atUint32)temp) {
      result.length = temp;
      result.offset = (atInt32)(searchPosPtr - startLBPtr);
    }

    if (result.length == (atUint32)n)
      return result;

    // ReadAheadBuffer is the maximum size of a character match

  } while ((startLBPtr--) > beginSearchPtr);

  return result;
}
} // Anonymous namespace

LZBase::LZBase(atInt32 minimumOffset, atInt32 slidingWindow, atInt32 minimumMatch, atInt32 blockSize)
: m_slidingWindow(slidingWindow)
, m_readAheadBuffer(minimumMatch)
, m_minMatch(minimumMatch)
, m_blockSize(blockSize)
, m_minOffset(minimumOffset) {}

void LZBase::setSlidingWindow(atInt32 slidingWindow) { m_slidingWindow = slidingWindow; }

atInt32 LZBase::slidingWindow() const { return m_slidingWindow; }

void LZBase::setReadAheadBuffer(atInt32 readAheadBuffer) { m_readAheadBuffer = readAheadBuffer; }

atInt32 LZBase::readAheadBuffer() const { return m_readAheadBuffer; }

void LZBase::setMinMatch(atInt32 minimumMatch) { m_minMatch = minimumMatch; }

atInt32 LZBase::minMatch() const { return m_minMatch; }

void LZBase::setBlockSize(atInt32 blockSize) { m_blockSize = blockSize; }

atInt32 LZBase::blockSize() const { return m_blockSize; }

void LZBase::setMinimumOffset(atUint32 minimumOffset) { m_minOffset = minimumOffset; }

atUint32 LZBase::minimumOffset() const { return m_minOffset; }

/*
  DerricMc:
  This search function is my own work and is no way affiliated with any one else
  I use the my own window_search function to drastically speed up the search function
  Normally a search for one byte is matched, then two, then three, all the way up
  to the size of the LookAheadBuffer. So I decided to skip the incremental search
  and search for the entire LookAheadBuffer and if I don't find the bytes are equal I return
  the next best match(which means if I look for 18 bytes and they are not found 18 bytes did not match,
  and 17 bytes did match then 17 bytes match is return).

*/
LZLengthOffset LZBase::search(const atUint8* posPtr, const atUint8* dataBegin, const atUint8* dataEnd) const {
  LZLengthOffset results = {0, 0};

  // Returns negative 1 for Search failures since the current position is passed the size to be compressed
  if (posPtr >= dataEnd) {
    results.length = -1;
    return results;
  }

  const atUint8* searchWindow;
  // LookAheadBuffer is ReadAheadBuffer Size if there are more bytes than ReadAheadBufferSize waiting
  // to be compressed else the number of remaining bytes is the LookAheadBuffer
  const int lookAheadBuffer_len =
      ((int)(dataEnd - posPtr) < m_readAheadBuffer) ? (int)(dataEnd - posPtr) : m_readAheadBuffer;
  const int slidingBuffer = (int)(posPtr - dataBegin) - m_slidingWindow;

  if (slidingBuffer > 0)
    searchWindow = dataBegin + slidingBuffer;
  else
    searchWindow = dataBegin;

  const atUint8* endPos = posPtr + lookAheadBuffer_len;

  if (!((posPtr - dataBegin < 1) || (dataEnd - posPtr < m_minMatch)))
    results = windowSearch(searchWindow, posPtr, endPos, posPtr - m_minOffset);

  return results;
}

