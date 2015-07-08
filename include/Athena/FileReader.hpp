#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include "Athena/IStreamReader.hpp"
#include <string>
#include <stdio.h>

namespace Athena
{
namespace io
{
class FileReader : public IStreamReader
{
public:
    FileReader(const std::string& filename);
    virtual ~FileReader();
    inline const std::string& filename() const
    {return m_filename;}

    void open();
    void close();
    inline bool isOpen() const
    {return m_fileHandle != NULL;}
    bool save();
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
    atUint64 position() const;
    atUint64 length() const;
    atUint64 readUBytesToBuf(void* buf, atUint64 len);

protected:
    std::string  m_filename;
    FILE*        m_fileHandle;
    atUint8      m_currentByte;
};
} // io
} // Athena

#ifndef FILEREADER_BASE
#define FILEREADER_BASE() \
private: \
    typedef Athena::io::FileReader base

#endif // FILEREADER_BASE

#endif // FILESTREAM_HPP
