#include "Athena/FileWriter.hpp"

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
        m_fileHandle = fopen(m_filename.c_str(), "r+b");

    if (!m_fileHandle)
    {
        atError("Unable to open file '%s'", m_filename.c_str());
        return;
    }

    // ensure we're at the beginning of the file
    rewind(m_fileHandle);
}

void FileWriter::close()
{
    if (!m_fileHandle)
    {
        atError("Cannot close an unopened stream");
        return;
    }

    fclose(m_fileHandle);
    m_fileHandle = NULL;
    return;
}

void FileWriter::seek(atInt64 pos, SeekOrigin origin)
{
    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
        atError("Unable to seek in file");
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
        atError("File not open for writing");
        return;
    }

    if (fwrite(data, 1, len, m_fileHandle) != len)
        atError("Unable to write to stream");
}

}
} // Athena
