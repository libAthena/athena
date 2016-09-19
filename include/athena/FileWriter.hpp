#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <stdio.h>
#endif
#include "athena/IStreamWriter.hpp"

namespace athena
{
namespace io
{
class FileWriter : public IStreamWriter
{
public:
    FileWriter(const std::string& filename, bool overwrite = true, bool globalErr=true);
    FileWriter(const std::wstring& filename, bool overwrite = true, bool globalErr=true);
    virtual ~FileWriter();

    inline std::string filename() const
    {
#if _WIN32
        return utility::wideToUtf8(m_filename);
#else
        return m_filename;
#endif
    }
    inline std::wstring wfilename() const
    {
#if _WIN32
        return m_filename;
#else
        return utility::utf8ToWide(m_filename);
#endif
    }

    void open(bool overwrite = true);
    void close();
    inline bool isOpen() const
    {return m_fileHandle != 0;}
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
    atUint64 position() const;
    atUint64 length() const;
    void writeUBytes(const atUint8* data, atUint64 len);

#if _WIN32
    using HandleType = HANDLE;
#else
    using HandleType = FILE*;
#endif

    HandleType _fileHandle() {return m_fileHandle;}
private:
#if _WIN32
    std::wstring m_filename;
#else
    std::string  m_filename;
#endif
    HandleType   m_fileHandle;
    atUint8      m_currentByte;
    atUint64     m_bytePosition;
    bool         m_globalErr;
};
}
} // Athena

#ifndef FILEWRITER_BASE
#define FILEWRITER_BASE() \
private: \
    typedef athena::io::FileWriter base;

#endif // FILEWRITER_BASE
#endif // FILEWRITER_HPP
