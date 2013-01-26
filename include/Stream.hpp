#ifndef __STREAM_HPP__
#define __STREAM_HPP__

#include "Types.hpp"

/*! \class Stream
 *  \brief Stream is the main class all streams inherit from
 *
 *  The Stream class contains the memory buffer, buffer size, and current position.<br />
 *  It provides basic functionality such as reading or writing bits and bytes
 *  The user has the ability to specify the Endianness of the Stream as well
 *  And all inheriting classes are required to take that into account to be accepted
 *  as a valid stream. <br />
 *  <br />
 *  Stream can also be used by itself though it's not advised as it's not feature filled.<br />
 *  It's highly suggested to use of the more advanced Streams such as BinaryReader.
 */
class Stream
{
public:

    /*! \enum Endian
     *  \brief Allows the user to specify the Endianness of the stream buffer.<br />
     *         The proper actions are automatically taken depending on platform and
     *         buffer settings
     */
    enum Endian
    {
        LittleEndian, //!< Specifies that the Stream is Little Endian (LSB)
        BigEndian //!< Specifies that the Stream is Big Endian (MSB)
    };

    /*! \enum SeekOrigin
     *  \brief Specifies how to seek in a stream.
     */
    enum SeekOrigin
    {
        Beginning = 0, //!< Tells the Stream to seek from the Beginning of the buffer.
        Current,       //!< Tells the Stream to seek from the Current position of the buffer.
        End            //!< Tells the Stream to seek from the End of the buffer.
    };


    /*! \brief The default constructor
     */
    Stream();
    /*! \brief This constructor takes an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    Stream(const Uint8* bytes, Uint64 length);
    /*! \brief This constructor creates a buffer from the given length.
     *
     *   \param length The length of the existing buffer
     */
    Stream(Int64 length);
    /*! \brief This constructor takes an existing Stream to read from.
     *
     *  \param stream The stream to read data from
     */
    Stream(Stream* stream);
    virtual ~Stream();

    /*! \brief Writes a bit at the current position and advances the position by one bit.
     * \param val the value to write
     * \throw IOException
     */
    virtual void writeBit(bool val);

    /*! \brief Writes a byte at the current position and advances the position by one byte.
     * \param byte The value to write
     * \throw IOException
     */
    virtual void writeByte(Int8 byte);

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     * \throw IOException
     */
    virtual void writeBytes(Int8* data, Int64 length);

    /*! \brief Reads a bit at the current position and advances the current position
     *
     * \return bool The value at the current position
     * \throw IOException
     */
    virtual bool readBit();
    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Int8 The value at the current position
     * \throw IOException
     */
    virtual Int8 readByte();

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Int8* The buffer at the current position from the given length.
     * \throw IOException
     */
    virtual Int8* readBytes(Int64 length);

    /*! \brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  \param position where in the buffer to seek
     *  \param origin The Origin to seek \sa SeekOrigin
     *  \throw IOException
     */
    void seek(Int64 position, SeekOrigin origin = Current);

    /*! \brief Resizes the buffer to the given length.<br />
     *         The new size must be greater than the current length to avoid loss of data.
     *  \param newSize The amount to resize by.
     *  \throw IOException
     */
    void resize(Uint64 newSize);

    /*! \brief Sets the buffer to the given one, deleting the current one.<br />
     *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
     *         if that was not the intent.
     *  \param data The new buffer.
     *  \param length The length of the new buffer.
     *  \throw IOException
     */
    void  setData(const Uint8* data, Uint64 length);


    /*! \brief Returns a copy of the current buffer.<br />
     *         Changes to the copy do not affect the buffer so it's perfectly safe to
     *         directly edit the buffer and use setData to set the new information.<br />
     *         However once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \return Uint8* The copy of the buffer
     */
    Uint8* data() const;

    /*! \brief Returns the length of the Stream.
    Int64 length();
    Int64 position();
    bool atEnd();
    void setAutoResizing(bool val);
    bool autoResizing() const;

    virtual bool isOpenForReading();
    virtual bool isOpenForWriting();

    void setEndianess(Endian endian);
    Endian endianness() const;
    bool isBigEndian() const;
    bool isLittleEndian() const;
protected:
    Uint32  m_bitPosition;
    Uint64  m_position;
    Uint64  m_length;
    Endian  m_endian;
    Uint8*  m_data;
    bool    m_autoResize;
};

#endif // __STREAM_HPP__
