#include "Athena/FileReader.hpp"
#include "Athena/FileNotFoundException.hpp"
#include "Athena/InvalidDataException.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/IOException.hpp"
#include "utf8.h"

#if _WIN32
#include "win32_largefilewrapper.h"
#elif __APPLE__
#include "osx_largefilewrapper.h"
#endif

namespace Athena
{
namespace io
{
FileReader::FileReader(const std::string& filename)
    : m_filename(filename),
      m_fileHandle(NULL),
      m_endian(Endian::LittleEndian),
      m_bitValid(false)
{
    open();
}

FileReader::~FileReader()
{
    if (isOpen())
        close();
}

std::string FileReader::filename() const
{
    return m_filename;
}

void FileReader::setEndian(Endian endian)
{
    m_endian = endian;
}

Endian FileReader::endian() const
{
    return m_endian;
}

bool FileReader::isBigEndian() const
{
    return (m_endian == Endian::BigEndian);
}

bool FileReader::isLittleEndian() const
{
    return (m_endian == Endian::LittleEndian);
}

void FileReader::open()
{
    m_fileHandle = fopen(m_filename.c_str(), "rb");

    if (!m_fileHandle)
        THROW_FILE_NOT_FOUND_EXCEPTION(m_filename);

    // ensure we're at the beginning of the file
    rewind(m_fileHandle);
}

void FileReader::close()
{
    if (!m_fileHandle)
        THROW_INVALID_OPERATION_EXCEPTION("Cannot close an unopened stream");

    fclose(m_fileHandle);
    m_fileHandle = NULL;
    return;
}

bool FileReader::isOpen() const
{
    return m_fileHandle != NULL;
}

void FileReader::seek(atInt64 pos, SeekOrigin origin)
{
    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
        THROW_INVALID_OPERATION_EXCEPTION("Unable to seek in file");
}

bool FileReader::atEnd() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(true, "File not open");

    return feof(m_fileHandle) != 0;
}

atUint64 FileReader::position() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open");

    return ftello64(m_fileHandle);
}

atUint64 FileReader::length() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open");

    return utility::fileSize(m_filename);
}

void FileReader::seekBit(int bit)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open");

    if (bit < 0 || bit > 7)
        THROW_INVALID_OPERATION_EXCEPTION("bit out of range");

    m_bitShift = bit;
}

bool FileReader::readBit()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(false, "File is not open for reading");

    if (!m_bitValid)
    {
        size_t size = fread(&m_currentByte, 1, 1, m_fileHandle);

        if (size != sizeof(atUint8))
            THROW_IO_EXCEPTION_RETURN(false, "Error reading from file.");

        m_bitShift = 0;
        m_bitValid = true;
    }

    atUint8 flag = (1 << m_bitShift);
    m_bitShift++;

    if (m_bitShift > 7)
        m_bitValid = false;

    return ((m_currentByte & flag) == flag);
}

atUint8 FileReader::readUByte()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    m_bitValid = false;
    atUint8 val = 0;
    fread(&val, 1, 1, m_fileHandle);
    return val;
}

atInt8 FileReader::readByte()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    return (atInt8)readUByte();
}

atUint8* FileReader::readUBytes(atUint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(nullptr, "File not open for reading");

    m_bitValid = false;
    atUint8* val = new atUint8[len];
    fread(val, 1, len, m_fileHandle);
    return val;
}

atUint64 FileReader::readUBytesToBuf(void* buf, atUint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    m_bitValid = false;
    return fread(buf, 1, len, m_fileHandle);
}

atInt8* FileReader::readBytes(atUint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(nullptr, "File not open for reading");

    return (atInt8*)readUBytes(len);
}

atUint16 FileReader::readUint16()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    m_bitValid = false;
    atUint16 val;
    fread(&val, 1, sizeof(atUint16), m_fileHandle);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapU16(val);

    return val;
}

atInt16 FileReader::readInt16()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    return (atInt16)readUint16();
}

atUint32 FileReader::readUint32()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    m_bitValid = false;
    atUint32 val;
    fread(&val, 1, sizeof(atUint32), m_fileHandle);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapU32(val);

    return val;
}

atInt32 FileReader::readInt32()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    return (atInt32)readUint32();
}

atUint64 FileReader::readUint64()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    m_bitValid = false;
    atUint64 val;
    fread(&val, 1, sizeof(atUint64), m_fileHandle);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapU64(val);

    return val;
}

atInt64 FileReader::readInt64()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    return (atInt64)readUint64();
}

double FileReader::readDouble()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    m_bitValid = false;
    double val;
    fread(&val, 1, sizeof(double), m_fileHandle);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapDouble(val);

    return val;
}

float FileReader::readFloat()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    m_bitValid = false;
    float val;
    fread(&val, 1, sizeof(float), m_fileHandle);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapFloat(val);

    return val;
}

bool FileReader::readBool()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(false, "File not open for reading");

    return (readByte() != 0);
}

atVec3f FileReader::readVec3f()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN({}, "File not open for reading");

    m_bitValid = false;
    atVec3f val = {};
    fread(&val, 1, 12, m_fileHandle);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
    {
        val.vec[0] = utility::swapFloat(val.vec[0]);
        val.vec[1] = utility::swapFloat(val.vec[1]);
        val.vec[2] = utility::swapFloat(val.vec[2]);
    }

    return val;
}

atVec4f FileReader::readVec4f()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN({}, "File not open for reading");

    m_bitValid = false;
    atVec4f val = {};
    fread(&val, 1, 16, m_fileHandle);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
    {
        val.vec[0] = utility::swapFloat(val.vec[0]);
        val.vec[1] = utility::swapFloat(val.vec[1]);
        val.vec[2] = utility::swapFloat(val.vec[2]);
        val.vec[3] = utility::swapFloat(val.vec[3]);
    }

    return val;
}

std::string FileReader::readString(atInt32 maxlen)
{
    std::string ret;
    atUint8 chr = readByte();

    atInt32 i = 0;

    while (chr != 0)
    {
        if (maxlen >= 0 && i >= maxlen - 1)
            break;

        ret += chr;
        chr = readByte();
        i++;
    }

    return ret;
}

std::wstring FileReader::readWString(atInt32 maxlen)
{
    std::wstring ret;
    atUint16 chr = readUint16();

    atInt32 i = 0;

    while (chr != 0)
    {
        if (maxlen >= 0 && i >= maxlen - 1)
            break;

        ret += chr;
        chr = readUint16();
        i++;
    }

    return ret;
}

std::string FileReader::readUnicode(atInt32 maxlen)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(std::string(), "File not open for reading");

    std::string ret;
    std::vector<short> tmp;

    atInt32 i = 0;

    for (;;)
    {
        if (maxlen >= 0 && i >= maxlen - 1)
            break;

        short chr = readUint16();

        if (chr)
            tmp.push_back(chr);
        else
            break;

        i++;
    };

    utf8::utf16to8(tmp.begin(), tmp.end(), back_inserter(ret));

    return ret;
}

} // io
} // Athena
