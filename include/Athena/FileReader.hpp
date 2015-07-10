#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include <string>
#include <memory>
#include <stdio.h>
#include "Athena/IStreamReader.hpp"

namespace Athena
{
namespace io
{
class FileReader : public IStreamReader
{
public:
    FileReader(const std::string& filename, atInt32 cacheSize = (32 * 1024));
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

    void setCacheSize(const atInt32 blockSize);
protected:
    std::string  m_filename;
    FILE*        m_fileHandle;
    std::unique_ptr<atUint8[]>    m_cacheData;
    atInt32     m_blockSize;
    atInt32      m_curBlock;
    atUint64     m_offset;
};
} // io
} // Athena

#ifndef FILEREADER_BASE
#define FILEREADER_BASE() \
private: \
    typedef Athena::io::FileReader base

#endif // FILEREADER_BASE

#endif // FILESTREAM_HPP
