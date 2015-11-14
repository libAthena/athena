#include "Athena/FileWriter.hpp"

#if _WIN32
#include "win32_largefilewrapper.h"
#elif __APPLE__
#include "osx_largefilewrapper.h"
#elif GEKKO
#include "gekko_support.h"
#include "osx_largefilewrapper.h"
#endif

namespace Athena
{
namespace io
{
FileWriter::FileWriter(const std::string& filename, bool overwrite)
    : m_fileHandle(NULL),
      m_bytePosition(0)
{
#if _WIN32
    m_filename = utility::utf8ToWide(filename);
#else
    m_filename = filename;
#endif
    open(overwrite);
}

FileWriter::FileWriter(const std::wstring& filename, bool overwrite)
    : m_fileHandle(NULL),
      m_bytePosition(0)
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
#if _WIN32
    if (overwrite)
        m_fileHandle = _wfopen(m_filename.c_str(), L"w+b");
    else
        m_fileHandle = _wfopen(m_filename.c_str(), L"r+b");
#else
    if (overwrite)
        m_fileHandle = fopen(m_filename.c_str(), "w+b");
    else
        m_fileHandle = fopen(m_filename.c_str(), "r+b");
#endif

    if (!m_fileHandle)
    {
        atError("Unable to open file '%s'", filename());
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
        atError("Cannot close an unopened stream");
        setError();
        return;
    }

    fclose(m_fileHandle);
    m_fileHandle = NULL;
    return;
}

void FileWriter::seek(atInt64 pos, SeekOrigin origin)
{
    if (!isOpen())
    {
        atError("Unable to seek in file, not open");
        setError();
        return;
    }

    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
    {
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
#if _WIN32
    return utility::fileSize(utility::wideToUtf8(m_filename));
#else
    return utility::fileSize(m_filename);
#endif
}

void FileWriter::writeUBytes(const atUint8* data, atUint64 len)
{
    if (!isOpen())
    {
        atError("File not open for writing");
        setError();
        return;
    }

    if (fwrite(data, 1, len, m_fileHandle) != len)
    {
        atError("Unable to write to stream");
        setError();
    }
}

}
} // Athena
