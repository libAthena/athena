// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#ifndef MEMORYWRITER_HPP
#define MEMORYWRITER_HPP

#include "Athena/IStreamWriter.hpp"
#include <string>
#include <functional>

namespace Athena
{
namespace io
{

/*! \class BinaryWriter
 *  \brief A Stream class for writing binary data
 *
 *  A Class for writing binary data to a file or memory stream,
 *  all work is done using a memory buffer, and not written directly to the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
class MemoryWriter : public IStreamWriter
{
public:
    /*! \brief This constructor takes an existing buffer to write to.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    explicit MemoryWriter(atUint8* data = nullptr, atUint64 length=0x10);

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    MemoryWriter(const std::string& filename, std::function<void(int)> progressFun = nullptr);

    virtual ~MemoryWriter();

    /*! \brief Sets the Endianss of the stream
     *
     *  \param endian The Endianess to set \sa Endian
     */
    void setEndian(Endian endian);

    /*! \brief Returns the current Endianness of the stream
     *
     *  \return Endian The current Stream Endianess
     */
    Endian endian()      const;

    /*! \brief Returns whether the stream is BigEndian
     *
     *  \return bool True for BigEndian; False for LittleEndian
     */
    bool isBigEndian()   const;

    /*! \brief Returns whether the stream is LittleEndian
     *
     *  \return bool True for LittleEndian; False for BigEndian
     */
    bool isLittleEndian()const;

    /*! \brief Retuns whether or not the Stream is open.
     *
     *  \return True if open; False otherwise.
     */
    bool isOpen() const;

    /*! \brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  \param position where in the buffer to seek
     *  \param origin The Origin to seek \sa SeekOrigin
     */
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);


    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    bool atEnd()         const;


    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    atUint64 position()    const;

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    atUint64 length()      const;

    /*! \brief Sets the buffer to the given one, deleting the current one.<br />
     *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
     *         if that was not the intent.<br />
     *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \param data The new buffer.
     *  \param length The length of the new buffer.
     *  \throw IOException
     */
    void  setData(const atUint8* data, atUint64 length);


    /*! \brief Returns a copy of the current buffer.<br />
     *         Changes to the copy do not affect the buffer so it's perfectly safe to
     *         directly edit the buffer and use setData to set the new information.<br />
     *         However once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \return Uint8* The copy of the buffer.
     */
    atUint8* data() const;

    /*! \brief Sets the target file
     *
     *  \param filepath The path to write to.
     */
    void setFilepath(const std::string& filepath);

    /*! \brief
     * Returns the target file
     */
    std::string filepath() const;

    /*! \brief Saves the file to the specified file.
     *
     *   \param filename If not empty, the filename to save to
     */
    void save(const std::string& filename="");

    /*!
     * \brief Seeks to the specified bit within the current byte
     * \param bit Bit to seek to, range is 0 - 7
     */
    void seekBit(int bit);

    /*! \brief Writes a bit at the current position and advances the position by one bit.
     * \param val the value to write
     * \throw IOException
     */
    void writeBit(bool val);

    /*! \brief Writes a byte at the current position and advances the position by one byte.
     * \param byte The value to write
     */
    void writeUByte(atUint8 val);

    /*! \brief Writes a byte at the current position and advances the position by one byte.
     * \param byte The value to write
     * \throw IOException
     */
    void writeByte(atInt8 val);

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    void writeUBytes(atUint8* data, atUint64 len);

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    void writeBytes(atInt8* data, atUint64 len);

    /*! \brief Writes an Int16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt16(atInt16 val);

    /*! \brief Writes an Uint16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeUint16(atUint16);

    /*! \brief Writes an Int32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt32(atInt32);

    /*! \brief Writes an Uint32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeUint32(atUint32);

    /*! \brief Writes an Int64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt64(atInt64);

    /*! \brief Writes an Uint64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeUint64(atUint64);

    /*! \brief Writes an float to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeFloat(float);

    /*! \brief Writes an double to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeDouble(double);

    /*! \brief Writes an bool to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeBool(bool);

    /*! \brief Writes an unicode string to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     */
    void writeUnicode(const std::string& str);

    /*! \brief Writes an string to the buffer and advances the buffer.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     */
    void writeString(const std::string& str);

    void fill(atUint8 val, atUint64 length);
    void fill(atInt8 val, atUint64 length);

    void setProgressCallback(std::function<void(int)> cb);
protected:
    void loadData();
    atUint8*      m_data;
    atUint64      m_length;
    std::string m_filepath; //!< Path to the target file
    atUint64      m_position;
    atUint64      m_bitPosition;
    Endian      m_endian;
    std::function<void(int)> m_progressCallback;
private:
    void resize(atUint64 newSize);
};
}
}

#ifndef MEMORYWRITER_BASE
#define MEMORYWRITER_BASE() \
    private: \
        typedef Athena::io::MemoryWriter base
#endif // BINARYWRITER_BASE
#endif // MEMORYWRITER_HPP
