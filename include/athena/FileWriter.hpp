#pragma once

#include <cstdio>

#include "athena/IStreamWriter.hpp"
#include "athena/Types.hpp"

namespace athena::io {
class FileWriter : public IStreamWriter {
public:
  explicit FileWriter(std::string_view filename, bool overwrite = true, bool globalErr = true);
  explicit FileWriter(std::wstring_view filename, bool overwrite = true, bool globalErr = true);
  ~FileWriter() override;

  std::string filename() const {
#ifdef _WIN32
    return utility::wideToUtf8(m_filename);
#else
    return m_filename;
#endif
  }
  std::wstring wfilename() const {
#ifdef _WIN32
    return m_filename;
#else
    return utility::utf8ToWide(m_filename);
#endif
  }

  void open(bool overwrite = true);
  void close();
  bool isOpen() const { return m_fileHandle != nullptr; }
  void seek(int64_t pos, SeekOrigin origin = SeekOrigin::Current) override;
  uint64_t position() const override;
  uint64_t length() const override;
  void writeUBytes(const uint8_t* data, uint64_t len) override;

#ifdef _WIN32
  using HandleType = void*;
#else
  using HandleType = FILE*;
#endif

  HandleType _fileHandle() { return m_fileHandle; }

private:
#ifdef _WIN32
  std::wstring m_filename;
#else
  std::string m_filename;
#endif
  HandleType m_fileHandle;
  bool m_globalErr;
};

class TransactionalFileWriter : public IStreamWriter {
#ifdef _WIN32
  std::wstring m_filename;
#else
  std::string m_filename;
#endif
  bool m_overwrite, m_globalErr;
  std::vector<uint8_t> m_deferredBuffer;
  uint64_t m_position = 0;

public:
  explicit TransactionalFileWriter(std::string_view filename, bool overwrite = true, bool globalErr = true)
  : m_overwrite(overwrite), m_globalErr(globalErr) {
#ifdef _WIN32
    m_filename = utility::utf8ToWide(filename);
#else
    m_filename = filename;
#endif
  }
  explicit TransactionalFileWriter(std::wstring_view filename, bool overwrite = true, bool globalErr = true)
  : m_overwrite(overwrite), m_globalErr(globalErr) {
#ifdef _WIN32
    m_filename = filename;
#else
    m_filename = utility::wideToUtf8(filename);
#endif
  }

  void flush() {
    if (m_deferredBuffer.size()) {
      FileWriter w(m_filename, m_overwrite, m_globalErr);
      w.writeUBytes(m_deferredBuffer.data(), m_deferredBuffer.size());
      cancel();
    }
  }

  void cancel() {
    m_deferredBuffer.clear();
    m_position = 0;
  }

  uint64_t position() const override { return m_position; }
  uint64_t length() const override { return m_deferredBuffer.size(); }
  void seek(int64_t pos, SeekOrigin origin = SeekOrigin::Current) override;
  void writeUBytes(const uint8_t* data, uint64_t len) override;

  ~TransactionalFileWriter() override { flush(); }
};
} // namespace athena::io
