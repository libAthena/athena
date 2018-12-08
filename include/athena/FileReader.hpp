#pragma once

#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <cstdio>
#endif

#include <string>
#include <memory>
#include "athena/IStreamReader.hpp"

namespace athena::io {
class FileReader : public IStreamReader {
public:
  FileReader(std::string_view filename, atInt32 cacheSize = (32 * 1024), bool globalErr = true);
  FileReader(std::wstring_view filename, atInt32 cacheSize = (32 * 1024), bool globalErr = true);
  virtual ~FileReader();

  inline std::string filename() const {
#if _WIN32
    return utility::wideToUtf8(m_filename);
#else
    return m_filename;
#endif
  }

  inline std::wstring wfilename() const {
#if _WIN32
    return m_filename;
#else
    return utility::utf8ToWide(m_filename);
#endif
  }

  void open();
  void close();
  inline bool isOpen() const { return m_fileHandle != 0; }
  bool save();
  void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
  atUint64 position() const;
  atUint64 length() const;
  atUint64 readUBytesToBuf(void* buf, atUint64 len);

  void setCacheSize(const atInt32 blockSize);

#if _WIN32
  using HandleType = HANDLE;
#else
  using HandleType = FILE*;
#endif

  HandleType _fileHandle() { return m_fileHandle; }

protected:
#if _WIN32
  std::wstring m_filename;
#else
  std::string m_filename;
#endif
  HandleType m_fileHandle;
  std::unique_ptr<atUint8[]> m_cacheData;
  atInt32 m_blockSize;
  atInt32 m_curBlock;
  atUint64 m_offset;
  bool m_globalErr;
};
} // namespace athena::io

#ifndef FILEREADER_BASE
#define FILEREADER_BASE()                                                                                              \
private:                                                                                                               \
  typedef athena::io::FileReader base

#endif // FILEREADER_BASE
