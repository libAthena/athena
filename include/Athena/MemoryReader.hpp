#ifndef MEMORYREADER_HPP
#define MEMORYREADER_HPP

#include "Athena/IStreamReader.hpp"
#include <string>
#include <functional>

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
class MemoryReader : public IStreamReader
{
public:
    ~MemoryReader();

    /*! \brief This constructor takes an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    MemoryReader(const atUint8* data, atUint64 length);

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    MemoryReader(const std::string& filename);

    /*! \brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  \param position where in the buffer to seek
     *  \param origin The Origin to seek \sa SeekOrigin
     */
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);

    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    inline atUint64 position() const
    {return m_position;}

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    inline atUint64 length() const
    {return m_length;}


    /*! \brief Sets the buffer to the given one, deleting the current one.<br />
     *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
     *         if that was not the intent.<br />
     *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \param data The new buffer.
     *  \param length The length of the new buffer.
     *  \throw IOException
     */
    void setData(const atUint8* data, atUint64 length);


    /*! \brief Returns a copy of the current buffer.<br />
     *         Changes to the copy do not affect the buffer so it's perfectly safe to
     *         directly edit the buffer and use setData to set the new information.<br />
     *         However once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \return Uint8* The copy of the buffer.
     */
    atUint8* data() const;

    atUint64 readUBytesToBuf(void* buf, atUint64 len);

protected:
    void loadData();
    atUint8*      m_data;
    atUint64      m_length;
    std::string   m_filepath; //!< Path to the target file
    atUint64      m_position;
};

} // io
} // Athena

#ifndef MEMORYREADER_BASE
#define MEMORYREADER_BASE() \
private: \
    typedef Athena::io::MemoryReader base

#endif // MEMORYREADER_BASE

#endif // MEMORYREADER_HPP
