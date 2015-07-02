#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#include "Athena/IStreamWriter.hpp"
#include <stdio.h>

namespace Athena
{
namespace io
{
class FileWriter : public IStreamWriter
{
public:
    FileWriter(const std::string& filename, bool overwrite = true);
    virtual ~FileWriter();

    inline void setEndian(Endian endian)
    {m_endian = endian;}
    inline Endian endian() const
    {return m_endian;}
    inline bool isBigEndian() const
    {return (m_endian == Endian::BigEndian);}
    inline bool isLittleEndian() const
    {return (m_endian == Endian::LittleEndian);}

    void open(bool overwrite = true);
    void close();
    inline bool isOpen() const
    {return m_fileHandle != NULL;}
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
    inline void seekAlign32() {seek(ROUND_UP_32(position()), SeekOrigin::Begin);}
    bool atEnd() const;
    atUint64 position() const;
    atUint64 length() const;

    void writeBit(bool val);
    void seekBit(int bit);
    void writeUByte(atUint8 val);
    inline void writeByte(atInt8 val)
    {FileWriter::writeUByte(val);}
    void writeUBytes(const atUint8* data, atUint64 len);
    void writeBytes(const atInt8* data, atUint64 len)
    {FileWriter::writeUBytes((atUint8*)data, len);}
    void writeUint16(atUint16 val);
    inline void writeInt16(atInt16 val)
    {FileWriter::writeUint16(val);}
    void writeUint32(atUint32 val);
    inline void writeInt32(atInt32 val)
    {FileWriter::writeUint32(val);}
    void writeUint64(atUint64 val);
    inline void writeInt64(atInt64 val)
    {FileWriter::writeUint64(val);}
    void writeDouble(double val);
    void writeFloat(float val);
    inline void writeBool(bool val)
    {FileWriter::writeByte(val);}
    void writeVec3f(atVec3f vec);
    void writeVec4f(atVec4f vec);
    void writeString(const std::string& val, atInt32 fixedLen = -1);
    void writeWString(const std::wstring& str, atInt32 fixedLen = -1);
    void writeUnicode(const std::string& str, atInt32 fixedLen = -1);
    void fill(atInt8 byte, atUint64 len);
    inline void fill(atUint8 byte, atUint64 len)
    {FileWriter::fill((atInt8)byte, len);}
private:
    std::string  m_filename;
    FILE*        m_fileHandle;
    Endian       m_endian;
    atUint8      m_currentByte;
    atUint64     m_bytePosition;
    atUint8      m_bitShift;
    bool         m_bitValid;
};
}
} // Athena

#ifndef FILEWRITER_BASE
#define FILEWRITER_BASE() \
private: \
    typedef Athena::io::FileWriter base;

#endif // FILEWRITER_BASE
#endif // FILEWRITER_HPP
