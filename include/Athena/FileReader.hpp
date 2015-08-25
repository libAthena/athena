#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include <string>
#include <memory>
#include <stdio.h>
#include "Athena/IStreamReader.hpp"

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
class FileReader : public IStreamReader<STLTRAITS>
{
public:
    inline const typename STLTRAITS::String& filename() const
    {return m_filename;}

    FileReader(const typename STLTRAITS::String& filename, atInt32 cacheSize = (32 * 1024))
        : m_filename(filename),
          m_fileHandle(nullptr),
          m_cacheData(nullptr),
          m_offset(0)
    {
        open();
        setCacheSize(cacheSize);
    }

    #if _WIN32
    FileReader::FileReader(const typename STLTRAITS::WString& filename, atInt32 cacheSize = (32 * 1024))
        : m_wfilename(filename),
        m_fileHandle(nullptr),
        m_cacheData(nullptr),
        m_offset(0)
    {
        open();
        setCacheSize(cacheSize);
    }
    #endif

    virtual ~FileReader()
    {
        if (isOpen())
            close();
    }

    void open()
    {
    #if _WIN32
        if (m_wfilename.size())
            m_fileHandle = _wfopen(m_wfilename.c_str(), L"rb");
        else
            m_fileHandle = fopen(m_filename.c_str(), "rb");
    #else
        m_fileHandle = fopen(m_filename.c_str(), "rb");
    #endif

        if (!m_fileHandle)
        {
            atError("File not found '%s'", m_filename.c_str());
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

    inline bool isOpen() const {return m_fileHandle != NULL;}

    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current)
    {
        // check block position
        if (m_blockSize > 0)
        {
            atUint64 oldOff = m_offset;
            switch(origin)
            {
                case SeekOrigin::Begin:
                    m_offset = pos;
                    break;
                case SeekOrigin::Current:
                    m_offset += pos;
                    break;
                case SeekOrigin::End:
                    m_offset = length() - pos;
                    break;
            }
            if (m_offset > length())
            {
                oldOff = m_offset;
                atError("Unable to seek in file");
                return;
            }

            size_t block = m_offset / m_blockSize;
            if (block != m_curBlock)
            {
                fseeko64(m_fileHandle, block * m_blockSize, SEEK_SET);
                fread(m_cacheData.get(), 1, m_blockSize, m_fileHandle);
                m_curBlock = (atInt32)block;
            }
        }
        else if (fseeko64(m_fileHandle, pos, (int)origin) != 0)
            atError("Unable to seek in file");
    }

    atUint64 position() const
    {
        if (!isOpen())
        {
            atError("File not open");
            return 0;
        }

        if (m_blockSize > 0)
            return m_offset;
        else
            return ftello64(m_fileHandle);
    }

    atUint64 length() const
    {
        if (!isOpen())
        {
            atError("File not open");
            return 0;
        }

        return utility::fileSize(m_filename);
    }

    atUint64 readUBytesToBuf(void* buf, atUint64 len)
    {
        if (!isOpen())
        {
            atError("File not open for reading");
            IStream::setError();
            return 0;
        }

        if (m_blockSize <= 0)
            return fread(buf, 1, len, m_fileHandle);
        else
        {
            size_t block = m_offset / m_blockSize;
            atUint64 cacheOffset = m_offset % m_blockSize;
            atUint64 cacheSize;
            atUint64 rem = len;
            atUint8* dst = (atUint8*)buf;

            while (rem)
            {
                if (block != m_curBlock)
                {
                    fseeko64(m_fileHandle, block * m_blockSize, SEEK_SET);
                    fread(m_cacheData.get(), 1, m_blockSize, m_fileHandle);
                    m_curBlock = (atInt32)block;
                }

                cacheSize = rem;
                if (cacheSize + cacheOffset > m_blockSize)
                    cacheSize = m_blockSize - cacheOffset;

                memcpy(dst, m_cacheData.get() + cacheOffset, cacheSize);
                dst += cacheSize;
                rem -= cacheSize;
                cacheOffset = 0;
                ++block;
            }
            m_offset += len;
            return dst - (atUint8*)buf;
        }
    }

    void setCacheSize(const atInt32 blockSize)
    {
        m_blockSize = blockSize;

        if (m_blockSize > length())
            m_blockSize = (atInt32)length();

        m_curBlock = -1;
        if (m_blockSize > 0)
            m_cacheData.reset(new atUint8[m_blockSize]);
    }

protected:
    typename STLTRAITS::String m_filename;
#if _WIN32
    typename STLTRAITS::WString m_wfilename;
#endif
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
