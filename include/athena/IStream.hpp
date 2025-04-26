#pragma once

#include "athena/Global.hpp"

namespace athena::io {

class IStream {
public:
  virtual ~IStream() = default;

  void setEndian(Endian endian) { m_endian = endian; }
  Endian endian() const { return m_endian; }
  bool isBigEndian() const { return (m_endian == Endian::Big); }
  bool isLittleEndian() const { return (m_endian == Endian::Little); }
  virtual void seek(int64_t, SeekOrigin) = 0;
  virtual bool atEnd() const = 0;
  virtual uint64_t position() const = 0;
  virtual uint64_t length() const = 0;
  bool hasError() const { return m_hasError; }

protected:
  void setError() { m_hasError = true; }
  bool m_hasError = false;
#if __BYTE_ORDER == __BIG_ENDIAN
  Endian m_endian = Endian::Big;
#else
  Endian m_endian = Endian::Little;
#endif
};
} // namespace athena::io
