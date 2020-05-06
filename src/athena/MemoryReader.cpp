#include "athena/MemoryReader.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>

#ifdef HW_RVL
#include <malloc.h>
#endif // HW_RVL

namespace athena::io {
MemoryReader::MemoryReader(const void* data, atUint64 length, bool takeOwnership, bool globalErr)
: m_data(data), m_length(length), m_position(0), m_owns(takeOwnership), m_globalErr(globalErr) {
  if (!data) {
    if (m_globalErr)
      atError(FMT_STRING("data cannot be NULL"));
    setError();
    return;
  }
}

MemoryReader::~MemoryReader() {
  if (m_owns)
    delete[] reinterpret_cast<const atUint8*>(m_data);
}

MemoryCopyReader::MemoryCopyReader(const void* data, atUint64 length) : MemoryReader(data, length, false) {
  if (!data) {
    if (m_globalErr)
      atError(FMT_STRING("data cannot be NULL"));
    setError();
    return;
  }

  m_dataCopy.reset(new atUint8[m_length]);
  m_data = m_dataCopy.get();
  memmove(m_dataCopy.get(), data, m_length);
}

void MemoryReader::seek(atInt64 position, SeekOrigin origin) {
  switch (origin) {
  case SeekOrigin::Begin:
    if ((position < 0 || atInt64(position) > atInt64(m_length))) {
      if (m_globalErr)
        atFatal(FMT_STRING("Position {:08X} outside stream bounds "), position);
      m_position = m_length;
      setError();
      return;
    }

    m_position = atUint64(position);
    break;

  case SeekOrigin::Current:
    if (((atInt64(m_position) + position) < 0 || (m_position + atUint64(position)) > m_length)) {
      if (m_globalErr)
        atFatal(FMT_STRING("Position {:08X} outside stream bounds "), position);
      m_position = (position < 0 ? 0 : m_length);
      setError();
      return;
    }

    m_position += position;
    break;

  case SeekOrigin::End:
    if ((((atInt64)m_length - position < 0) || (m_length - position) > m_length)) {
      if (m_globalErr)
        atFatal(FMT_STRING("Position {:08X} outside stream bounds "), position);
      m_position = m_length;
      setError();
      return;
    }

    m_position = m_length - position;
    break;
  }
}

void MemoryReader::setData(const atUint8* data, atUint64 length, bool takeOwnership) {
  if (m_owns)
    delete[] static_cast<const atUint8*>(m_data);
  m_data = (atUint8*)data;
  m_length = length;
  m_position = 0;
  m_owns = takeOwnership;
}

void MemoryCopyReader::setData(const atUint8* data, atUint64 length) {
  m_dataCopy.reset(new atUint8[length]);
  m_data = m_dataCopy.get();
  memmove(m_dataCopy.get(), data, length);
  m_length = length;
  m_position = 0;
}

atUint8* MemoryReader::data() const {
  atUint8* ret = new atUint8[m_length];
  memset(ret, 0, m_length);
  memmove(ret, m_data, m_length);
  return ret;
}

atUint64 MemoryReader::readUBytesToBuf(void* buf, atUint64 length) {
  if (m_position >= m_length) {
    if (m_globalErr)
      atError(FMT_STRING("Position {:08X} outside stream bounds "), m_position);
    m_position = m_length;
    setError();
    return 0;
  }

  length = std::min(length, m_length - m_position);
  memmove(buf, reinterpret_cast<const atUint8*>(m_data) + m_position, length);
  m_position += length;
  return length;
}

void MemoryCopyReader::loadData() {
  FILE* in;
  atUint64 length;
  in = fopen(m_filepath.c_str(), "rb");

  if (!in) {
    if (m_globalErr)
      atError(FMT_STRING("Unable to open file '%s'"), m_filepath);
    setError();
    return;
  }

  rewind(in);

  length = utility::fileSize(m_filepath);
  m_dataCopy.reset(new atUint8[length]);
  m_data = m_dataCopy.get();

  atUint64 done = 0;
  atUint64 blocksize = BLOCKSZ;

  do {
    if (blocksize > length - done)
      blocksize = length - done;

    atInt64 ret = fread(m_dataCopy.get() + done, 1, blocksize, in);

    if (ret < 0) {
      if (m_globalErr)
        atError(FMT_STRING("Error reading data from disk"));
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
