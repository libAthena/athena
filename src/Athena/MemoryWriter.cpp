#include "Athena/MemoryWriter.hpp"
#include "Athena/IOException.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/InvalidDataException.hpp"
#include "Athena/FileNotFoundException.hpp"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

#ifdef HW_RVL
#include <malloc.h>
#endif // HW_RVL

namespace Athena
{
namespace io
{

MemoryWriter::MemoryWriter(atUint8* data, atUint64 length)
    : m_data((atUint8*)data),
      m_length(length),
      m_position(0)
{
    if (!data)
        THROW_INVALID_DATA_EXCEPTION("data cannot be NULL");

    if (length == 0)
        THROW_INVALID_OPERATION_EXCEPTION("length cannot be 0");
}

MemoryCopyWriter::MemoryCopyWriter(atUint8* data, atUint64 length)
{
    m_data = data;
    m_length = length;
    m_position = 0;

    if (length == 0)
        THROW_INVALID_OPERATION_EXCEPTION("length cannot be 0");

    m_dataCopy.reset(new atUint8[length]);
    m_data = m_dataCopy.get();
    if (data)
        memcpy(m_data, data, length);
}

MemoryCopyWriter::MemoryCopyWriter(const std::string& filename)
    : MemoryWriter(NULL, 0)
{
    m_filepath = filename;
    m_length = 0x10;
    m_position = 0;
    m_dataCopy.reset(new atUint8[m_length]);
    m_data = m_dataCopy.get();

    if (!m_data)
        THROW_IO_EXCEPTION("Could not allocate memory!");

    memset(m_data, 0, m_length);
}

void MemoryWriter::seek(atInt64 position, SeekOrigin origin)
{
    switch (origin)
    {
        case SeekOrigin::Begin:
            if (position < 0)
                THROW_IO_EXCEPTION("Position outside stream bounds");

            if ((atUint64)position > m_length)
                THROW_IO_EXCEPTION("data exceeds available buffer space");

            m_position = position;
            break;

        case SeekOrigin::Current:
            if ((((atInt64)m_position + position) < 0))
                THROW_IO_EXCEPTION("Position outside stream bounds");

            if (m_position + position > m_length)
                THROW_IO_EXCEPTION("data exceeds available buffer space");

            m_position += position;
            break;

        case SeekOrigin::End:
            if (((atInt64)m_length - position) < 0)
                THROW_IO_EXCEPTION("Position outside stream bounds");

            if ((atUint64)position > m_length)
                THROW_IO_EXCEPTION("data exceeds available buffer space");

            m_position = m_length - position;
            break;
    }
}

void MemoryCopyWriter::seek(atInt64 position, SeekOrigin origin)
{
    switch (origin)
    {
        case SeekOrigin::Begin:
            if (position < 0)
                THROW_IO_EXCEPTION("Position outside stream bounds");

            if ((atUint64)position > m_length)
                resize(position);

            m_position = position;
            break;

        case SeekOrigin::Current:
            if ((((atInt64)m_position + position) < 0))
                THROW_IO_EXCEPTION("Position outside stream bounds");

            if (m_position + position > m_length)
                resize(m_position + position);

            m_position += position;
            break;

        case SeekOrigin::End:
            if (((atInt64)m_length - position) < 0)
                THROW_IO_EXCEPTION("Position outside stream bounds");

            if ((atUint64)position > m_length)
                resize(position);

            m_position = m_length - position;
            break;
    }
}

void MemoryWriter::setData(atUint8* data, atUint64 length)
{
    m_data = (atUint8*)data;
    m_length = length;
    m_position = 0;
}

void MemoryCopyWriter::setData(const atUint8* data, atUint64 length)
{
    m_dataCopy.reset(new atUint8[length]);
    m_data = m_dataCopy.get();
    memcpy(m_data, data, length);
    m_length = length;
    m_position = 0;
}

atUint8* MemoryWriter::data() const
{
    atUint8* ret = new atUint8[m_length];
    memset(ret, 0, m_length);
    memcpy(ret, m_data, m_length);
    return ret;
}


void MemoryWriter::save(const std::string& filename)
{
    if (filename.empty() && m_filepath.empty())
        THROW_INVALID_OPERATION_EXCEPTION("No file specified, cannot save.");

    if (!filename.empty())
        m_filepath = filename;

    FILE* out = fopen(m_filepath.c_str(), "wb");

    if (!out)
        THROW_FILE_NOT_FOUND_EXCEPTION(m_filepath);

    atUint64 done = 0;
    atUint64 blocksize = BLOCKSZ;

    do
    {
        if (blocksize > m_length - done)
            blocksize = m_length - done;

        atInt64 ret = fwrite(m_data + done, 1, blocksize, out);

        if (ret < 0)
            THROW_IO_EXCEPTION("Error writing data to disk");
        else if (ret == 0)
            break;

        done += blocksize;
    }
    while (done < m_length);

    fclose(out);
}

void MemoryWriter::writeUBytes(const atUint8* data, atUint64 length)
{
    if (!data)
        THROW_INVALID_DATA_EXCEPTION("data cannnot be NULL");

    if (m_position + length > m_length)
        THROW_IO_EXCEPTION("data length exceeds available buffer space");

    memcpy((atInt8*)(m_data + m_position), data, length);

    m_position += length;
}

void MemoryCopyWriter::writeUBytes(const atUint8* data, atUint64 length)
{
    if (!data)
        THROW_INVALID_DATA_EXCEPTION("data cannnot be NULL");

    if (m_position + length > m_length)
        resize(m_position + length);

    memcpy((atInt8*)(m_data + m_position), data, length);

    m_position += length;
}

void MemoryCopyWriter::resize(atUint64 newSize)
{
    if (newSize < m_length)
        THROW_INVALID_OPERATION_EXCEPTION("Stream::resize() -> New size cannot be less to the old size.");

    // Allocate and copy new buffer
    atUint8* newArray = new atUint8[newSize];
    memset(newArray, 0, newSize);

    if (m_dataCopy)
        memcpy(newArray, m_dataCopy.get(), m_length);
    m_dataCopy.reset(newArray);

    // Swap the pointer and size out for the new ones.
    m_data = newArray;
    m_length = newSize;
}

} // io
} // Athena
