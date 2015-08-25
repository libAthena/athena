#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#include "Athena/IStreamWriter.hpp"
#include <stdio.h>

#if _WIN32
#include "win32_largefilewrapper.h"
#elif __APPLE__
#include "osx_largefilewrapper.h"
#endif

namespace Athena
{
namespace io
{
template<typename STLTRAITS = StlTraits>
class FileWriter : public IStreamWriter<STLTRAITS>
{
public:
    inline bool isOpen() const {return m_fileHandle != NULL;}

    FileWriter(const std::string& filename, bool overwrite = true)
        : m_filename(filename),
          m_fileHandle(NULL),
          m_bytePosition(0)
    {
        open(overwrite);
    }

    #if _WIN32
    FileWriter(const std::wstring& filename, bool overwrite = true)
        : m_wfilename(filename),
        m_fileHandle(NULL),
        m_bytePosition(0)
    {
        open(overwrite);
    }
    #endif

    virtual ~FileWriter()
    {
        if (isOpen())
            close();
    }

    void open(bool overwrite = true)
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
        {
            atError("Unable to open file '%s'", m_filename.c_str());
            IStream::setError();
            return;
        }

        // ensure we're at the beginning of the file
        rewind(m_fileHandle);
    }

    void close()
    {
        if (!m_fileHandle)
        {
            atError("Cannot close an unopened stream");
            IStream::setError();
            return;
        }

        fclose(m_fileHandle);
        m_fileHandle = NULL;
        return;
    }

    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current)
    {
        if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
        {
            atError("Unable to seek in file");
            IStream::setError();
        }
    }

    atUint64 position() const
    {
        return ftello64(m_fileHandle);
    }

    atUint64 length() const
    {
        return utility::fileSize(m_filename);
    }

    void writeUBytes(const atUint8* data, atUint64 len)
    {
        if (!isOpen())
        {
            atError("File not open for writing");
            IStream::setError();
            return;
        }

        if (fwrite(data, 1, len, m_fileHandle) != len)
        {
            atError("Unable to write to stream");
            IStream::setError();
        }
    }

private:
    typename STLTRAITS::String  m_filename;
#if _WIN32
    typename STLTRAITS::WString m_wfilename;
#endif
    FILE*        m_fileHandle;
    atUint8      m_currentByte;
    atUint64     m_bytePosition;
};
}
} // Athena

#ifndef FILEWRITER_BASE
#define FILEWRITER_BASE() \
private: \
    typedef Athena::io::FileWriter base;

#endif // FILEWRITER_BASE
#endif // FILEWRITER_HPP
