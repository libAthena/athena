#include "athena/MemoryReader.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>

#ifdef HW_RVL
#include <malloc.h>
#endif // HW_RVL

namespace athena::io {
MemoryReader::MemoryReader(const void* data, uint64_t length, bool takeOwnership, bool globalErr)
: m_data(data), m_length(length), m_position(0), m_owns(takeOwnership), m_globalErr(globalErr) {
  if (!data) {
    if (m_globalErr)
      atError("data cannot be NULL");
    setError();
    return;
  }
}

MemoryReader::~MemoryReader() {
  if (m_owns)
    delete[] static_cast<const uint8_t*>(m_data);
}

MemoryCopyReader::MemoryCopyReader(const void* data, uint64_t length) : MemoryReader(data, length, false) {
  if (!data) {
    if (m_globalErr)
      atError("data cannot be NULL");
    setError();
    return;
  }

  m_dataCopy.reset(new uint8_t[m_length]);
  m_data = m_dataCopy.get();
  memmove(m_dataCopy.get(), data, m_length);
}

void MemoryReader::seek(int64_t position, SeekOrigin origin) {
  switch (origin) {
  case SeekOrigin::Begin:
    if ((position < 0 || int64_t(position) > int64_t(m_length))) {
      if (m_globalErr)
        atFatal("Position {:08X} outside stream bounds ", position);
      m_position = m_length;
      setError();
      return;
    }

    m_position = uint64_t(position);
    break;

  case SeekOrigin::Current:
    if (((int64_t(m_position) + position) < 0 || (m_position + uint64_t(position)) > m_length)) {
      if (m_globalErr)
        atFatal("Position {:08X} outside stream bounds ", position);
      m_position = (position < 0 ? 0 : m_length);
      setError();
      return;
    }

    m_position += position;
    break;

  case SeekOrigin::End:
    if ((((int64_t)m_length - position < 0) || (m_length - position) > m_length)) {
      if (m_globalErr)
        atFatal("Position {:08X} outside stream bounds ", position);
      m_position = m_length;
      setError();
      return;
    }

    m_position = m_length - position;
    break;
  }
}

void MemoryReader::setData(const uint8_t* data, uint64_t length, bool takeOwnership) {
  if (m_owns)
    delete[] static_cast<const uint8_t*>(m_data);
  m_data = (uint8_t*)data;
  m_length = length;
  m_position = 0;
  m_owns = takeOwnership;
}

void MemoryCopyReader::setData(const uint8_t* data, uint64_t length) {
  m_dataCopy.reset(new uint8_t[length]);
  m_data = m_dataCopy.get();
  memmove(m_dataCopy.get(), data, length);
  m_length = length;
  m_position = 0;
}

uint8_t* MemoryReader::data() const {
  uint8_t* ret = new uint8_t[m_length];
  memset(ret, 0, m_length);
  memmove(ret, m_data, m_length);
  return ret;
}

uint64_t MemoryReader::readUBytesToBuf(void* buf, uint64_t length) {
  if (m_position >= m_length) {
    if (m_globalErr)
      atError("Position {:08X} outside stream bounds ", m_position);
    m_position = m_length;
    setError();
    return 0;
  }

  length = std::min(length, m_length - m_position);
  memmove(buf, static_cast<const uint8_t*>(m_data) + m_position, length);
  m_position += length;
  return length;
}

void MemoryCopyReader::loadData() {
  FILE* in;
  uint64_t length;
  in = fopen(m_filepath.c_str(), "rb");

  if (!in) {
    if (m_globalErr)
      atError("Unable to open file '%s'", m_filepath);
    setError();
    return;
  }

  rewind(in);

  length = utility::fileSize(m_filepath);
  m_dataCopy.reset(new uint8_t[length]);
  m_data = m_dataCopy.get();

  uint64_t done = 0;
  uint64_t blocksize = BLOCKSZ;

  do {
    if (blocksize > length - done)
      blocksize = length - done;

    int64_t ret = fread(m_dataCopy.get() + done, 1, blocksize, in);

    if (ret < 0) {
      if (m_globalErr)
        atError("Error reading data from disk");
      setError();
      return;
    } else if (ret == 0)
      break;

    done += ret;

  } while (done < length);

  fclose(in);
  m_length = length;
  m_position = 0;
}

} // namespace athena::io
