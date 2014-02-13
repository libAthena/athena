// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>
#ifndef __STREAM_HPP__
#define __STREAM_HPP__

#include "Types.hpp"

namespace zelda
{
namespace io
{

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
    //! \brief Default buffer block size.
    static const Uint32 BLOCKSZ;

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
     *   \param bytes The existing buffer
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

    /*! \brief The destructor cleans up memory and sets everything back
     *         to the default settings.
     */
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
    virtual void writeUByte(Uint8 byte);
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
    virtual void writeUBytes(Uint8* data, Int64 length);

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

    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Uint8 The value at the current position
     * \throw IOException
     */
    virtual Uint8 readUByte();

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Uint8* The buffer at the current position from the given length.
     * \throw IOException
     */
    virtual Uint8* readUBytes(Int64 length);

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
     *         if that was not the intent.<br />
     *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
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
     *  \return Uint8* The copy of the buffer.
     */
    Uint8* data() const;

    /*! \brief Returns the length of the Stream.
     *
     *  \return Int64 The length of the stream.
     */
    Int64 length() const;

    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    Int64 position() const;

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    bool atEnd() const;

    /*! \brief Sets whether the Stream resizes when the at the end of the buffer.
     *
     *  \param val True for resizing; False for no resizing.
     */
    void setAutoResizing(bool val);

    /*! \brief Retuns whether or not the Stream currenty autoresizes.
     *
     *  \return True for resizing; False otherwise.
     */
    bool autoResizing() const;


    /*! \brief Retuns whether or not the Stream is open for reading.
     *
     *  \return True if open for reading; False otherwise.
     */
    virtual bool isOpenForReading() const;

    /*! \brief Retuns whether or not the Stream is open for writing
     *
     *  \return True if open for writing; False otherwise.
     */
    virtual bool isOpenForWriting() const;

    /*! \brief Sets the Endianss of the stream
     *
     *  \param endian The Endianess to set \sa Endian
     */
    void setEndianess(Endian endian);

    /*! \brief Returns the current Endianness of the stream
     *
     *  \return Endian The current Stream Endianess
     */
    Endian endian() const;


    /*! \brief Returns whether the stream is BigEndian
     *
     *  \return bool True for BigEndian; False for LittleEndian
     */
    bool isBigEndian() const;

    /*! \brief Returns whether the stream is LittleEndian
     *
     *  \return bool True for LittleEndian; False for BigEndian
     */
    bool isLittleEndian() const;

protected:
    Uint32  m_bitPosition; //!< The current position in the current byte
    Uint64  m_position;    //!< The current position in the Stream
    Uint64  m_length;      //!< The length of the Stream
    Endian  m_endian;      //!< The Endianess of the Stream
    Uint8*  m_data;        //!< The Stream buffer
    bool    m_autoResize;  //!< Whether the stream is autoresizing
};
} // io
} // zelda
#endif // __STREAM_HPP__
