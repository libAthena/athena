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

#include "Athena/MemoryReader.hpp"
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
MemoryReader::MemoryReader(const atUint8* data, atUint64 length)
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

    m_data = new atUint8[m_length];
    memcpy(m_data, data, m_length);
}

MemoryReader::MemoryReader(const std::string& filename, std::function<void(int)> progFun)
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

MemoryReader::~MemoryReader()
{
    delete[] m_data;
    m_data = NULL;
}

void MemoryReader::setEndian(Endian endian)
{
    m_endian = endian;
}

Endian MemoryReader::endian() const
{
    return m_endian;
}

bool MemoryReader::isBigEndian() const
{
    return (m_endian == Endian::BigEndian);
}

bool MemoryReader::isLittleEndian() const
{
    return (m_endian == Endian::LittleEndian);
}

bool MemoryReader::isOpen() const
{
    return m_data != nullptr;
}

void MemoryReader::seek(atInt64 position, SeekOrigin origin)
{
    switch (origin)
    {
    case SeekOrigin::Begin:
        if ((position < 0 || (atInt64)position > (atInt64)m_length))
            THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);
        m_position = position;
    break;
    case SeekOrigin::Current:
        if ((((atInt64)m_position + position) < 0 || (m_position + position) > m_length))
            THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);
        m_position += position;
    break;
    case SeekOrigin::End:
        if ((((atInt64)m_length - position < 0) || (m_length - position) > m_length))
            THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);
        m_position = m_length - position;
    break;
    }
}

bool MemoryReader::atEnd() const
{
    return m_position >= m_length;
}

atUint64 MemoryReader::position() const
{
    return m_position;
}

atUint64 MemoryReader::length() const
{
    return m_length;
}

void MemoryReader::setData(const atUint8* data, atUint64 length)
{
    if (m_data)
        delete[] m_data;

    m_data = (atUint8*)data;
    m_length = length;
    m_position = 0;
    m_bitPosition = 0;
}

atUint8* MemoryReader::data() const
{
    atUint8* ret = new atUint8[m_length];
    memset(ret, 0, m_length);
    memcpy(ret, m_data, m_length);
    return ret;
}

void MemoryReader::setFilepath(const std::string& filepath)
{
    m_filepath = filepath;
}

std::string MemoryReader::filepath() const
{
    return m_filepath;
}

void MemoryReader::seekBit(int bit)
{
    if (!m_data)
        loadData();

    if (bit < 0 || bit > 7)
        THROW_INVALID_OPERATION_EXCEPTION("bit out of range %i %s",  bit, (bit < 0 ? "< 0" : "> 7"));

    m_bitPosition = bit;
}

bool MemoryReader::readBit()
{
    if (!m_data)
        loadData();
    if (m_position > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    bool ret = (*(atUint8*)(m_data + m_position) & (1 << m_bitPosition)) != 0;

    m_bitPosition++;
    if (m_bitPosition > 7)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }

    return ret;
}

atInt8 MemoryReader::readByte()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }
    if (m_position + 1 > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    return *(atInt8*)(m_data + m_position++);
}

atUint8 MemoryReader::readUByte()
{
    if (!m_data)
        loadData();

    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }
    if (m_position + 1 > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    return *(atUint8*)(m_data + m_position++);
}

atInt8* MemoryReader::readBytes(atUint64 length)
{
    return (atInt8*)readUBytes(length);
}

atUint8* MemoryReader::readUBytes(atUint64 length)
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }

    if (m_position + length > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    atUint8* ret;
    ret = new atUint8[length];

    memcpy(ret, (const atUint8*)(m_data + m_position), length);
    m_position += length;
    return ret;
}

atInt16 MemoryReader::readInt16()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }

    if (m_position + sizeof(atInt16) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);
    atInt16 ret = *(atInt16*)(m_data + m_position);
    m_position += sizeof(atInt16);

    if (isBigEndian())
        utility::BigInt16(ret);
    else
        utility::LittleInt16(ret);

    return ret;
}

atUint16 MemoryReader::readUint16()
{
    return readInt16();
}

atInt32 MemoryReader::readInt32()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }

    if (m_position + sizeof(atInt32) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);
    atInt32 ret = *(atInt32*)(m_data + m_position);
    m_position += 4;

    if (isBigEndian())
        utility::BigInt32(ret);
    else
        utility::LittleInt32(ret);

    return ret;
}

atUint32 MemoryReader::readUint32()
{
    return readInt32();
}

atInt64 MemoryReader::readInt64()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }
    if (m_position + sizeof(atInt64) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    atInt64 ret = *(atInt64*)(m_data + m_position);
    m_position += 8;

    if (isBigEndian())
        utility::BigInt64(ret);
    else
        utility::LittleInt64(ret);

    return ret;
}

atUint64 MemoryReader::readUint64()
{
    return readInt64();
}

float MemoryReader::readFloat()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }
    if (m_position + sizeof(float) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    float ret = *(float*)(m_data + m_position);
    m_position += 4;

    if (isBigEndian())
        utility::BigFloat(ret);
    else
        utility::LittleFloat(ret);

    return ret;
}

double MemoryReader::readDouble()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }
    if (m_position + sizeof(double) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    double ret = *(double*)(m_data + m_position);
    m_position += 8;

    if (isBigEndian())
        utility::BigDouble(ret);
    else
        utility::LittleDouble(ret);
    return ret;
}

bool MemoryReader::readBool()
{
    if (!m_data)
        loadData();

    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(atUint8);
    }
    if (m_position + sizeof(bool) > m_length)
        THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", m_position);

    bool ret = *(bool*)(m_data + m_position);
    m_position += 1;
    return ret;
}

std::string MemoryReader::readUnicode(atInt32 maxlen)
{
    if (!m_data)
        loadData();
    std::string ret;
    std::vector<short> tmp;
    atUint16 chr = readUint16();

    atInt32 i = 0;
    for(;;)
    {
        if (maxlen >= 0 && i >= maxlen - 1)
            break;

        if (!chr)
            break;
        tmp.push_back(chr);
        chr = readUint16();
        i++;
    }

    utf8::utf16to8(tmp.begin(), tmp.end(), back_inserter(ret));
    return ret;
}

std::string MemoryReader::readString(atInt32 maxlen)
{
    std::string ret = "";
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

void MemoryReader::setProgressCallback(std::function<void (int)> cb)
{
    m_progressCallback = cb;
}

void MemoryReader::loadData()
{
    FILE* in;
    atUint64 length;
    in = fopen(m_filepath.c_str(), "rb");

    if (!in)
        THROW_FILE_NOT_FOUND_EXCEPTION(m_filepath);
    rewind(in);

    length = utility::fileSize(m_filepath);
#ifdef HW_RVL
    m_data = (Uint8*)memalign(32, length);
#else
    m_data = new atUint8[length];
#endif

    atUint64 done = 0;
    atUint64 blocksize = BLOCKSZ;
    do
    {
        if (blocksize > length - done)
            blocksize = length - done;

        atInt64 ret = fread(m_data + done, 1, blocksize, in);

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
