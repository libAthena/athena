#ifndef MEMORYWRITER_HPP
#define MEMORYWRITER_HPP

#include <string>
#include <memory>
#include <functional>
#include "Athena/IStreamWriter.hpp"

namespace Athena
{
namespace io
{

/*! \class MemoryWriter
 *  \brief A Stream class for writing data to a memory position
 *
 *  A Class for writing binary data to a file or memory stream,
 *  all work is done using a memory buffer, and not written directly to the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
template<typename STLTRAITS = StlTraits>
class MemoryWriter : public IStreamWriter<STLTRAITS>
{
public:

    /*! \brief This constructor references an existing buffer to write to in-place.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    explicit MemoryWriter(atUint8* data, atUint64 length)
        : m_data((atUint8*)data),
          m_length(length),
          m_position(0)
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
                if (position < 0)
                {
                    atError("Position outside stream bounds");
                    IStream::setError();
                    return;
                }

                if ((atUint64)position > m_length)
                {
                    atError("data exceeds available buffer space");
                    IStream::setError();
                    return;
                }

                m_position = position;
                break;

            case SeekOrigin::Current:
                if ((((atInt64)m_position + position) < 0))
                {
                    atError("Position outside stream bounds");
                    IStream::setError();
                    return;
                }

                if (m_position + position > m_length)
                {
                    atError("data exceeds available buffer space");
                    IStream::setError();
                    return;
                }

                m_position += position;
                break;

            case SeekOrigin::End:
                if (((atInt64)m_length - position) < 0)
                {
                    atError("Position outside stream bounds");
                    IStream::setError();
                    return;
                }

                if ((atUint64)position > m_length)
                {
                    atError("data exceeds available buffer space");
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
    inline atUint64 position()    const
    {return m_position;}

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    inline atUint64 length()      const
    {return m_length;}

    inline bool isOpen() const {return true;}

    /*! \brief Sets the buffer to the given one, deleting the current one.<br />
     *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
     *         if that was not the intent.<br />
     *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \param data The new buffer.
     *  \param length The length of the new buffer.
     *  \throw IOException
     */
    void  setData(atUint8* data, atUint64 length)
    {
        m_data = (atUint8*)data;
        m_length = length;
        m_position = 0;
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

    /*! \brief Sets the target file
     *
     *  \param filepath The path to write to.
     */
    inline void setFilepath(const std::string& filepath)
    {m_filepath = filepath;}

    /*! \brief
     * Returns the target file
     */
    inline std::string filepath() const
    {return m_filepath;}


    /*! \brief Saves the file to the specified file.
     *
     *   \param filename If not empty, the filename to save to
     */
    void save(const std::string& filename = "")
    {
        if (filename.empty() && m_filepath.empty())
        {
            atError("No file specified, cannot save.");
            IStream::setError();
            return;
        }

        if (!filename.empty())
            m_filepath = filename;

        FILE* out = fopen(m_filepath.c_str(), "wb");

        if (!out)
        {
            atError("Unable to open file '%s'", m_filepath.c_str());
            IStream::setError();
            return;
        }

        atUint64 done = 0;
        atUint64 blocksize = BLOCKSZ;

        do
        {
            if (blocksize > m_length - done)
                blocksize = m_length - done;

            atInt64 ret = fwrite(m_data + done, 1, blocksize, out);

            if (ret < 0)
            {
                atError("Error writing data to disk");
                IStream::setError();
                return;
            }
            else if (ret == 0)
                break;

            done += blocksize;
        }
        while (done < m_length);

        fclose(out);
    }

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    void writeUBytes(const atUint8* data, atUint64 length)
    {
        if (!data)
        {
            atError("data cannnot be NULL");
            IStream::setError();
            return;
        }

        if (m_position + length > m_length)
        {
            atError("data length exceeds available buffer space");
            IStream::setError();
            return;
        }

        memcpy((atInt8*)(m_data + m_position), data, length);

        m_position += length;
    }

protected:
    MemoryWriter() {}
    atUint8*      m_data;
    atUint64      m_length;
    atUint64      m_position;
    std::string   m_filepath; //!< Path to the target file
};

template<typename STLTRAITS = StlTraits>
class MemoryCopyWriter : public MemoryWriter<STLTRAITS>
{
    using base = MemoryWriter<STLTRAITS>;
public:

