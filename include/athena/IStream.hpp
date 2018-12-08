#pragma once

#include "Global.hpp"

namespace athena::io {
std::ostream& operator<<(std::ostream& os, Endian& endian);

class IStream {
public:
  virtual ~IStream() = default;

  void setEndian(Endian endian) { m_endian = endian; }
  Endian endian() const { return m_endian; }
  bool isBigEndian() const { return (m_endian == Endian::Big); }
  bool isLittleEndian() const { return (m_endian == Endian::Little); }
  virtual void seek(atInt64, SeekOrigin) = 0;
  virtual bool atEnd() const = 0;
  virtual atUint64 position() const = 0;
  virtual atUint64 length() const = 0;
  bool hasError() const { return m_hasError; }

protected:
  void setError() { m_hasError = true; }
  bool m_hasError = false;
#if __BYTE_ORDER == __BIG_ENDIAN
  Endian m_endian = Big;
#else
  Endian m_endian = Little;
#endif
};
} // namespace athena::io
