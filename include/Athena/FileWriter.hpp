#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#include "Athena/IStreamWriter.hpp"
#include <stdio.h>

namespace Athena
{
namespace io
{
class FileWriter : public IStreamWriter
{
public:
    FileWriter(const std::string& filename, bool overwrite = true);
#if _WIN32
    FileWriter(const std::wstring& filename, bool overwrite = true);
#endif
    virtual ~FileWriter();

    void open(bool overwrite = true);
    void close();
    inline bool isOpen() const
    {return m_fileHandle != NULL;}
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
    atUint64 position() const;
    atUint64 length() const;
    void writeUBytes(const atUint8* data, atUint64 len);

private:
    std::string  m_filename;
#if _WIN32
    std::wstring m_wfilename;
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
