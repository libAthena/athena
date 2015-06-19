#include "Athena/FileWriter.hpp"
#include "Athena/FileNotFoundException.hpp"
#include "Athena/InvalidDataException.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/IOException.hpp"

#if _WIN32
#include "win32_largefilewrapper.h"
#elif __APPLE__
#include "osx_largefilewrapper.h"
#endif

#include "utf8.h"

namespace Athena
{
namespace io
{
FileWriter::FileWriter(const std::string& filename, bool overwrite)
    : m_filename(filename),
      m_fileHandle(NULL),
      m_endian(Endian::LittleEndian),
      m_bytePosition(0),
      m_bitShift(0),
      m_bitValid(false)
{
    open(overwrite);
}

FileWriter::~FileWriter()
{
    if (isOpen())
        close();
}

void FileWriter::setEndian(Endian endian)
{
    m_endian = endian;
}

Endian FileWriter::endian() const
{
    return m_endian;
}

bool FileWriter::isBigEndian() const
{
    return (m_endian == Endian::BigEndian);
}

bool FileWriter::isLittleEndian() const
{
    return (m_endian == Endian::LittleEndian);
}

void FileWriter::open(bool overwrite)
{
    if (overwrite)
        m_fileHandle = fopen(m_filename.c_str(), "w+b");
    else
        m_fileHandle = fopen(m_filename.c_str(), "r+b");

    if (!m_fileHandle)
        THROW_FILE_NOT_FOUND_EXCEPTION(m_filename);

    // ensure we're at the beginning of the file
    rewind(m_fileHandle);
}

void FileWriter::close()
{
    if (!m_fileHandle)
        THROW_INVALID_OPERATION_EXCEPTION("Cannot close an unopened stream");

    fclose(m_fileHandle);
    m_fileHandle = NULL;
    return;
}

bool FileWriter::isOpen() const
{
    return m_fileHandle != NULL;
}

void FileWriter::seek(atInt64 pos, SeekOrigin origin)
{
    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
        THROW_IO_EXCEPTION("Unable to seek in file");
}

bool FileWriter::atEnd() const
{
    return feof(m_fileHandle) != 0;
}

atUint64 FileWriter::position() const
{
    return ftello64(m_fileHandle);
}

atUint64 FileWriter::length() const
{
    return utility::fileSize(m_filename);
}

void FileWriter::writeBit(bool val)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    if (!m_bitValid)
    {
        m_bitValid = true;
        m_bitShift = 0;
        m_bytePosition = ftello64(m_fileHandle);
    }


    if (val)
        m_currentByte |= (1 << m_bitShift++);
    else
        m_currentByte &= ~(1 << m_bitShift++);

    if (m_bitShift > 7)
        m_bitValid = false;

    fseeko64(m_fileHandle, m_bytePosition, (int)SeekOrigin::Begin);

    if (fwrite(&m_currentByte, 1, 1, m_fileHandle) != sizeof(atInt8))
        THROW_IO_EXCEPTION("Unable to data to file");
}

void FileWriter::seekBit(int bit)
{
    if (bit < 0 || bit > 7)
        THROW_INVALID_OPERATION_EXCEPTION("bit must be >= 0 and <= 7");

    m_bitShift = bit;
    m_bitValid = true;
}

void FileWriter::writeUByte(atUint8 val)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if (fwrite(&val, 1, sizeof(atUint8), m_fileHandle) != sizeof(atUint8))
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeByte(atInt8 val)
{
    writeUByte(val);
}

void FileWriter::writeUBytes(atUint8* data, atUint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if (fwrite(data, 1, len, m_fileHandle) != len)
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeBytes(atInt8* data, atUint64 len)
{
    writeUBytes((atUint8*)data, len);
}

void FileWriter::writeUint16(atUint16 val)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapU16(val);

    if (fwrite(&val, 1, sizeof(atUint16), m_fileHandle) != sizeof(atUint16))
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeInt16(atInt16 val)
{
    writeUint16(val);
}

void FileWriter::writeUint32(atUint32 val)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapU32(val);

    if (fwrite(&val, 1, sizeof(atUint32), m_fileHandle) != sizeof(atUint32))
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeInt32(atInt32 val)
{
    writeUint32(val);
}

void FileWriter::writeUint64(atUint64 val)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapU64(val);

    if (fwrite(&val, 1, sizeof(atUint64), m_fileHandle) != sizeof(atUint64))
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeInt64(atInt64 val)
{
    writeUint64(val);
}

void FileWriter::writeDouble(double val)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapDouble(val);

    if (fwrite(&val, 1, sizeof(double), m_fileHandle) != sizeof(double))
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeFloat(float val)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        val = utility::swapFloat(val);

    if (fwrite(&val, 1, sizeof(float), m_fileHandle) != sizeof(float))
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeBool(bool val)
{
    writeByte(val);
}

void FileWriter::writeVec3f(atVec3f vec)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
    {
        vec.vec[0] = utility::swapFloat(vec.vec[0]);
        vec.vec[1] = utility::swapFloat(vec.vec[1]);
        vec.vec[2] = utility::swapFloat(vec.vec[2]);
    }

    if (fwrite(&vec, 1, 12, m_fileHandle) != 12)
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeVec4f(atVec4f vec)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
    {
        vec.vec[0] = utility::swapFloat(vec.vec[0]);
        vec.vec[1] = utility::swapFloat(vec.vec[1]);
        vec.vec[2] = utility::swapFloat(vec.vec[2]);
        vec.vec[3] = utility::swapFloat(vec.vec[3]);
    }

    if (fwrite(&vec, 1, 16, m_fileHandle) != 16)
        THROW_IO_EXCEPTION("Unable to write to stream");
}

void FileWriter::writeString(const std::string& val, atInt32 fixedLen)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;
    char term = '\0';

