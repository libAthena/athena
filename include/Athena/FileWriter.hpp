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

#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#include "Athena/Stream.hpp"

namespace Athena
{
namespace io
{
class FileWriter : public Stream
{
public:
    FileWriter(const std::string& filename);
    ~FileWriter();

    void setEndian(Endian endian);
    Endian endian() const;
    bool isBigEndian() const;
    bool isLittleEndian() const;
    void open();
    void close();
    bool isOpen() const;
    bool save();
    void seek(Int64 pos, SeekOrigin origin = SeekOrigin::Current);
    bool atEnd() const;
    Uint64 position() const;
    Uint64 length() const;

    void writeBit   (bool val);
    void seekBit    (int bit);
    void writeUByte (Uint8 val);
    void writeByte  (Int8 val);
    void writeUBytes(Uint8* data, Uint64 len);
    void writeBytes (Int8* data, Uint64 len);
    void writeUint16(Uint16 val);
    void writeInt16 (Int16 val);
    void writeUint32(Uint32 val);
    void writeInt32 (Int32 val);
    void writeUint64(Uint64 val);
    void writeInt64 (Int64 val);
    void writeDouble(double val);
    void writeFloat (float val);
    void writeBool  (bool val);
    void writeString(const std::string& val);
    void writeUnicode(const std::string& str);
private:
    std::string  m_filename;
    FILE*        m_fileHandle;
    Endian       m_endian;
    Uint8        m_currentByte;
    Uint8        m_bytePosition;
    Uint8        m_bitShift;
    bool         m_bitValid;
};
}
} // Athena
#endif // FILEWRITER_HPP
