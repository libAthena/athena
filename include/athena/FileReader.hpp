#pragma once

#include <cstdio>
#include <memory>
#include <string>

#include "athena/IStreamReader.hpp"
#include "athena/Types.hpp"

namespace athena::io {
class FileReader : public IStreamReader {
public:
  explicit FileReader(std::string_view filename, int32_t cacheSize = (32 * 1024), bool globalErr = true);
  explicit FileReader(std::wstring_view filename, int32_t cacheSize = (32 * 1024), bool globalErr = true);
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
  bool isOpen() const { return m_fileHandle != nullptr; }
  void seek(int64_t pos, SeekOrigin origin = SeekOrigin::Current) override;
  uint64_t position() const override;
  uint64_t length() const override;
  uint64_t readUBytesToBuf(void* buf, uint64_t len) override;

  void setCacheSize(const int32_t blockSize);

#if _WIN32
  using HandleType = void*;
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
  uint64_t m_fileSize;
  std::unique_ptr<uint8_t[]> m_cacheData;
  int32_t m_blockSize;
  int32_t m_curBlock;
  uint64_t m_offset;
  bool m_globalErr;
};
} // namespace athena::io
