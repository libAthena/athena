#ifndef ISTREAMWRITER_HPP
#define ISTREAMWRITER_HPP

#include "IStream.hpp"

namespace Athena
{
namespace io
{
class IStreamWriter : public IStream
{
public:
    virtual ~IStreamWriter() {}
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
    virtual void writeBit(bool) = 0;
    virtual void writeUByte(atUint8) = 0;
    virtual void writeByte(atInt8) = 0;
    virtual void writeUBytes(const atUint8*, atUint64) = 0;
    virtual void writeBytes(const atInt8*, atUint64) = 0;
    virtual void writeUint16(atUint16) = 0;
    virtual void writeInt16(atInt16) = 0;
    virtual void writeUint32(atUint32) = 0;
    virtual void writeInt32(atInt32) = 0;
    virtual void writeUint64(atUint64) = 0;
    virtual void writeInt64(atInt64) = 0;
    virtual void writeDouble(double) = 0;
    virtual void writeFloat(float) = 0;
    virtual void writeBool(bool) = 0;
    virtual void writeVec3f(atVec3f vec) = 0;
    virtual void writeVec4f(atVec4f vec) = 0;
    virtual void writeString(const std::string&, atInt32 = -1) = 0;
    virtual void writeWString(const std::wstring&, atInt32 = -1) = 0;
    virtual void writeUnicode(const std::string&, atInt32 = -1) = 0;
    virtual void fill(atUint8, atUint64) = 0;
    virtual void fill(atInt8, atUint64) = 0;
};
}
}
#endif // STREAMWRITER_HPP

