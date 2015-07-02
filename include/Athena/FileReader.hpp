#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include "Athena/IStreamReader.hpp"
#include <string>
#include <stdio.h>

namespace Athena
{
namespace io
{
class FileReader : public IStreamReader
{
public:
    FileReader(const std::string& filename);
    virtual ~FileReader();
    inline const std::string& filename() const
    {return m_filename;}

    inline void setEndian(Endian endian)
    {m_endian = endian;}
    inline Endian endian() const
    {return m_endian;}
    inline bool isBigEndian() const
    {return (m_endian == Endian::BigEndian);}
    inline bool isLittleEndian() const
    {return (m_endian == Endian::LittleEndian);}

    void open();
    void close();
    inline bool isOpen() const
    {return m_fileHandle != NULL;}
    bool save();
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
    inline void seekAlign32()
    {FileReader::seek(ROUND_UP_32(FileReader::position()), SeekOrigin::Begin);}
    bool atEnd() const;
    atUint64 position() const;
    atUint64 length() const;


    void   seekBit(int);
    bool   readBit();
    atUint8  readUByte();
    inline atInt8   readByte()
    {return (atInt8)FileReader::readUByte();}
    atUint8* readUBytes(atUint64 len);
    inline atInt8*  readBytes(atUint64 len)
    {return (atInt8*)FileReader::readUBytes(len);}
    atUint64 readBytesToBuf(void* buf, atUint64 len)
    {return FileReader::readUBytesToBuf(buf, len);}
    atUint64 readUBytesToBuf(void* buf, atUint64 len);
    atUint16 readUint16();
    inline atInt16  readInt16()
    {return (atInt16)FileReader::readUint16();}
    atUint32 readUint32();
    inline atInt32  readInt32()
    {return (atInt32)FileReader::readUint32();}
    atUint64 readUint64();
    inline atInt64  readInt64()
    {return (atInt64)FileReader::readUint64();}
    double readDouble();
    float  readFloat();
    inline bool   readBool()
    {return (FileReader::readByte() != 0);}
    atVec3f readVec3f();
    atVec4f readVec4f();
    std::string readString(atInt32 fixedLen = -1);
    std::wstring readWString(atInt32 fixedLen = -1);
    std::string readUnicode(atInt32 fixedLen = -1);
protected:
    std::string  m_filename;
    FILE*        m_fileHandle;
    Endian       m_endian;
    atUint8        m_currentByte;
    atUint8        m_bitShift;
    bool         m_bitValid;
};
} // io
} // Athena

#ifndef FILEREADER_BASE
#define FILEREADER_BASE() \
private: \
    typedef Athena::io::FileReader base

#endif // FILEREADER_BASE

#endif // FILESTREAM_HPP
