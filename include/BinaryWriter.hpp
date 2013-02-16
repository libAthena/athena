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
#ifndef __BINARYWRITER_HPP__
#define __BINARYWRITER_HPP__

#include "Stream.hpp"
#include <string>

/*! \class BinaryWriter
 *  \brief A Stream class for writing binary data
 *
 *  A Class for writing binary data to a file or memory stream,
 *  all work is done using a memory buffer, and not written directly to the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
class BinaryWriter : public Stream
{
public:
    /*! \brief This constructor takes an existing buffer to write to.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    BinaryWriter(const Uint8* data, Uint64 length);

    /*! \brief This constructor takes an existing Stream to write to.
     *
     *  \param stream The stream to write data to
     */
    BinaryWriter(const Stream& stream);
    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    BinaryWriter(const std::string& filename);

    /*! \brief Saves the file to the specified file.
     *
     *   \param filename If not empty, the filename to save to
     */
    void save(const std::string& filename="");

    /*! \brief Writes an Int16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt16(Int16 val);

    /*! \brief Writes an Uint16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeUInt16(Uint16);

    /*! \brief Writes an Int32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt32(Int32);

    /*! \brief Writes an Uint32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeUInt32(Uint32);

    /*! \brief Writes an Int64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt64(Int64);

    /*! \brief Writes an Uint64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeUInt64(Uint64);

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
protected:
    Int8 readByte();
    Int8* readBytes(Int64);
    bool isOpenForReading();
    std::string m_filename;
};

#endif
