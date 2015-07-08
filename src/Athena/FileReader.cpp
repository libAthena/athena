#include "Athena/FileReader.hpp"
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
FileReader::FileReader(const std::string& filename)
    : m_filename(filename),
      m_fileHandle(NULL)
{
    open();
}

FileReader::~FileReader()
{
    if (isOpen())
        close();
}

void FileReader::open()
{
    m_fileHandle = fopen(m_filename.c_str(), "rb");

    if (!m_fileHandle)
        THROW_FILE_NOT_FOUND_EXCEPTION(m_filename);

    // ensure we're at the beginning of the file
    rewind(m_fileHandle);
}

void FileReader::close()
{
    if (!m_fileHandle)
        THROW_INVALID_OPERATION_EXCEPTION("Cannot close an unopened stream");

    fclose(m_fileHandle);
    m_fileHandle = NULL;
    return;
}

void FileReader::seek(atInt64 pos, SeekOrigin origin)
{
    if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
        THROW_INVALID_OPERATION_EXCEPTION("Unable to seek in file");
}

atUint64 FileReader::position() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open");

    return ftello64(m_fileHandle);
}

atUint64 FileReader::length() const
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open");

    return utility::fileSize(m_filename);
}

atUint64 FileReader::readUBytesToBuf(void* buf, atUint64 len)
{
    if (!isOpen())
        THROW_INVALID_OPERATION_EXCEPTION_RETURN(0, "File not open for reading");

    return fread(buf, 1, len, m_fileHandle);
}

} // io
} // Athena
