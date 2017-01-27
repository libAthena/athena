#include "athena/FileWriter.hpp"

#if __APPLE__  || __FreeBSD__
#include "osx_largefilewrapper.h"
#elif GEKKO
#include "gekko_support.h"
#include "osx_largefilewrapper.h"
#endif

namespace athena
{
namespace io
{
FileWriter::FileWriter(const std::string& filename, bool overwrite, bool globalErr)
    : m_fileHandle(NULL),
      m_bytePosition(0),
      m_globalErr(globalErr)
{
#if _WIN32
    m_filename = utility::utf8ToWide(filename);
#else
    m_filename = filename;
#endif
    open(overwrite);
}

FileWriter::FileWriter(const std::wstring& filename, bool overwrite, bool globalErr)
    : m_fileHandle(NULL),
      m_bytePosition(0),
      m_globalErr(globalErr)
{
#if _WIN32
    m_filename = filename;
#else
    m_filename = utility::wideToUtf8(filename);
#endif
    open(overwrite);
}

FileWriter::~FileWriter()
{
    if (isOpen())
        close();
}

void FileWriter::open(bool overwrite)
{
    if (overwrite)
        m_fileHandle = fopen(m_filename.c_str(), "w+b");
    else
    {
        m_fileHandle = fopen(m_filename.c_str(), "a+b");
        if (m_fileHandle)
        {
            fclose(m_fileHandle);
            m_fileHandle = fopen(m_filename.c_str(), "r+b");
        }
    }

    if (!m_fileHandle)
    {
        if (m_globalErr)
            atError("Unable to open file '%s'", filename().c_str());
        setError();
        return;
    }

    // reset error
    m_hasError = false;
}

void FileWriter::close()
{
    if (!m_fileHandle)
    {
        if (m_globalErr)
            atError("Cannot close an unopened stream");
        setError();
        return;
    }

    fclose(m_fileHandle);
    m_fileHandle = NULL;
}

void FileWriter::seek(atInt64 pos, SeekOrigin origin)
{
    if (!isOpen())
    {
        if (m_globalErr)
            atError("Unable to seek in file, not open");
        setError();
        return;
    }

    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
    {
        if (m_globalErr)
            atError("Unable to seek in file");
        setError();
    }
}

atUint64 FileWriter::position() const
{
    return ftello64(m_fileHandle);
}

atUint64 FileWriter::length() const
{
    return utility::fileSize(m_filename);
}

void FileWriter::writeUBytes(const atUint8* data, atUint64 len)
{
    if (!isOpen())
    {
        if (m_globalErr)
            atError("File not open for writing");
        setError();
        return;
    }

    if (fwrite(data, 1, len, m_fileHandle) != len)
    {
        if (m_globalErr)
            atError("Unable to write to stream");
        setError();
    }
}

void TransactionalFileWriter::seek(atInt64 pos, SeekOrigin origin)
{
    switch (origin)
    {
    case SeekOrigin::Begin:
        m_position = pos;
        break;
    case SeekOrigin::Current:
        m_position += pos;
        break;
    case SeekOrigin::End:
        break;
    }
}

void TransactionalFileWriter::writeUBytes(const atUint8* data, atUint64 len)
{
    atUint64 neededSz = m_position + len;
    if (neededSz > m_deferredBuffer.size())
    {
        m_deferredBuffer.reserve(neededSz * 2);
        m_deferredBuffer.resize(neededSz);
    }

    memmove(m_deferredBuffer.data() + m_position, data, len);
    m_position += len;
}

}
} // Athena
