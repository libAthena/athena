#ifdef ATHENA_ENABLE_PHYSFS
#ifndef PHYSFSSTREAM_HPP
#define PHYSFSSTREAM_HPP

#include "athena/Stream.hpp"
#include <physfs.h>

namespace athena
{
namespace io
{
class PHYSFSFileReader : public Stream
{
public:
    PHYSFSFileReader(const std::string& path);

    void setEndian(Endian);
    Endian endian() const;
    bool isBigEndian() const;
    bool isLittleEndian() const;
    atUint8* data();

    atUint64 length() const;

    atUint64 position() const;

    bool isOpen() const;

    void     seek(atInt64 position, SeekOrigin origin);
    atInt8   readByte();
    atUint8  readUByte();
    atUint8* readUBytes(atUint64 length);
    atInt8*  readBytes(atUint64 length);
    atUint16 readUint16();
    atInt16  readInt16();
    atUint32 readUint32();
    atInt32  readInt32();
    atUint64 readUint64();
    atInt64  readInt64();
    double   readDouble();
    float    readFloat();
    bool     readBool();

    bool     atEnd() const;

    // TODO: Strings
private:
    Endian       m_endian;
    std::string  m_path;
    atUint64     m_length;
    PHYSFS_File* m_handle;
};
}
}

#ifndef PHYSFSFILEREADER_BASE
#define PHYSFSFILEREADER_BASE() \
    typedef athena::io::PHYSFSFileReader base
#endif
#endif // PHYSFSSTREAM_HPP
#endif // ATHENA_ENABLE_PHYSFS
