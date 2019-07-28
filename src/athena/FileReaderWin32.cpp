#include "athena/FileReader.hpp"
#include "win32_largefilewrapper.h"

namespace athena::io {
FileReader::FileReader(std::string_view filename, atInt32 cacheSize, bool globalErr)
: m_fileHandle(nullptr), m_cacheData(nullptr), m_offset(0), m_globalErr(globalErr) {
  m_filename = utility::utf8ToWide(filename);
  open();
  setCacheSize(cacheSize);
}

FileReader::FileReader(std::wstring_view filename, atInt32 cacheSize, bool globalErr)
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
      atError(fmt("File not found '{}'"), _filename);
    setError();
    return;
  }

  // reset error
  m_hasError = false;
}

void FileReader::close() {
  if (!m_fileHandle) {
    if (m_globalErr)
      atError(fmt("Cannot close an unopened stream"));
    setError();
    return;
  }

  CloseHandle(m_fileHandle);
  m_fileHandle = 0;
  return;
}

void FileReader::seek(atInt64 pos, SeekOrigin origin) {
  if (!isOpen())
    return;

  // check block position
  if (m_blockSize > 0) {
    atUint64 oldOff = m_offset;
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
        atError(fmt("Unable to seek in file"));
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
      m_curBlock = (atInt32)block;
    }
  } else {
    LARGE_INTEGER li;
    li.QuadPart = pos;
    if (!SetFilePointerEx(m_fileHandle, li, nullptr, DWORD(origin))) {
      if (m_globalErr)
        atError(fmt("Unable to seek in file"));
      setError();
    }
  }
}

atUint64 FileReader::position() const {
  if (!isOpen()) {
    if (m_globalErr)
      atError(fmt("File not open"));
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

atUint64 FileReader::length() const {
  if (!isOpen()) {
    if (m_globalErr)
      atError(fmt("File not open"));
    return 0;
  }

  LARGE_INTEGER res;
  GetFileSizeEx(m_fileHandle, &res);
  return res.QuadPart;
}

atUint64 FileReader::readUBytesToBuf(void* buf, atUint64 len) {
  if (!isOpen()) {
    if (m_globalErr)
      atError(fmt("File not open for reading"));
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
    if (m_offset >= atUint64(fs.QuadPart))
      return 0;
    if (m_offset + len >= atUint64(fs.QuadPart))
      len = fs.QuadPart - m_offset;

    size_t block = m_offset / m_blockSize;
    atUint64 cacheOffset = m_offset % m_blockSize;
    atUint64 cacheSize;
    atUint64 rem = len;
    atUint8* dst = (atUint8*)buf;

    while (rem) {
      if (block != m_curBlock) {
        LARGE_INTEGER li;
        li.QuadPart = block * m_blockSize;
        SetFilePointerEx(m_fileHandle, li, nullptr, FILE_BEGIN);
        DWORD readSz;
        ReadFile(m_fileHandle, m_cacheData.get(), m_blockSize, &readSz, nullptr);
        m_curBlock = (atInt32)block;
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
    return dst - (atUint8*)buf;
  }
}

void FileReader::setCacheSize(const atInt32 blockSize) {
  m_blockSize = blockSize;

  if (m_blockSize > length())
    m_blockSize = (atInt32)length();

  m_curBlock = -1;
  if (m_blockSize > 0)
    m_cacheData.reset(new atUint8[m_blockSize]);
}

} // namespace athena::io
