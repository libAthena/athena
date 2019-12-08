#include "athena/FileWriter.hpp"

#if defined(__APPLE__) || defined(__FreeBSD__)
#include "osx_largefilewrapper.h"
#elif defined(GEKKO) || defined(__SWITCH__)
#include "gekko_support.h"
#include "osx_largefilewrapper.h"
#endif

#include <unistd.h>

namespace athena::io {
FileWriter::FileWriter(std::string_view filename, bool overwrite, bool globalErr)
: m_fileHandle(nullptr), m_globalErr(globalErr) {
#ifdef _WIN32
  m_filename = utility::utf8ToWide(filename);
#else
  m_filename = filename;
#endif
  open(overwrite);
}

FileWriter::FileWriter(std::wstring_view filename, bool overwrite, bool globalErr)
: m_fileHandle(nullptr), m_globalErr(globalErr) {
#ifdef _WIN32
  m_filename = filename;
#else
  m_filename = utility::wideToUtf8(filename);
#endif
  open(overwrite);
}

FileWriter::~FileWriter() {
  if (isOpen())
    close();
}

void FileWriter::open(bool overwrite) {
  if (overwrite) {
    std::string tmpFilename = m_filename + '~';
    m_fileHandle = fopen(tmpFilename.c_str(), "w+b");
  } else {
    m_fileHandle = fopen(m_filename.c_str(), "a+b");
    if (m_fileHandle) {
      fclose(m_fileHandle);
      m_fileHandle = fopen(m_filename.c_str(), "r+b");
    }
  }

  if (!m_fileHandle) {
    if (m_globalErr)
      atError(fmt("Unable to open file '%s'"), filename().c_str());
    setError();
    return;
  }

  // reset error
  m_hasError = false;
}

void FileWriter::close() {
  if (!m_fileHandle) {
    if (m_globalErr)
      atError(fmt("Cannot close an unopened stream"));
    setError();
    return;
  }

  fclose(m_fileHandle);
  m_fileHandle = nullptr;

  std::string tmpFilename = m_filename + '~';
#ifdef __SWITCH__
  /* Due to Horizon not being a fully POSIX compatible OS, we need to make sure the file *does not* exist before
   * attempting to rename */
  unlink(m_filename.c_str());
#endif
  rename(tmpFilename.c_str(), m_filename.c_str());
}

void FileWriter::seek(atInt64 pos, SeekOrigin origin) {
  if (!isOpen()) {
    if (m_globalErr)
      atError(fmt("Unable to seek in file, not open"));
    setError();
    return;
  }

  if (fseeko64(m_fileHandle, pos, int(origin)) != 0) {
    if (m_globalErr)
      atError(fmt("Unable to seek in file"));
    setError();
  }
}

atUint64 FileWriter::position() const { return atUint64(ftello64(m_fileHandle)); }

atUint64 FileWriter::length() const { return utility::fileSize(m_filename); }

void FileWriter::writeUBytes(const atUint8* data, atUint64 len) {
  if (!isOpen()) {
    if (m_globalErr)
      atError(fmt("File not open for writing"));
    setError();
    return;
  }

  if (fwrite(data, 1, len, m_fileHandle) != len) {
    if (m_globalErr)
      atError(fmt("Unable to write to stream"));
    setError();
  }
}
} // namespace athena::io
