#ifndef STREAM_HPP
#define STREAM_HPP

#include "Global.hpp"
#include "Athena/NotImplementedException.hpp"

namespace Athena
{
namespace io
{
std::ostream& operator<<(std::ostream& os, Endian& endian);

class IStream
{
public:
    virtual ~IStream() {}

    virtual void setEndian(Endian)    = 0;
    virtual Endian endian()      const = 0;
    virtual bool isBigEndian()   const = 0;
    virtual bool isLittleEndian()const = 0;
    virtual bool isOpen()        const = 0;
    virtual void seek(atInt64, SeekOrigin) = 0;
    virtual bool atEnd()         const = 0;
    virtual atUint64 position()    const = 0;
    virtual atUint64 length()      const = 0;
    virtual void seekBit(int) = 0;
};
}
}
#endif // STREAM_HPP
