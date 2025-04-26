#include "athena/FileWriter.hpp"

#include <cstring>

namespace athena::io {
void TransactionalFileWriter::seek(int64_t pos, SeekOrigin origin) {
  switch (origin) {
  case SeekOrigin::Begin:
    m_position = pos;
    break;
  case SeekOrigin::Current:
    m_position += pos;
    break;
  case SeekOrigin::End:
    break;
  }
}

void TransactionalFileWriter::writeUBytes(const uint8_t* data, uint64_t len) {
  uint64_t neededSz = m_position + len;
  if (neededSz > m_deferredBuffer.size()) {
    m_deferredBuffer.reserve(neededSz * 2);
    m_deferredBuffer.resize(neededSz);
  }

  memmove(m_deferredBuffer.data() + m_position, data, len);
  m_position += len;
}
} // namespace athena::io
