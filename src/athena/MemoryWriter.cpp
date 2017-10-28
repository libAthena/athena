#include "athena/MemoryWriter.hpp"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>

#ifdef HW_RVL
#include <malloc.h>
#endif // HW_RVL

namespace athena
{
namespace io
{

MemoryWriter::MemoryWriter(atUint8* data, atUint64 length, bool takeOwnership)
    : m_data((atUint8*)data),
      m_length(length),
      m_position(0),
      m_bufferOwned(takeOwnership)
{
    if (!data)
    {
        atError("data cannot be NULL");
        setError();
        return;
    }
}

MemoryWriter::~MemoryWriter()
{
    if (m_bufferOwned)
        delete m_data;
    m_data = nullptr;
    m_length = 0;
}

MemoryCopyWriter::MemoryCopyWriter(atUint8* data, atUint64 length)
{
    m_data = data;
    m_length = length;
    m_position = 0;
    m_bufferOwned = false;

    if (length == 0)
    {
        atError("length cannot be 0");
        setError();
        return;
    }
    m_dataCopy.reset(new atUint8[length]);
    m_data = m_dataCopy.get();
    if (data)
        memmove(m_data, data, length);
}

MemoryCopyWriter::MemoryCopyWriter(const std::string& filename)
{
    m_filepath = filename;
    m_length = 0x10;
    m_position = 0;
    m_dataCopy.reset(new atUint8[m_length]);
    m_data = m_dataCopy.get();
    m_bufferOwned = false;

    if (!m_data)
    {
        atError("Could not allocate memory!");
        setError();
        return;
    }
}

void MemoryWriter::seek(atInt64 position, SeekOrigin origin)
{
    switch (origin)
    {
    case SeekOrigin::Begin:
        if (position < 0)
            {
                atError("Position outside stream bounds");
                setError();
                return;
            }

            if ((atUint64)position > m_length)
            {
                atError("data exceeds available buffer space");
                setError();
                return;
            }

            m_position = position;
            break;

        case SeekOrigin::Current:
            if ((((atInt64)m_position + position) < 0))
            {
                atError("Position outside stream bounds");
                setError();
                return;
            }

            if (m_position + position > m_length)
            {
                atError("data exceeds available buffer space");
                setError();
                return;
            }

            m_position += position;
            break;

        case SeekOrigin::End:
            if (((atInt64)m_length - position) < 0)
            {
                atError("Position outside stream bounds");
                setError();
                return;
            }

            if ((atUint64)position > m_length)
            {
                atError("data exceeds available buffer space");
                setError();
                return;
            }

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
            {
                atError("Position outside stream bounds");
                setError();
                return;
            }

            if ((atUint64)position > m_length)
                resize(position);

            m_position = position;
            break;

        case SeekOrigin::Current:
            if ((((atInt64)m_position + position) < 0))
            {
                atError("Position outside stream bounds");
                setError();
                return;
            }

            if (m_position + position > m_length)
                resize(m_position + position);

            m_position += position;
            break;

        case SeekOrigin::End:
            if (((atInt64)m_length - position) < 0)
            {
                atError("Position outside stream bounds");
                setError();
                return;
            }

            if ((atUint64)position > m_length)
                resize(position);

            m_position = m_length - position;
            break;
    }
}

void MemoryWriter::setData(atUint8* data, atUint64 length, bool takeOwnership)
{
    if (m_bufferOwned)
        delete m_data;

    m_data = (atUint8*)data;
    m_length = length;
    m_position = 0;
    m_bufferOwned = takeOwnership;
}

void MemoryCopyWriter::setData(const atUint8* data, atUint64 length)
{
    m_dataCopy.reset(new atUint8[length]);
    m_data = m_dataCopy.get();
    memmove(m_data, data, length);
    m_length = length;
    m_position = 0;
    m_bufferOwned = false;
}

atUint8* MemoryWriter::data() const
{
    atUint8* ret = new atUint8[m_length];
    memset(ret, 0, m_length);
    memmove(ret, m_data, m_length);
    return ret;
}


void MemoryWriter::save(const std::string& filename)
{
    if (filename.empty() && m_filepath.empty())
    {
        atError("No file specified, cannot save.");
        setError();
        return;
    }

    if (!filename.empty())
        m_filepath = filename;

    FILE* out = fopen(m_filepath.c_str(), "wb");

    if (!out)
    {
        atError("Unable to open file '%s'", m_filepath.c_str());
        setError();
        return;
    }

    atUint64 done = 0;
    atUint64 blocksize = BLOCKSZ;

    do
    {
        if (blocksize > m_length - done)
            blocksize = m_length - done;

        atInt64 ret = fwrite(m_data + done, 1, blocksize, out);

        if (ret < 0)
        {
            atError("Error writing data to disk");
            setError();
            return;
        }
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
    {
        atError("data cannnot be NULL");
        setError();
        return;
    }

    if (m_position + length > m_length)
    {
        atError("data length exceeds available buffer space");
        setError();
        return;
    }

    memmove(reinterpret_cast<atInt8*>(m_data + m_position), data, length);

    m_position += length;
}

void MemoryCopyWriter::writeUBytes(const atUint8* data, atUint64 length)
{
    if (!data)
    {
        atError("data cannnot be NULL");
        setError();
        return;
    }

    if (m_position + length > m_length)
        resize(m_position + length);

    memmove(reinterpret_cast<atInt8*>(m_data + m_position), data, length);

    m_position += length;
}

void MemoryCopyWriter::resize(atUint64 newSize)
{
    if (newSize < m_length)
    {
        atError("New size cannot be less to the old size.");
        return;
    }

    // Allocate and copy new buffer
    atUint8* newArray = new atUint8[newSize];
    memset(newArray, 0, newSize);

    if (m_dataCopy)
        memmove(newArray, m_dataCopy.get(), m_length);
    m_dataCopy.reset(newArray);

    // Swap the pointer and size out for the new ones.
    m_data = newArray;
    m_length = newSize;
}

} // io
} // Athena
