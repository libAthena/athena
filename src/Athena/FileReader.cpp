// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#include "Athena/FileReader.hpp"
#include "Athena/FileNotFoundException.hpp"
#include "Athena/InvalidDataException.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/IOException.hpp"
#include "utf8.h"

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

void FileReader::seek(Int64 pos, SeekOrigin origin)
{
    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
        THROW_INVALID_OPERATION_EXCEPTION("Unable to seek in file");
}

bool FileReader::atEnd() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open");

    return feof(m_fileHandle) != 0;
}

Uint64 FileReader::position() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open");

    return ftello64(m_fileHandle);
}

Uint64 FileReader::length() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open");

    return utility::fileSize(m_fileHandle);
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
        THROW_INVALID_OPERATION_EXCEPTION("File is not open for reading");

    if (!m_bitValid)
    {
        int size = fread(&m_currentByte, 1, 1, m_fileHandle);
        if (size != sizeof(Uint8))
            THROW_IO_EXCEPTION("Error reading from file.");

        m_bitShift = 0;
        m_bitValid = true;
    }

    Uint8 flag = (1 << m_bitShift);
    m_bitShift++;
    if (m_bitShift > 7)
        m_bitValid = false;

    return ((m_currentByte & flag) == flag);
}

Uint8 FileReader::readUByte()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    m_bitValid = false;
    Uint8 val = 0;
    fread(&val, 1, 1, m_fileHandle);
    return val;
}

Int8 FileReader::readByte()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");
    return (Int8)readUByte();
}

Uint8* FileReader::readUBytes(Uint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");
    m_bitValid = false;
    Uint8* val = new Uint8[len];
    fread(val, 1, len, m_fileHandle);
    return val;
}

Int8* FileReader::readBytes(Uint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");
    return (Int8*)readUBytes(len);
}

Uint16 FileReader::readUint16()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    m_bitValid = false;
    Uint16 val;
    fread(&val, 1, sizeof(Uint16), m_fileHandle);

    if ((utility::isSystemBigEndian() && !isBigEndian()) || (!utility::isSystemBigEndian() && isBigEndian()))
        utility::swapU16(val);

    return val;
}

Int16 FileReader::readInt16()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    return (Int16)readUint16();
}

Uint32 FileReader::readUint32()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    m_bitValid = false;
    Uint32 val;
    fread(&val, 1, sizeof(Uint32), m_fileHandle);

    if ((utility::isSystemBigEndian() && !isBigEndian()) || (!utility::isSystemBigEndian() && isBigEndian()))
        utility::swapU64(val);

    return val;
}

Int32 FileReader::readInt32()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    return (Int32)readUint32();
}

Uint64 FileReader::readUint64()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    m_bitValid = false;
    Uint64 val;
    fread(&val, 1, sizeof(Uint64), m_fileHandle);

    if ((utility::isSystemBigEndian() && !isBigEndian()) || (!utility::isSystemBigEndian() && isBigEndian()))
        utility::swapU64(val);

    return val;
}

Int64 FileReader::readInt64()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    return (Int64)readUint64();
}

double FileReader::readDouble()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    m_bitValid = false;
    double val;
    fread(&val, 1, sizeof(double), m_fileHandle);

    if ((utility::isSystemBigEndian() && !isBigEndian()) || (!utility::isSystemBigEndian() && isBigEndian()))
        utility::swapDouble(val);

    return val;
}

float FileReader::readFloat()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    m_bitValid = false;
    float val;
    fread(&val, 1, sizeof(float), m_fileHandle);

    if ((utility::isSystemBigEndian() && !isBigEndian()) || (!utility::isSystemBigEndian() && isBigEndian()))
        utility::swapFloat(val);

    return val;
}

bool FileReader::readBool()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    return (readByte() != 0);
}

std::string FileReader::readString()
{
    std::string ret = "";
    Uint8 chr = readByte();

    while (chr != 0)
    {
        ret += chr;
        chr = readByte();
    }

    return ret;
}

std::string FileReader::readUnicode()
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION("File not open for reading");

    std::string ret;
    std::vector<short> tmp;

    for(;;)
    {
        short chr = readUint16();
        if (chr)
            tmp.push_back(chr);
        else
            break;
    };

    utf8::utf16to8(tmp.begin(), tmp.end(), back_inserter(ret));
    return ret;
}

} // io
} // Athena
