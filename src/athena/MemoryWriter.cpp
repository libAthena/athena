#include "athena/MemoryWriter.hpp"

#include <cstdio>
#include <cstring>

#ifdef HW_RVL
#include <malloc.h>
#endif // HW_RVL

namespace athena::io {

MemoryWriter::MemoryWriter(uint8_t* data, uint64_t length, bool takeOwnership)
: m_data(data), m_length(length), m_bufferOwned(takeOwnership) {
  if (!data) {
    atError("data cannot be NULL");
    setError();
    return;
  }
}

MemoryWriter::~MemoryWriter() {
  if (m_bufferOwned)
    delete m_data;
  m_data = nullptr;
  m_length = 0;
}

MemoryCopyWriter::MemoryCopyWriter(uint8_t* data, uint64_t length) {
  m_data = data;
  m_length = length;
  m_position = 0;
  m_bufferOwned = false;

  if (length == 0) {
    atError("length cannot be 0");
    setError();
    return;
  }
  m_dataCopy.reset(new uint8_t[length]);
  m_data = m_dataCopy.get();
  if (data)
    memmove(m_data, data, length);
}

MemoryCopyWriter::MemoryCopyWriter(std::string_view filename) {
  m_filepath = filename;
  m_length = 0x10;
  m_position = 0;
  m_dataCopy.reset(new uint8_t[m_length]);
  m_data = m_dataCopy.get();
  m_bufferOwned = false;

  if (!m_data) {
    atError("Could not allocate memory!");
    setError();
    return;
  }
}

void MemoryWriter::seek(int64_t position, SeekOrigin origin) {
  switch (origin) {
  case SeekOrigin::Begin:
    if (position < 0) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if ((uint64_t)position > m_length) {
      atError("data exceeds available buffer space");
      setError();
      return;
    }

    m_position = position;
    break;

  case SeekOrigin::Current:
    if ((((int64_t)m_position + position) < 0)) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if (m_position + position > m_length) {
      atError("data exceeds available buffer space");
      setError();
      return;
    }

    m_position += position;
    break;

  case SeekOrigin::End:
    if (((int64_t)m_length - position) < 0) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if ((uint64_t)position > m_length) {
      atError("data exceeds available buffer space");
      setError();
      return;
    }

    m_position = m_length - position;
    break;
  }
}

void MemoryCopyWriter::seek(int64_t position, SeekOrigin origin) {
  switch (origin) {
  case SeekOrigin::Begin:
    if (position < 0) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if ((uint64_t)position > m_length)
      resize(position);

    m_position = position;
    break;

  case SeekOrigin::Current:
    if ((((int64_t)m_position + position) < 0)) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if (m_position + position > m_length)
      resize(m_position + position);

    m_position += position;
    break;

  case SeekOrigin::End:
    if (((int64_t)m_length - position) < 0) {
      atError("Position outside stream bounds");
      setError();
      return;
    }

    if ((uint64_t)position > m_length)
      resize(position);

    m_position = m_length - position;
    break;
  }
}

void MemoryWriter::setData(uint8_t* data, uint64_t length, bool takeOwnership) {
  if (m_bufferOwned)
    delete m_data;

  m_data = data;
  m_length = length;
  m_position = 0;
  m_bufferOwned = takeOwnership;
}

void MemoryCopyWriter::setData(const uint8_t* data, uint64_t length) {
  m_dataCopy.reset(new uint8_t[length]);
  m_data = m_dataCopy.get();
  memmove(m_data, data, length);
  m_length = length;
  m_position = 0;
  m_bufferOwned = false;
}

uint8_t* MemoryWriter::data() const {
  uint8_t* ret = new uint8_t[m_length];
  memset(ret, 0, m_length);
  memmove(ret, m_data, m_length);
  return ret;
}

void MemoryWriter::save(std::string_view filename) {
  if (filename.empty() && m_filepath.empty()) {
    atError("No file specified, cannot save.");
    setError();
    return;
  }

  if (!filename.empty()) {
    m_filepath = filename;
  }

  auto deleter = [](FILE* f) { return std::fclose(f); };
  std::unique_ptr<FILE, decltype(deleter)> out{std::fopen(m_filepath.c_str(), "wb"), deleter};
  if (!out) {
    atError("Unable to open file '{}'", m_filepath);
    setError();
    return;
  }

  uint64_t done = 0;
  uint64_t blocksize = BLOCKSZ;

  do {
    if (blocksize > m_length - done) {
      blocksize = m_length - done;
    }

    const int64_t ret = std::fwrite(m_data + done, 1, blocksize, out.get());

    if (ret < 0) {
      atError("Error writing data to disk");
      setError();
      return;
    }

    if (ret == 0) {
      break;
    }

    done += blocksize;
  } while (done < m_length);
}

void MemoryWriter::writeUBytes(const uint8_t* data, uint64_t length) {
  if (!data) {
    atError("data cannnot be NULL");
    setError();
    return;
  }

  if (m_position + length > m_length) {
    atError("data length exceeds available buffer space");
    setError();
    return;
  }

  memmove(m_data + m_position, data, length);

  m_position += length;
}

void MemoryCopyWriter::writeUBytes(const uint8_t* data, uint64_t length) {
  if (!data) {
    atError("data cannnot be NULL");
    setError();
    return;
  }

  if (m_position + length > m_length)
    resize(m_position + length);

  memmove(m_data + m_position, data, length);

  m_position += length;
}

void MemoryCopyWriter::resize(uint64_t newSize) {
  if (newSize < m_length) {
    atError("New size cannot be less to the old size.");
    return;
  }

  // Allocate and copy new buffer
  auto newArray = std::make_unique<uint8_t[]>(newSize);
  if (m_dataCopy) {
    std::memmove(newArray.get(), m_dataCopy.get(), m_length);
  }
  m_dataCopy = std::move(newArray);

  // Swap the pointer and size out for the new ones.
  m_data = m_dataCopy.get();
  m_length = newSize;
}

} // namespace athena::io
