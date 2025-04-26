#include "athena/FileReader.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "win32_largefilewrapper.h"

namespace athena::io {
FileReader::FileReader(std::string_view filename, int32_t cacheSize, bool globalErr)
: m_fileHandle(nullptr), m_cacheData(nullptr), m_offset(0), m_globalErr(globalErr) {
  m_filename = utility::utf8ToWide(filename);
  open();
  setCacheSize(cacheSize);
}

FileReader::FileReader(std::wstring_view filename, int32_t cacheSize, bool globalErr)
: m_fileHandle(nullptr), m_cacheData(nullptr), m_offset(0), m_globalErr(globalErr) {
  m_filename = filename;
  open();
  setCacheSize(cacheSize);
}

FileReader::~FileReader() {
  if (isOpen())
    close();
}

void FileReader::open() {
  int attempt = 0;
  do {
#if WINDOWS_STORE
    m_fileHandle = CreateFile2(m_filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr);
#else
    m_fileHandle = CreateFileW(m_filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL, nullptr);
#endif
  } while (m_fileHandle == INVALID_HANDLE_VALUE && attempt++ < 100);

  if (m_fileHandle == INVALID_HANDLE_VALUE) {
    m_fileHandle = 0;
    std::string _filename = filename();
    if (m_globalErr)
      atError("File not found '{}'", _filename);
    setError();
    return;
  }

  // reset error
  m_hasError = false;
}

void FileReader::close() {
  if (!m_fileHandle) {
    if (m_globalErr)
      atError("Cannot close an unopened stream");
    setError();
    return;
  }

  CloseHandle(m_fileHandle);
  m_fileHandle = 0;
  return;
}

void FileReader::seek(int64_t pos, SeekOrigin origin) {
  if (!isOpen())
    return;

  // check block position
  if (m_blockSize > 0) {
    uint64_t oldOff = m_offset;
    switch (origin) {
    case SeekOrigin::Begin:
      m_offset = pos;
      break;
    case SeekOrigin::Current:
      m_offset += pos;
      break;
    case SeekOrigin::End:
      m_offset = length() - pos;
      break;
    }
    if (m_offset > length()) {
      oldOff = m_offset;
      if (m_globalErr)
        atError("Unable to seek in file");
      setError();
      return;
    }

    size_t block = m_offset / m_blockSize;
    if (block != m_curBlock) {
      LARGE_INTEGER li;
      li.QuadPart = block * m_blockSize;
      SetFilePointerEx(m_fileHandle, li, nullptr, FILE_BEGIN);
      DWORD readSz;
      ReadFile(m_fileHandle, m_cacheData.get(), m_blockSize, &readSz, nullptr);
      m_curBlock = (int32_t)block;
    }
  } else {
    LARGE_INTEGER li;
    li.QuadPart = pos;
    if (!SetFilePointerEx(m_fileHandle, li, nullptr, DWORD(origin))) {
      if (m_globalErr)
        atError("Unable to seek in file");
      setError();
    }
  }
}

uint64_t FileReader::position() const {
  if (!isOpen()) {
    if (m_globalErr)
      atError("File not open");
    return 0;
  }

  if (m_blockSize > 0)
    return m_offset;
  else {
    LARGE_INTEGER li = {};
    LARGE_INTEGER res;
    SetFilePointerEx(m_fileHandle, li, &res, FILE_CURRENT);
    return res.QuadPart;
  }
}

uint64_t FileReader::length() const {
  if (!isOpen()) {
    if (m_globalErr)
      atError("File not open");
    return 0;
  }

  LARGE_INTEGER res;
  GetFileSizeEx(m_fileHandle, &res);
  return res.QuadPart;
}

uint64_t FileReader::readUBytesToBuf(void* buf, uint64_t len) {
  if (!isOpen()) {
    if (m_globalErr)
      atError("File not open for reading");
    setError();
    return 0;
  }

  if (m_blockSize <= 0) {
    DWORD ret = 0;
    ReadFile(m_fileHandle, buf, len, &ret, nullptr);
    return ret;
  } else {
    LARGE_INTEGER fs;
    GetFileSizeEx(m_fileHandle, &fs);
    if (m_offset >= uint64_t(fs.QuadPart))
      return 0;
    if (m_offset + len >= uint64_t(fs.QuadPart))
      len = fs.QuadPart - m_offset;

    size_t block = m_offset / m_blockSize;
    uint64_t cacheOffset = m_offset % m_blockSize;
    uint64_t cacheSize;
    uint64_t rem = len;
    uint8_t* dst = (uint8_t*)buf;

    while (rem) {
      if (block != m_curBlock) {
        LARGE_INTEGER li;
        li.QuadPart = block * m_blockSize;
        SetFilePointerEx(m_fileHandle, li, nullptr, FILE_BEGIN);
        DWORD readSz;
        ReadFile(m_fileHandle, m_cacheData.get(), m_blockSize, &readSz, nullptr);
        m_curBlock = (int32_t)block;
      }

      cacheSize = rem;
      if (cacheSize + cacheOffset > m_blockSize)
        cacheSize = m_blockSize - cacheOffset;

      memmove(dst, m_cacheData.get() + cacheOffset, cacheSize);
      dst += cacheSize;
      rem -= cacheSize;
      cacheOffset = 0;
      ++block;
    }
    m_offset += len;
    return dst - (uint8_t*)buf;
  }
}

void FileReader::setCacheSize(const int32_t blockSize) {
  m_blockSize = blockSize;

  if (m_blockSize > length())
    m_blockSize = (int32_t)length();

  m_curBlock = -1;
  if (m_blockSize > 0)
    m_cacheData.reset(new uint8_t[m_blockSize]);
}

} // namespace athena::io