    /*! \brief This constructor copies an existing buffer to write to.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    explicit MemoryCopyWriter(atUint8* data=nullptr, atUint64 length=0x10)
    {
        base::m_data = data;
        base::m_length = length;
        base::m_position = 0;

        if (length == 0)
        {
            atError("length cannot be 0");
            IStream::setError();
            return;
        }

        base::m_dataCopy.reset(new atUint8[length]);
        base::m_data = m_dataCopy.get();
        if (data)
            memcpy(base::m_data, data, length);
    }

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    MemoryCopyWriter(const std::string& filename)
        : base(NULL, 0)
    {
        base::m_filepath = filename;
        base::m_length = 0x10;
        base::m_position = 0;
        m_dataCopy.reset(new atUint8[base::m_length]);
        base::m_data = m_dataCopy.get();

        if (!base::m_data)
        {
            atError("Could not allocate memory!");
            IStream::setError();
            return;
        }

        memset(base::m_data, 0, base::m_length);
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
                if (position < 0)
                {
                    atError("Position outside stream bounds");
                    IStream::setError();
                    return;
                }

                if ((atUint64)position > base::m_length)
                    resize(position);

                base::m_position = position;
                break;

            case SeekOrigin::Current:
                if ((((atInt64)base::m_position + position) < 0))
                {
                    atError("Position outside stream bounds");
                    IStream::setError();
                    return;
                }

                if (base::m_position + position > base::m_length)
                    resize(base::m_position + position);

                base::m_position += position;
                break;

            case SeekOrigin::End:
                if (((atInt64)base::m_length - position) < 0)
                {
                    atError("Position outside stream bounds");
                    IStream::setError();
                    return;
                }

                if ((atUint64)position > base::m_length)
                    resize(position);

                base::m_position = base::m_length - position;
                break;
        }
    }

    /*! \brief Sets the buffer to the given one, deleting the current one.<br />
     *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
     *         if that was not the intent.<br />
     *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \param data The new buffer.
     *  \param length The length of the new buffer.
     *  \throw IOException
     */
    void  setData(const atUint8* data, atUint64 length)
    {
        m_dataCopy.reset(new atUint8[length]);
        base::m_data = m_dataCopy.get();
        memcpy(base::m_data, data, length);
        base::m_length = length;
        base::m_position = 0;
    }

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    void writeUBytes(const atUint8* data, atUint64 length)
    {
        if (!data)
        {
            atError("data cannnot be NULL");
            IStream::setError();
            return;
        }

        if (base::m_position + length > base::m_length)
            resize(base::m_position + length);

        memcpy((atInt8*)(base::m_data + base::m_position), data, length);

        base::m_position += length;
    }

protected:
    std::unique_ptr<atUint8[]> m_dataCopy;
private:
    void resize(atUint64 newSize)
    {
        if (newSize < base::m_length)
        {
            atError("New size cannot be less to the old size.");
            return;
        }

        // Allocate and copy new buffer
        atUint8* newArray = new atUint8[newSize];
        memset(newArray, 0, newSize);

        if (m_dataCopy)
            memcpy(newArray, m_dataCopy.get(), base::m_length);
        m_dataCopy.reset(newArray);

        // Swap the pointer and size out for the new ones.
        base::m_data = newArray;
        base::m_length = newSize;
    }
};

}
}

#ifndef MEMORYWRITER_BASE
#define MEMORYWRITER_BASE() \
    private: \
        typedef Athena::io::MemoryWriter base
#endif // BINARYWRITER_BASE

#ifndef MEMORYCOPYWRITER_BASE
#define MEMORYCOPYWRITER_BASE() \
    private: \
        typedef Athena::io::MemoryCopyWriter base
#endif // BINARYWRITER_BASE

#endif // MEMORYWRITER_HPP
