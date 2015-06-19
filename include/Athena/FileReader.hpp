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
    std::string filename() const;

    void setEndian(Endian endian);
    Endian endian() const;
    bool isBigEndian() const;
    bool isLittleEndian() const;
    void open();
    void close();
    bool isOpen() const;
    bool save();
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
    inline void seekAlign32() {seek(ROUND_UP_32(position()), SeekOrigin::Begin);}
    bool atEnd() const;
    atUint64 position() const;
    atUint64 length() const;


    void   seekBit(int);
    bool   readBit();
    atUint8  readUByte();
    atInt8   readByte();
    atUint8* readUBytes(atUint64 len);
    atInt8*  readBytes(atUint64 len);
    atUint64 readBytesToBuf(void* buf, atUint64 len) {return readUBytesToBuf(buf, len);}
    atUint64 readUBytesToBuf(void* buf, atUint64 len);
    atUint16 readUint16();
    atInt16  readInt16();
    atUint32 readUint32();
    atInt32  readInt32();
    atUint64 readUint64();
    atInt64  readInt64();
    double readDouble();
    float  readFloat();
    bool   readBool();
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
