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

#include "Athena/BinaryReader.hpp"
#include "Athena/IOException.hpp"
#include "Athena/FileNotFoundException.hpp"
#include "Athena/InvalidDataException.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "utf8.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cstring>

#ifdef HW_RVL
#include <malloc.h>
#endif // HW_RVL

namespace Athena
{
namespace io
{
BinaryReader::BinaryReader(const Uint8* data, Uint64 length)
    : m_length(length),
      m_position(0),
      m_bitPosition(0),
      m_endian(Endian::LittleEndian),
      m_progressCallback(nullptr)
{
    if (!data)
        THROW_INVALID_DATA_EXCEPTION("data cannot be NULL");
    if (length == 0)
        THROW_INVALID_OPERATION_EXCEPTION("length cannot be 0");

    m_data = new Uint8[m_length];
    memcpy(m_data, data, m_length);
}

BinaryReader::BinaryReader(const std::string& filename, std::function<void(int)> progFun)
    : m_data(NULL),
      m_length(0),
      m_filepath(filename),
      m_position(0),
      m_bitPosition(0),
      m_endian(Endian::LittleEndian),
      m_progressCallback(progFun)
{
    loadData();
}

BinaryReader::~BinaryReader()
{
    delete[] m_data;
    m_data = NULL;
}

void BinaryReader::setEndian(Endian endian)
{
    m_endian = endian;
}

Endian BinaryReader::endian() const
{
    return m_endian;
}

bool BinaryReader::isBigEndian() const
{
    return (m_endian == Endian::BigEndian);
}

bool BinaryReader::isLittleEndian() const
{
    return (m_endian == Endian::LittleEndian);
}

bool BinaryReader::isOpen() const
{
    return m_data != nullptr;
}

void BinaryReader::seek(Int64 position, SeekOrigin origin)
{
    switch (origin)
    {
    case SeekOrigin::Begin:
        if ((position < 0 || (Int64)position > (Int64)m_length))
            THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);
        m_position = position;
    break;
    case SeekOrigin::Current:
        if ((((Int64)m_position + position) < 0 || (m_position + position) > m_length))
            THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);
        m_position += position;
    break;
    case SeekOrigin::End:
        if ((((Int64)m_length - position < 0) || (m_length - position) > m_length))
            THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);
        m_position = m_length - position;
    break;
    }
}

bool BinaryReader::atEnd() const
{
    return m_position >= m_length;
}

Uint64 BinaryReader::position() const
{
    return m_position;
}

Uint64 BinaryReader::length() const
{
    return m_length;
}

void BinaryReader::setData(const Uint8* data, Uint64 length)
{
    if (m_data)
        delete[] m_data;

    m_data = (Uint8*)data;
    m_length = length;
    m_position = 0;
    m_bitPosition = 0;
}

Uint8* BinaryReader::data() const
{
    Uint8* ret = new Uint8[m_length];
    memset(ret, 0, m_length);
    memcpy(ret, m_data, m_length);
    return ret;
}

void BinaryReader::setFilepath(const std::string& filepath)
{
    m_filepath = filepath;
}

std::string BinaryReader::filepath() const
{
    return m_filepath;
}

void BinaryReader::seekBit(int bit)
{
    if (!m_data)
        loadData();

    if (bit < 0 || bit > 7)
        THROW_INVALID_OPERATION_EXCEPTION("bit out of range %i %s",  bit, (bit < 0 ? "< 0" : "> 7"));

    m_bitPosition = bit;
}

bool BinaryReader::readBit()
{
    if (!m_data)
        loadData();
    if (m_position > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    bool ret = (*(Uint8*)(m_data + m_position) & (1 << m_bitPosition));

    m_bitPosition++;
    if (m_bitPosition > 7)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    return ret;
}

Int8 BinaryReader::readByte()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }
    if (m_position + 1 > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    return *(Int8*)(m_data + m_position++);
}

Uint8 BinaryReader::readUByte()
{
    if (!m_data)
        loadData();

    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }
    if (m_position + 1 > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    return *(Uint8*)(m_data + m_position++);
}

Int8* BinaryReader::readBytes(Int64 length)
{
    return (Int8*)readUBytes(length);
}

Uint8* BinaryReader::readUBytes(Int64 length)
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + length > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    Uint8* ret;
    ret = new Uint8[length];

    memcpy(ret, (const Uint8*)(m_data + m_position), length);
    m_position += length;
    return ret;
}

Int16 BinaryReader::readInt16()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Int16) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);
    Int16 ret = *(Int16*)(m_data + m_position);
    m_position += sizeof(Int16);

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        ret = utility::swap16(ret);
    return ret;
}

Uint16 BinaryReader::readUint16()
{
    return readInt16();
}

Int32 BinaryReader::readInt32()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Int32) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);
    Int32 ret = *(Int32*)(m_data + m_position);
    m_position += 4;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        ret = utility::swap32(ret);
    return ret;
}

Uint32 BinaryReader::readUint32()
{
    return readInt32();
}

Int64 BinaryReader::readInt64()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }
    if (m_position + sizeof(Int64) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    Int64 ret = *(Int64*)(m_data + m_position);
    m_position += 8;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        ret = utility::swap64(ret);
    return ret;
}

Uint64 BinaryReader::readUint64()
{
    return readUint64();
}

float BinaryReader::readFloat()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }
    if (m_position + sizeof(float) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    float ret = *(float*)(m_data + m_position);
    m_position += 4;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        ret = utility::swapFloat(ret);
    return ret;
}

double BinaryReader::readDouble()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }
    if (m_position + sizeof(double) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    double ret = *(double*)(m_data + m_position);
    m_position += 8;

    if ((!utility::isSystemBigEndian() && isBigEndian()) || (utility::isSystemBigEndian() && isLittleEndian()))
        ret = utility::swapDouble(ret);

    return ret;
}

bool BinaryReader::readBool()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }
    if (m_position + sizeof(bool) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    bool ret = *(bool*)(m_data + m_position);
    m_position += 1;
    return ret;
}

std::string BinaryReader::readUnicode()
{
    if (!m_data)
        loadData();
    std::string ret;
    std::vector<short> tmp;
    Uint16 chr = readUint16();
    for(;;)
    {
        if (!chr)
            break;
        tmp.push_back(chr);
        chr = readUint16();
    }

    utf8::utf16to8(tmp.begin(), tmp.end(), back_inserter(ret));
    return ret;
}

std::string BinaryReader::readString()
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

void BinaryReader::setProgressCallback(std::function<void (int)> cb)
{
    m_progressCallback = cb;
}

void BinaryReader::loadData()
{
    FILE* in;
    Uint32 length;
    in = fopen(m_filepath.c_str(), "rb");

    if (!in)
        THROW_FILE_NOT_FOUND_EXCEPTION(m_filepath);

    fseek(in, 0, SEEK_END);
    length = ftell(in);
    fseek(in, 0, SEEK_SET);
#ifdef HW_RVL
    m_data = (Uint8*)memalign(32, length);
#else
    m_data = new Uint8[length];
#endif

    Uint32 done = 0;
    Uint32 blocksize = BLOCKSZ;
    do
    {
        if (blocksize > length - done)
            blocksize = length - done;

        Int32 ret = fread(m_data + done, 1, blocksize, in);

        if (ret < 0)
            THROW_IO_EXCEPTION("Error reading data from disk");
        else if (ret == 0)
            break;

        done += ret;

        if (m_progressCallback)
            m_progressCallback((int)((float)(done* 100.f)/length));

    } while (done < length);

    fclose(in);
    m_length = length;
    m_position = 0;
    m_bitPosition = 0;
}

}
}
