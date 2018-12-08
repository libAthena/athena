#include "athena/MCFileWriter.hpp"

namespace athena::io {

MCFileWriter::MCFileWriter(atUint8* data, atUint64 length) : MemoryCopyWriter(data, length) {}

MCFileWriter::MCFileWriter(const std::string& filename) : MemoryCopyWriter(filename) {}

// TODO: Check the implementation, it seems to work fine, however it's not exactly correct,
// looking at the disassembly, MC seems to do some weird checking that isn't being done with this solution
// need to figure out what it's doing and whether it's relevant to the checksum.
atUint16 MCFileWriter::calculateSlotChecksum(atUint32 game) {
  atUint16 first = calculateChecksum((m_data + 0x34 + (0x10 * game)), 4);
  atUint16 second = calculateChecksum((m_data + 0x80 + (0x500 * game)), 0x500);

  first = (first + second) & 0xFFFF;
  atUint16 result = first << 16;
  second = ~first & 0xFFFF;
  second += 1;
  result += second;

  return result;
}

atUint16 MCFileWriter::calculateChecksum(atUint8* data, atUint32 length) {
  atUint16 sum = 0;
  int i = length;

  for (atUint32 j = 0; j < length; j += 2) {
    sum += *(atUint16*)(data + j) ^ i;
    i -= 2;
  }

  sum &= 0xFFFF;

  return sum;
}

} // namespace athena::io
