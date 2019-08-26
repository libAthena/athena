#pragma once

#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <cstdio>
#endif

#include "athena/IStreamWriter.hpp"
#include "athena/Types.hpp"

namespace athena::io {
class FileWriter : public IStreamWriter {
public:
  FileWriter(std::string_view filename, bool overwrite = true, bool globalErr = true);
  FileWriter(std::wstring_view filename, bool overwrite = true, bool globalErr = true);
  ~FileWriter() override;

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

  void open(bool overwrite = true);
  void close();
  bool isOpen() const { return m_fileHandle != 0; }
  void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current) override;
  atUint64 position() const override;
  atUint64 length() const override;
  void writeUBytes(const atUint8* data, atUint64 len) override;

#if _WIN32
  using HandleType = HANDLE;
#else
  using HandleType = FILE*;
#endif

  HandleType _fileHandle() { return m_fileHandle; }

private:
#if _WIN32
  std::wstring m_filename;
#else
  std::string m_filename;
#endif
  HandleType m_fileHandle;
  bool m_globalErr;
};

class TransactionalFileWriter : public IStreamWriter {
#if _WIN32
  std::wstring m_filename;
#else
  std::string m_filename;
#endif
  bool m_overwrite, m_globalErr;
  std::vector<uint8_t> m_deferredBuffer;
  atUint64 m_position = 0;

public:
  TransactionalFileWriter(std::string_view filename, bool overwrite = true, bool globalErr = true)
  : m_overwrite(overwrite), m_globalErr(globalErr) {
#if _WIN32
    m_filename = utility::utf8ToWide(filename);
#else
    m_filename = filename;
#endif
  }
  TransactionalFileWriter(std::wstring_view filename, bool overwrite = true, bool globalErr = true)
  : m_overwrite(overwrite), m_globalErr(globalErr) {
#if _WIN32
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

  atUint64 position() const override { return m_position; }
  atUint64 length() const override { return m_deferredBuffer.size(); }
  void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current) override;
  void writeUBytes(const atUint8* data, atUint64 len) override;

  ~TransactionalFileWriter() override { flush(); }
};
} // namespace athena::io
