#include "athena/MCFileWriter.hpp"

namespace athena::io {

MCFileWriter::MCFileWriter(uint8_t* data, uint64_t length) : MemoryCopyWriter(data, length) {}

MCFileWriter::MCFileWriter(const std::string& filename) : MemoryCopyWriter(filename) {}

// TODO: Check the implementation, it seems to work fine, however it's not exactly correct,
// looking at the disassembly, MC seems to do some weird checking that isn't being done with this solution
// need to figure out what it's doing and whether it's relevant to the checksum.
uint16_t MCFileWriter::calculateSlotChecksum(uint32_t game) {
  uint16_t first = calculateChecksum((m_data + 0x34 + (0x10 * game)), 4);
  uint16_t second = calculateChecksum((m_data + 0x80 + (0x500 * game)), 0x500);

  first = (first + second) & 0xFFFF;
  uint16_t result = first << 16;
  second = ~first & 0xFFFF;
  second += 1;
  result += second;

  return result;
}

uint16_t MCFileWriter::calculateChecksum(uint8_t* data, uint32_t length) {
  uint16_t sum = 0;
  int i = length;

  for (uint32_t j = 0; j < length; j += 2) {
    sum += *(uint16_t*)(data + j) ^ i;
    i -= 2;
  }

  sum &= 0xFFFF;

  return sum;
}

} // namespace athena::io
