#ifndef STREAM_HPP
#define STREAM_HPP

#include "Global.hpp"

namespace athena
{
namespace io
{
std::ostream& operator<<(std::ostream& os, Endian& endian);

class IStream
{
public:
    IStream() {}
    virtual ~IStream() {}

    inline void setEndian(Endian endian) { m_endian = endian; }
    inline Endian endian() const { return m_endian; }
    inline bool isBigEndian() const { return (m_endian == Endian::BigEndian); }
    inline bool isLittleEndian() const { return (m_endian == Endian::LittleEndian); }
    virtual void seek(atInt64, SeekOrigin) = 0;
    virtual bool atEnd() const = 0;
    virtual atUint64 position() const = 0;
    virtual atUint64 length() const = 0;
    bool hasError() const { return m_hasError; }
protected:
    void setError() { m_hasError = true; }
    bool m_hasError = false;
#if __BYTE_ORDER == __BIG_ENDIAN
    Endian m_endian = BigEndian;
#else
    Endian m_endian = LittleEndian;
#endif
};
}
}
#endif // STREAM_HPP
