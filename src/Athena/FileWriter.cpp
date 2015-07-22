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

namespace Athena
{
namespace io
{
FileWriter::FileWriter(const std::string& filename, bool overwrite)
    : m_filename(filename),
      m_fileHandle(NULL),
      m_bytePosition(0)
{
    open(overwrite);
}

#if _WIN32
FileWriter::FileWriter(const std::wstring& filename, bool overwrite)
    : m_wfilename(filename),
    m_fileHandle(NULL),
    m_bytePosition(0)
{
    open(overwrite);
}
#endif

FileWriter::~FileWriter()
{
    if (isOpen())
        close();
}

void FileWriter::open(bool overwrite)
{
#if _WIN32
    if (m_wfilename.size())
    {
        if (overwrite)
            m_fileHandle = _wfopen(m_wfilename.c_str(), L"w+b");
        else
            m_fileHandle = _wfopen(m_wfilename.c_str(), L"r+b");
    }
    else
    {
        if (overwrite)
            m_fileHandle = fopen(m_filename.c_str(), "w+b");
        else
            m_fileHandle = fopen(m_filename.c_str(), "r+b");
    }
#else
    if (overwrite)
        m_fileHandle = fopen(m_filename.c_str(), "w+b");
    else
        m_fileHandle = fopen(m_filename.c_str(), "r+b"); 
#endif



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

void FileWriter::seek(atInt64 pos, SeekOrigin origin)
{
    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
        THROW_IO_EXCEPTION("Unable to seek in file");
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
        THROW_INVALID_OPERATION_EXCEPTION("File not open for writing");

    if (fwrite(data, 1, len, m_fileHandle) != len)
        THROW_IO_EXCEPTION("Unable to write to stream");
}

}
} // Athena
