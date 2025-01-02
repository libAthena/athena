#include "athena/VectorWriter.hpp"

#include <algorithm>
#include <cstring>
#include <vector>

namespace athena::io {

void VectorWriter::seek(atInt64 position, SeekOrigin origin) {
  switch (origin) {
  case SeekOrigin::Begin:
    if (position < 0) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if ((atUint64)position > m_data.size())
      m_data.resize(position);

    m_position = position;
    break;

  case SeekOrigin::Current:
    if ((((atInt64)m_position + position) < 0)) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if (m_position + position > m_data.size())
      m_data.resize(m_position + position);

    m_position += position;
    break;

  case SeekOrigin::End:
    if (((atInt64)m_data.size() - position) < 0) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if ((atUint64)position > m_data.size()) {
      atError("data exceeds vector size");
      setError();
      return;
    }

    m_position = m_data.size() - position;
    break;
  }
}

void VectorWriter::writeUBytes(const atUint8* data, atUint64 length) {
  if (!data) {
    atError("data cannnot be NULL");
    setError();
    return;
  }

  if (m_position < m_data.size()) {
    size_t delta = std::min(m_data.size() - m_position, length);
    memmove(reinterpret_cast<atInt8*>(&m_data[m_position]), data, delta);
    data += delta;
    length -= delta;
    m_position += delta;
  }

  if (length != 0) {
    size_t insertPos = m_data.size();
    m_data.resize(insertPos + length);
    memmove(reinterpret_cast<atInt8*>(&m_data[insertPos]), data, length);
    m_position += length;
  }
}

} // namespace athena::io
