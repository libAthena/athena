// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#include "BinaryWriter.hpp"
#include "IOException.hpp"
#include "InvalidOperationException.hpp"
#include "FileNotFoundException.hpp"
#include "utility.hpp"
#include "utf8.h"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

#ifdef HW_RVL
#include <malloc.h>
#endif // HW_RVL

namespace zelda
{
namespace io
{

BinaryWriter::BinaryWriter(const Uint8* data, Uint64 length)
    : Stream(data, length)
{}

BinaryWriter::BinaryWriter(const Stream& stream) :
    Stream(stream)
{}

BinaryWriter::BinaryWriter(const std::string& filename)
    : m_filepath(filename)
{
    m_length = 0x10;
    m_bitPosition = 0;
    m_position = 0;
#ifdef HW_RVL
    m_data = (Uint8*)memalign(32, m_length);
#else
    m_data = new Uint8[m_length];
#endif

    if (!m_data)
        throw error::IOException("BinaryWriter::BinaryWriter -> Could not allocate memory!");
    memset(m_data, 0, m_length);
}

void BinaryWriter::setFilepath(const std::string& filepath)
{
    m_filepath = filepath;
}

std::string BinaryWriter::filepath() const
{
    return m_filepath;
}

void BinaryWriter::save(const std::string& filename)
{
    if (filename.empty() && m_filepath.empty())
        throw error::InvalidOperationException("BinaryWriter::save -> No file specified, cannot save.");

    if (!filename.empty())
        m_filepath = filename;

    FILE* out = fopen(m_filepath.c_str(), "wb");
    if (!out)
        throw error::FileNotFoundException(m_filepath);

    Uint32 done = 0;
    Uint32 blocksize = BLOCKSZ;
    do
    {
        if (blocksize > m_length - done)
            blocksize = m_length - done;

        Int32 ret = fwrite(m_data + done, 1, blocksize, out);

        if (ret < 0)
            throw error::IOException("BinaryWriter::save Error writing data to disk");
        else if (ret == 0)
            break;

        done += blocksize;
    }while (done < m_length);

    fclose(out);
}

Int8 BinaryWriter::readByte()
{
    throw error::IOException("BinaryWriter::readByte -> Stream not open for reading");
}

Int8* BinaryWriter::readBytes(Int64)
{
    throw error::IOException("BinaryWriter::readBytes -> Stream not open for reading");
}

void BinaryWriter::writeInt16(Int16 val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Int16) > m_length && m_autoResize)
        resize(m_position + sizeof(Int16));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteInt16 -> Position outside stream bounds");

    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swap16(val);

    *(Int16*)(m_data + m_position) = val;
    m_position += sizeof(Int16);
}

void BinaryWriter::writeUInt16(Uint16 val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Uint16) > m_length && m_autoResize)
        resize(m_position + sizeof(Uint16));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteUInt16 -> Position outside stream bounds");


    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swapU16(val);

    *(Uint16*)(m_data + m_position) = val;
    m_position += sizeof(Uint16);
}

void BinaryWriter::writeInt32(Int32 val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Int32) > m_length && m_autoResize)
        resize(m_position + sizeof(Int32));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteInt32 -> Position outside stream bounds");

    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swap32(val);

    *(Int32*)(m_data + m_position) = val;
    m_position += sizeof(Int32);
}

void BinaryWriter::writeUInt32(Uint32 val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Uint32) > m_length && m_autoResize)
        resize(m_position + sizeof(Uint32));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteUInt32 -> Position outside stream bounds");

    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swapU32(val);

    *(Uint32*)(m_data + m_position) = val;
    m_position += sizeof(Uint32);
}

void BinaryWriter::writeInt64(Int64 val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Int64) > m_length && m_autoResize)
        resize(m_position + sizeof(Int64));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteInt64 -> Position outside stream bounds");


    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swap64(val);

    *(Int64*)(m_data + m_position) = val;
    m_position += sizeof(Int64);
}

void BinaryWriter::writeUInt64(Uint64 val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(Uint64) > m_length && m_autoResize)
        resize(m_position + sizeof(Uint64));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteUInt64 -> Position outside stream bounds");

    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swapU64(val);

    *(Uint64*)(m_data + m_position) = val;
    m_position += sizeof(Uint64);
}

void BinaryWriter::writeFloat(float val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(float) > m_length && m_autoResize)
        resize(m_position + sizeof(float));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteFloat -> Position outside stream bounds");

    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swapFloat(val);

    *(float*)(m_data + m_position) = val;
    m_position += sizeof(float);
}

void BinaryWriter::writeDouble(double val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(double) > m_length && m_autoResize)
        resize(m_position + sizeof(double));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteDouble -> Position outside stream bounds");

    if ((!utility::isSystemBigEndian() && m_endian == BigEndian) || (utility::isSystemBigEndian() && m_endian == LittleEndian))
        val = utility::swapDouble(val);

    *(double*)(m_data + m_position)= val;
    m_position += sizeof(double);
}

void BinaryWriter::writeBool(bool val)
{
    if (m_bitPosition > 0)
    {
        m_bitPosition = 0;
        m_position += sizeof(Uint8);
    }

    if (m_position + sizeof(bool) > m_length && m_autoResize)
        resize(m_position + sizeof(bool));
    else if (m_position > m_length)
        throw error::IOException("BinaryWriter::WriteBool -> Position outside stream bounds");


    *(bool*)(m_data + m_position) = val;
    m_position += sizeof(bool);
}

void BinaryWriter::writeUnicode(const std::string& str)
{
    std::string tmpStr = "\xEF\xBB\xBF" + str;

    std::vector<short> tmp;

    utf8::utf8to16(tmpStr.begin(), tmpStr.end(), back_inserter(tmp));

    for (Uint16 chr : tmp)
    {
        if (chr != 0xFEFF)
            writeInt16(chr);
    }
}

void BinaryWriter::writeString(const std::string& str)
{
    for (Uint8 c : str)
    {
        if (c != '\0')
            writeUByte(c);
    }
    writeUByte(0);
}

bool BinaryWriter::isOpenForReading()
{
    return false;
}
} // io
} // zelda
