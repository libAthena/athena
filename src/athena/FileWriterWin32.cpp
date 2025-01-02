#include "athena/FileWriter.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <algorithm>
#include <limits>

namespace athena::io {
FileWriter::FileWriter(std::string_view filename, bool overwrite, bool globalErr)
: m_fileHandle(0), m_globalErr(globalErr) {
  m_filename = utility::utf8ToWide(filename);
  open(overwrite);
}

FileWriter::FileWriter(std::wstring_view filename, bool overwrite, bool globalErr)
: m_fileHandle(0), m_globalErr(globalErr) {
  m_filename = filename;
  open(overwrite);
}

FileWriter::~FileWriter() {
  if (isOpen())
    close();
}

void FileWriter::open(bool overwrite) {
  int attempt = 0;
  do {
    if (overwrite) {
      std::wstring tmpFilename = m_filename + L'~';
#if WINDOWS_STORE
      m_fileHandle = CreateFile2(tmpFilename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS, nullptr);
#else
      m_fileHandle = CreateFileW(tmpFilename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
                                 FILE_ATTRIBUTE_NORMAL, nullptr);
#endif
    } else {
#if WINDOWS_STORE
      m_fileHandle = CreateFile2(m_filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_ALWAYS, nullptr);
#else
      m_fileHandle = CreateFileW(m_filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
                                 FILE_ATTRIBUTE_NORMAL, nullptr);
#endif
    }
  } while (m_fileHandle == INVALID_HANDLE_VALUE && attempt++ < 100);

  if (m_fileHandle == INVALID_HANDLE_VALUE) {
    m_fileHandle = 0;
    if (m_globalErr) {
      auto _filename = filename();
      atError("Unable to open file '{}'", _filename);
    }
    setError();
    return;
  }

  // reset error
  m_hasError = false;
}

void FileWriter::close() {
  if (!m_fileHandle) {
    if (m_globalErr)
      atError("Cannot close an unopened stream");
    setError();
    return;
  }

  FlushFileBuffers(m_fileHandle);
  CloseHandle(m_fileHandle);
  m_fileHandle = 0;

  std::wstring tmpFilename = m_filename + L'~';
  MoveFileExW(tmpFilename.c_str(), m_filename.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
}

void FileWriter::seek(atInt64 pos, SeekOrigin origin) {
  if (!isOpen()) {
    if (m_globalErr)
      atError("Unable to seek in file, not open");
    setError();
    return;
  }

  LARGE_INTEGER li;
  li.QuadPart = pos;
  if (!SetFilePointerEx(m_fileHandle, li, nullptr, DWORD(origin))) {
    if (m_globalErr)
      atError("Unable to seek in file");
    setError();
  }
}

atUint64 FileWriter::position() const {
  LARGE_INTEGER li = {};
  LARGE_INTEGER res;
  SetFilePointerEx(m_fileHandle, li, &res, FILE_CURRENT);
  return static_cast<atUint64>(res.QuadPart);
}

atUint64 FileWriter::length() const { return utility::fileSize(m_filename); }

void FileWriter::writeUBytes(const atUint8* data, atUint64 len) {
  if (!isOpen()) {
    if (m_globalErr) {
      atError("File not open for writing");
    }
    setError();
    return;
  }

  atUint64 remaining = len;
  do {
    const auto toWrite = static_cast<DWORD>(std::min(remaining, atUint64{std::numeric_limits<DWORD>::max()}));
    DWORD written = 0;

    if (WriteFile(m_fileHandle, data, toWrite, &written, nullptr) == FALSE) {
      if (m_globalErr) {
        atError("Unable to write to file");
      }
      setError();
      return;
    }

    remaining -= written;
    data += written;
  } while (remaining != 0);
}

} // namespace athena::io
