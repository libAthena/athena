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

#ifndef MEMORYREADER_HPP
#define MEMORYREADER_HPP

#include "Athena/IStreamReader.hpp"
#include <string>
#include <functional>

namespace Athena
{
namespace io
{
/*! \class BinaryReader
 *  \brief A Stream class for reading binary data
 *
 *  A Class for reading binary data from a file or memory stream,
 *  all work is done using a memory buffer, and not read directly from the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
class MemoryReader : public IStreamReader
{
public:
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
    MemoryReader(const std::string& filename, std::function<void(int)> progressFun = nullptr);

    virtual ~MemoryReader();

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
    bool atEnd() const;

    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    atUint64 position() const;

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    atUint64 length() const;

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
     *  \sa Endian
     *  \param filepath The path to write to.
     */
    void setFilepath(const std::string& filepath);

    /*! \brief Returns the target file
     *
     */
    std::string filepath() const;

    /*!
     * \brief Seeks to the specified bit within the current byte
     * \param bit Bit to seek to, range is 0 - 7
     */
    void seekBit(int bit);

    /*! \brief Reads a bit at the current position and advances the current position
     *
     * \return bool The value at the current position
     */
    bool readBit();

    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Int8 The value at the current position
     */
    atInt8 readByte();

    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Uint8 The value at the current position
     */
    atUint8 readUByte();

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Uint8* The buffer at the current position from the given length.
     */
    atInt8* readBytes(atUint64 length);

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Int8* The buffer at the current position from the given length.
     */
    atUint8* readUBytes(atUint64 length);

    /*! \brief Reads a Int16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int16 The value at the current address
     *  \throw IOException when address is out of range
     */
    atInt16 readInt16();

    /*! \brief Reads a Uint16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint16 The value at the current address
     *  \throw IOException when address is out of range
     */
    atUint16 readUint16();

    /*! \brief Reads a Int32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int32 The value at the current address
     *  \throw IOException when address is out of range
     */
    atInt32 readInt32();

    /*! \brief Reads a Uint32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint32 The value at the current address
     *  \throw IOException when address is out of range
     */
    atUint32 readUint32();

    /*! \brief Reads a Int64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int64 The value at the current address
     *  \throw IOException when address is out of range
     */
    atInt64 readInt64();

    /*! \brief Reads a Uint64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint64 The value at the current address
     *  \throw IOException when address is out of range
     */
    atUint64 readUint64();

    /*! \brief Reads a float and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return float The value at the current address
     *  \throw IOException when address is out of range
     */
    float readFloat();

    /*! \brief Reads a double and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return double The value at the current address
     *  \throw IOException when address is out of range
     */
    double readDouble();

    /*! \brief Reads a bool and advances the current position
     *
     *  \return bool The value at the current address
     *  \throw IOException when address is out of range
     */
    bool readBool();

    /*! \brief Reads a Unicode string and advances the position in the file
     *
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    std::string readUnicode(atInt32 maxlen = -1);

    /*! \brief Reads a string and advances the position in the file
     *
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    std::string readString(atInt32 maxlen = -1);

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
};

} // io
} // Athena

#ifndef MEMORYREADER_BASE
#define MEMORYREADER_BASE() \
private: \
    typedef Athena::io::MemoryReader base

#endif // MEMORYREADER_BASE

#endif // MEMORYREADER_HPP
