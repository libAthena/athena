#ifndef ISTREAMREADER_HPP
#define ISTREAMREADER_HPP

#include "IStream.hpp"

namespace Athena
{
namespace io
{
class IStreamReader : public IStream
{
public:
    virtual ~IStreamReader() {}
    virtual void setEndian(Endian)    = 0;
    virtual Endian endian()      const = 0;
    virtual bool isBigEndian()   const = 0;
    virtual bool isLittleEndian()const = 0;
    virtual bool isOpen()        const = 0;
    virtual void seek(atInt64, SeekOrigin) = 0;
    virtual void seekAlign32() = 0;
    virtual bool atEnd()         const = 0;
    virtual atUint64 position()    const = 0;
    virtual atUint64 length()      const = 0;
    virtual void seekBit(int) = 0;
    virtual bool     readBit() = 0;
    virtual atUint8  readUByte() = 0;
    virtual atInt8   readByte() = 0;
    virtual atUint8* readUBytes(atUint64) = 0;
    virtual atInt8*  readBytes(atUint64) = 0;
    virtual atUint64 readUBytesToBuf(void*, atUint64) = 0;
    virtual atUint64 readBytesToBuf(void*, atUint64) = 0;
    virtual atUint16 readUint16() = 0;
    virtual atInt16  readInt16() = 0;
    virtual atUint32 readUint32() = 0;
    virtual atInt32  readInt32() = 0;
    virtual atUint64 readUint64() = 0;
    virtual atInt64  readInt64() = 0;
    virtual double   readDouble() = 0;
    virtual float    readFloat() = 0;
    virtual bool     readBool() = 0;
    virtual atVec3f  readVec3f() = 0;
    virtual atVec4f  readVec4f() = 0;
    virtual std::string readUnicode(atInt32 = -1) = 0;
    virtual std::string readString(atInt32 = -1) = 0;
    virtual std::wstring readWString(atInt32 = -1) = 0;
};
}
}
#endif // ISTREAMREADER

