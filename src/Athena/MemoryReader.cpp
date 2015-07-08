#include "Athena/MemoryReader.hpp"
#include "Athena/IOException.hpp"
#include "Athena/FileNotFoundException.hpp"
#include "Athena/InvalidDataException.hpp"
#include "Athena/InvalidOperationException.hpp"

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
    : m_data(data),
      m_length(length),
      m_position(0)
{
    if (!data)
        THROW_INVALID_DATA_EXCEPTION("data cannot be NULL");

    if (length == 0)
        THROW_INVALID_OPERATION_EXCEPTION("length cannot be 0");
}

MemoryCopyReader::MemoryCopyReader(const atUint8* data, atUint64 length)
    : MemoryReader(data, length)
{
    if (!data)
        THROW_INVALID_DATA_EXCEPTION("data cannot be NULL");

    if (length == 0)
        THROW_INVALID_OPERATION_EXCEPTION("length cannot be 0");

    m_dataCopy.reset(new atUint8[m_length]);
    m_data = m_dataCopy.get();
    memcpy(m_dataCopy.get(), data, m_length);
}

void MemoryReader::seek(atInt64 position, SeekOrigin origin)
{
    switch (origin)
    {
        case SeekOrigin::Begin:
            if ((position < 0 || (atInt64)position > (atInt64)m_length))
                THROW_IO_EXCEPTION("Position %0.8X outside stream bounds ", position);

            m_position = position;
            break;

        case SeekOrigin::Current:
            if ((((atInt64)m_position + position) < 0 || (m_position + position) > m_length))
                THROW_IO_EXCEPTION("Position %0.8X outside stream bounds ", position);

            m_position += position;
            break;

        case SeekOrigin::End:
            if ((((atInt64)m_length - position < 0) || (m_length - position) > m_length))
                THROW_IO_EXCEPTION("Position %0.8X outside stream bounds ", position);

            m_position = m_length - position;
            break;
    }
}

void MemoryReader::setData(const atUint8* data, atUint64 length)
{
    m_data = (atUint8*)data;
    m_length = length;
    m_position = 0;
}

void MemoryCopyReader::setData(const atUint8* data, atUint64 length)
{
    m_dataCopy.reset(new atUint8[length]);
    m_data = m_dataCopy.get();
    memcpy(m_dataCopy.get(), data, length);
    m_length = length;
    m_position = 0;
}

atUint8* MemoryReader::data() const
{
    atUint8* ret = new atUint8[m_length];
    memset(ret, 0, m_length);
    memcpy(ret, m_data, m_length);
    return ret;
}

atUint64 MemoryReader::readUBytesToBuf(void* buf, atUint64 length)
{
    if (m_position + length > m_length)
        THROW_IO_EXCEPTION_RETURN(0, "Position %0.8X outside stream bounds ", m_position);

    memcpy(buf, (const atUint8*)(m_data + m_position), length);
    m_position += length;
    return length;
}

void MemoryCopyReader::loadData()
{
    FILE* in;
    atUint64 length;
    in = fopen(m_filepath.c_str(), "rb");

    if (!in)
        THROW_FILE_NOT_FOUND_EXCEPTION(m_filepath);

    rewind(in);

    length = utility::fileSize(m_filepath);
    m_dataCopy.reset(new atUint8[length]);
    m_data = m_dataCopy.get();

    atUint64 done = 0;
    atUint64 blocksize = BLOCKSZ;

    do
    {
        if (blocksize > length - done)
            blocksize = length - done;

        atInt64 ret = fread(m_dataCopy.get() + done, 1, blocksize, in);

        if (ret < 0)
            THROW_IO_EXCEPTION("Error reading data from disk");
        else if (ret == 0)
            break;

        done += ret;


    }
    while (done < length);

    fclose(in);
    m_length = length;
    m_position = 0;
}

}
}
