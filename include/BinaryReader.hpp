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

#ifndef __BINARYREADER_HPP__
#define __BINARYREADER_HPP__

#include "Stream.hpp"
#include <string>

namespace zelda
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
class BinaryReader : public Stream
{
public:
    /*! \brief This constructor takes an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    BinaryReader(const Uint8* data, Uint64 length);
    /*! \brief This constructor takes an existing Stream to read from.
     *
     *  \param stream The stream to read data from
     */
    BinaryReader(const Stream& stream);
    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    BinaryReader(const std::string& filename);


    /*! \brief Reads a Int16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int16 The value at the current address
     *  \throw IOException when address is out of range
     */
    Int16       readInt16();

    /*! \brief Reads a Uint16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint16 The value at the current address
     *  \throw IOException when address is out of range
     */
    Uint16      readUInt16();

    /*! \brief Reads a Int32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int32 The value at the current address
     *  \throw IOException when address is out of range
     */
    Int32       readInt32();

    /*! \brief Reads a Uint32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint32 The value at the current address
     *  \throw IOException when address is out of range
     */
    Uint32      readUInt32();

    /*! \brief Reads a Int64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int64 The value at the current address
     *  \throw IOException when address is out of range
     */
    Int64       readInt64();

    /*! \brief Reads a Uint64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint64 The value at the current address
     *  \throw IOException when address is out of range
     */
    Uint64      readUInt64();

    /*! \brief Reads a float and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return float The value at the current address
     *  \throw IOException when address is out of range
     */
    float       readFloat();

    /*! \brief Reads a double and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return double The value at the current address
     *  \throw IOException when address is out of range
     */
    double      readDouble();
    /*! \brief Reads a bool and advances the current position
     *
     *  \return bool The value at the current address
     *  \throw IOException when address is out of range
     */
    bool        readBool();
    /*! \brief Reads a Unicode string and advances the position in the file
     *
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    std::string readUnicode();

    /*! \brief Reads a string and advances the position in the file
     *
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    std::string readString();

protected:
    /*! \brief Overload of isOpenForWriting in Stream
     *
     * \return false
     */
    bool   isOpenForWriting();
    /*! \brief Overload of writeByte in Stream
     *
     * \throw IOException
     */
    void writeByte(Int8 byte);
    /*! \brief Overload of writeBytes in Stream
     *
     * \throw IOException
     */
    void writeBytes(Int8*, Int64);
    std::string m_filepath; //!< Path to the target file
};
}
}

#ifndef BINARYREADER_BASE
#define BINARYREADER_BASE \
    private: \
    typedef zelda::io::BinaryReader base;
#endif // BINARYREADER_BASE
#endif // __BINARYREADER_HPP__
