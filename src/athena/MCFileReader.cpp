#include "athena/MCFileReader.hpp"
#include "athena/MCFile.hpp"
namespace athena {

namespace io {

static const atUint32 SCRAMBLE_VALUE = 0x5A424741;
MCFileReader::MCFileReader(atUint8* data, atUint64 length) : MemoryCopyReader(data, length) {}

MCFileReader::MCFileReader(const std::string& filename) : MemoryCopyReader(filename) {}

MCFile* MCFileReader::readFile() {
  bool isScrambled = readUint32() != SCRAMBLE_VALUE;
  m_position = 0;

  if (isScrambled)
    MCFile::unscramble(m_dataCopy.get(), m_length);

  return nullptr;
}

} // namespace io
} // namespace athena
