#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <athena/Types.hpp>

struct LZLengthOffset {
  uint32_t length; // The number of bytes compressed
  uint16_t offset; // How far back in sliding window where bytes that match the lookAheadBuffer is located
  bool operator==(const LZLengthOffset& lo_pair) const { return length == lo_pair.length && offset == lo_pair.offset; }
  bool operator!=(const LZLengthOffset& lo_pair) const { return !operator==(lo_pair); }
};

class LZLookupTable {
public:
  LZLookupTable();
  explicit LZLookupTable(int32_t minimumMatch, int32_t slidingWindow = 4096, int32_t lookAheadWindow = 18);
  ~LZLookupTable();
  LZLengthOffset search(const uint8_t* curPos, const uint8_t* dataBegin, const uint8_t* dataEnd);
  void setLookAheadWindow(int32_t lookAheadWindow);

private:
  using LookupTable = std::multimap<std::vector<uint8_t>, int32_t>;
  LookupTable table;
  int32_t m_minimumMatch = 3;
  int32_t m_slidingWindow = 4096;
  int32_t m_lookAheadWindow = 18;
  std::vector<uint8_t> m_buffer;
};