    if (fixedLen < 0)
    {
        if (fwrite(val.c_str(), 1, val.length(), m_fileHandle) != val.length())
            THROW_IO_EXCEPTION("Unable to write to stream");

        if (fwrite(&term, 1, 1, m_fileHandle) != 1)
            THROW_IO_EXCEPTION("Unable to write to stream");
    }
    else
    {
        if (val.length() >= fixedLen)
        {
            if (fwrite(val.c_str(), 1, fixedLen, m_fileHandle) != fixedLen)
                THROW_IO_EXCEPTION("Unable to write to stream");
        }
        else
        {
            if (fwrite(val.c_str(), 1, val.length(), m_fileHandle) != val.length())
                THROW_IO_EXCEPTION("Unable to write to stream");
            for (atUint32 i=val.length() ; i<fixedLen ; ++i)
            {
                if (fwrite(&term, 1, 1, m_fileHandle) != 1)
                    THROW_IO_EXCEPTION("Unable to write to stream");
            }
        }
    }
}

void FileWriter::writeWString(const std::wstring& val, atInt32 fixedLen)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    m_bitValid = false;
    wchar_t term = L'\0';

    if (fixedLen < 0)
    {
        if (fwrite(val.c_str(), 2, val.length(), m_fileHandle) != val.length())
            THROW_IO_EXCEPTION("Unable to write to stream");

        if (fwrite(&term, 2, 1, m_fileHandle) != 1)
            THROW_IO_EXCEPTION("Unable to write to stream");
    }
    else
    {
        if (val.length() >= fixedLen)
        {
            if (fwrite(val.c_str(), 2, fixedLen, m_fileHandle) != fixedLen)
                THROW_IO_EXCEPTION("Unable to write to stream");
        }
        else
        {
            if (fwrite(val.c_str(), 2, val.length(), m_fileHandle) != val.length())
                THROW_IO_EXCEPTION("Unable to write to stream");
            for (atUint32 i=val.length() ; i<fixedLen ; ++i)
            {
                if (fwrite(&term, 2, 1, m_fileHandle) != 1)
                    THROW_IO_EXCEPTION("Unable to write to stream");
            }
        }
    }
}

void FileWriter::writeUnicode(const std::string& str, atInt32 fixedLen)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    std::string tmpStr = "\xEF\xBB\xBF" + str;

    std::vector<atUint16> tmp;

    utf8::utf8to16(tmpStr.begin(), tmpStr.end(), back_inserter(tmp));

    if (fixedLen < 0)
    {
        for (atUint16 chr : tmp)
        {
            if (chr != 0xFEFF)
                writeUint16(chr);
        }
        writeUint16(0);
    }
    else
    {
        auto it = tmp.begin();
        for (atInt32 i=0 ; i<fixedLen ; ++i)
        {
            atUint16 chr;
            if (it == tmp.end())
                chr = 0;
            else
                chr = *it++;

            if (chr == 0xFEFF)
            {
                --i;
                continue;
            }

            writeUint16(chr);
        }
    }
}

void FileWriter::fill(atInt8 byte, atUint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    fwrite(&byte, 1, len, m_fileHandle);
}

void FileWriter::fill(atUint8 byte, atUint64 len)
{
    fill((atInt8)byte, len);
}

}
} // Athena
