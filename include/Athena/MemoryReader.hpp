#ifndef MEMORYREADER_HPP
#define MEMORYREADER_HPP

#include <string>
#include <memory>
#include <functional>
#include "Athena/IStreamReader.hpp"

namespace Athena
{
namespace io
{
/*! \class MemoryReader
 *  \brief A Stream class for reading data from a memory position
 *
 *  A Class for reading binary data from a file or memory stream,
 *  all work is done using a memory buffer, and not read directly from the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
template<typename STLTRAITS = StlTraits>
class MemoryReader : public IStreamReader<STLTRAITS>
{
    using base = IStreamReader<STLTRAITS>;
public:
    virtual ~MemoryReader()
    {
        if (m_owns)
            delete[] m_data;
    }

    /*! \brief This constructor references an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     *   \param takeOwnership Memory will be freed with the reader if set
     */
    MemoryReader(const atUint8* data, atUint64 length, bool takeOwnership=false)
        : m_data(data),
          m_length(length),
          m_position(0),
          m_owns(takeOwnership)
    {
        if (!data)
        {
            atError("data cannot be NULL");
            IStream::setError();
            return;
        }

        if (length == 0)
        {
            atError("length cannot be 0");
            IStream::setError();
            return;
        }
    }

    /*! \brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  \param position where in the buffer to seek
     *  \param origin The Origin to seek \sa SeekOrigin
     */
    void seek(atInt64 position, SeekOrigin origin = SeekOrigin::Current)
    {
        switch (origin)
        {
            case SeekOrigin::Begin:
                if ((position < 0 || (atInt64)position > (atInt64)m_length))
                {
                    atError("Position %0.8X outside stream bounds ", position);
                    IStream::setError();
                    return;
                }

                m_position = position;
                break;

            case SeekOrigin::Current:
                if ((((atInt64)m_position + position) < 0 || (m_position + position) > m_length))
                {
                    atError("Position %0.8X outside stream bounds ", position);
                    IStream::setError();
                    return;
                }

                m_position += position;
                break;

            case SeekOrigin::End:
                if ((((atInt64)m_length - position < 0) || (m_length - position) > m_length))
                {
                    atError("Position %0.8X outside stream bounds ", position);
                    IStream::setError();
                    return;
                }

                m_position = m_length - position;
                break;
        }
    }

    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    inline atUint64 position() const {return m_position;}

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    inline atUint64 length() const {return m_length;}


    /*! \brief Sets the buffer to the given one, deleting the current one.<br />
     *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
     *         if that was not the intent.<br />
     *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \param data The new buffer.
     *  \param length The length of the new buffer.
     *  \param takeOwnership Memory will be freed with the reader if set
     *  \throw IOException
     */
    void setData(const atUint8* data, atUint64 length, bool takeOwnership=false)
    {
        if (m_owns && m_data)
            delete[] m_data;
        m_data = (atUint8*)data;
        m_length = length;
        m_position = 0;
        m_owns = takeOwnership;
    }


    /*! \brief Returns a copy of the current buffer.<br />
     *         Changes to the copy do not affect the buffer so it's perfectly safe to
     *         directly edit the buffer and use setData to set the new information.<br />
     *         However once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \return Uint8* The copy of the buffer.
     */
    atUint8* data() const
    {
        atUint8* ret = new atUint8[m_length];
        memset(ret, 0, m_length);
        memcpy(ret, m_data, m_length);
        return ret;
    }

    /*! \brief Reads a specified number of bytes to user-allocated buffer
     *  \param buf User-allocated buffer pointer
     *  \param length Length to read
     *  \return Number of bytes read
     */
    atUint64 readUBytesToBuf(void* buf, atUint64 length)
    {
        if (m_position + length > m_length)
        {
            atError("Position %0.8X outside stream bounds ", m_position);
            IStream::setError();
            return 0;
        }

        memcpy(buf, (const atUint8*)(m_data + m_position), length);
        m_position += length;
        return length;
    }

protected:
    const atUint8*   m_data;
    atUint64         m_length;
    atUint64         m_position;
    bool             m_owns;
};

template<typename STLTRAITS = StlTraits>
class MemoryCopyReader : public MemoryReader<STLTRAITS>
{
    using base = MemoryReader<STLTRAITS>;
public:
    /*! \brief This constructor copies an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    MemoryCopyReader(const atUint8* data, atUint64 length)
    : base(data, length, false)
    {
        if (!data)
        {
            atError("data cannot be NULL");
            IStream::setError();
            return;
        }

        if (length == 0)
        {
            atError("length cannot be 0");
            IStream::setError();
            return;
        }

        m_dataCopy.reset(new atUint8[base::m_length]);
        base::m_data = m_dataCopy.get();
        memcpy(m_dataCopy.get(), data, base::m_length);
    }

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    MemoryCopyReader(const std::string& filename)
    : base(NULL, 0),
      m_filepath(filename)
    {loadData();}

    void setData(const atUint8* data, atUint64 length)
    {
        m_dataCopy.reset(new atUint8[length]);
        base::m_data = m_dataCopy.get();
        memcpy(m_dataCopy.get(), data, length);
        base::m_length = length;
        base::m_position = 0;
    }

protected:
    void loadData()
    {
        FILE* in;
        atUint64 length;
        in = fopen(m_filepath.c_str(), "rb");

        if (!in)
        {
            atError("Unable to open file '%s'", m_filepath.c_str());
            IStream::setError();
            return;
        }

        rewind(in);

        length = utility::fileSize(m_filepath);
        m_dataCopy.reset(new atUint8[length]);
        base::m_data = m_dataCopy.get();

        atUint64 done = 0;
        atUint64 blocksize = BLOCKSZ;

        do
        {
            if (blocksize > length - done)
                blocksize = length - done;

            atInt64 ret = fread(m_dataCopy.get() + done, 1, blocksize, in);

            if (ret < 0)
            {
                atError("Error reading data from disk");
                IStream::setError();
                return;
            }
            else if (ret == 0)
                break;

            done += ret;


        }
        while (done < length);

        fclose(in);
        base::m_length = length;
        base::m_position = 0;
    }
    std::unique_ptr<atUint8[]> m_dataCopy;
    std::string   m_filepath; //!< Path to the target file
};

} // io
} // Athena

#ifndef MEMORYREADER_BASE
#define MEMORYREADER_BASE() \
private: \
    typedef Athena::io::MemoryReader base

#endif // MEMORYREADER_BASE

#ifndef MEMORYCOPYREADER_BASE
#define MEMORYCOPYREADER_BASE() \
private: \
    typedef Athena::io::MemoryCopyReader base

#endif // MEMORYCOPYREADER_BASE

#endif // MEMORYREADER_HPP
