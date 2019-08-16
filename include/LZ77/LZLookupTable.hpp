#pragma once

#include <map>
#include <deque>
#include <vector>
#include <cstdint>
#include <athena/Types.hpp>

struct LZLengthOffset {
  atUint32 length; // The number of bytes compressed
  atUint16 offset; // How far back in sliding window where bytes that match the lookAheadBuffer is located
  bool operator==(const LZLengthOffset& lo_pair) const { return length == lo_pair.length && offset == lo_pair.offset; }
  bool operator!=(const LZLengthOffset& lo_pair) const { return !operator==(lo_pair); }
};

class LZLookupTable {
public:
  LZLookupTable();
  LZLookupTable(atInt32 minimumMatch, atInt32 slidingWindow = 4096, atInt32 lookAheadWindow = 18);
  ~LZLookupTable();
  LZLengthOffset search(const atUint8* curPos, const atUint8* dataBegin, const atUint8* dataEnd);
  void setLookAheadWindow(atInt32 lookAheadWindow);

private:
  typedef std::multimap<std::vector<uint8_t>, int32_t> LookupTable;
  LookupTable table;
  atInt32 m_minimumMatch;
  atInt32 m_slidingWindow;
  atInt32 m_lookAheadWindow;
  std::vector<uint8_t> m_buffer;
};
