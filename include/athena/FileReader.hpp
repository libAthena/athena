#pragma once

#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <cstdio>
#endif

#include <memory>
#include <string>

#include "athena/IStreamReader.hpp"
#include "athena/Types.hpp"

namespace athena::io {
class FileReader : public IStreamReader {
public:
  explicit FileReader(std::string_view filename, atInt32 cacheSize = (32 * 1024), bool globalErr = true);
  explicit FileReader(std::wstring_view filename, atInt32 cacheSize = (32 * 1024), bool globalErr = true);
  ~FileReader() override;

  std::string filename() const {
#if _WIN32
    return utility::wideToUtf8(m_filename);
#else
    return m_filename;
#endif
  }

  std::wstring wfilename() const {
#if _WIN32
    return m_filename;
#else
    return utility::utf8ToWide(m_filename);
#endif
  }

  void open();
  void close();
  bool isOpen() const { return m_fileHandle != 0; }
  bool save();
  void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current) override;
  atUint64 position() const override;
  atUint64 length() const override;
  atUint64 readUBytesToBuf(void* buf, atUint64 len) override;

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
