#include "athena/FileWriter.hpp"
#include "win32_largefilewrapper.h"

namespace athena
{
namespace io
{
FileWriter::FileWriter(std::string_view filename, bool overwrite, bool globalErr)
    : m_fileHandle(0),
      m_bytePosition(0),
      m_globalErr(globalErr)
{
    m_filename = utility::utf8ToWide(filename);
    open(overwrite);
}

FileWriter::FileWriter(std::wstring_view filename, bool overwrite, bool globalErr)
    : m_fileHandle(0),
      m_bytePosition(0),
      m_globalErr(globalErr)
{
    m_filename = filename;
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
    {
        m_fileHandle = CreateFileW(m_filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE,
                                   nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    else
    {
        m_fileHandle = CreateFileW(m_filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE,
                                   nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    }

    if (m_fileHandle == INVALID_HANDLE_VALUE)
    {
        m_fileHandle = 0;
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

    CloseHandle(m_fileHandle);
    m_fileHandle = 0;
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

    LARGE_INTEGER li;
    li.QuadPart = pos;
    if (!SetFilePointerEx(m_fileHandle, li, nullptr, DWORD(origin)))
    {
        if (m_globalErr)
            atError("Unable to seek in file");
        setError();
    }
}

atUint64 FileWriter::position() const
{
    LARGE_INTEGER li = {};
    LARGE_INTEGER res;
    SetFilePointerEx(m_fileHandle, li, &res, FILE_CURRENT);
    return res.QuadPart;
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

    DWORD ret = 0;
    WriteFile(m_fileHandle, data, len, &ret, nullptr);
    if (ret != len)
    {
        if (m_globalErr)
            atError("Unable to write to stream");
        setError();
    }
}

}
} // Athena
