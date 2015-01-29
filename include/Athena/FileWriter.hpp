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
    virtual ~FileWriter();

    void setEndian(Endian endian);
    Endian endian() const;
    bool isBigEndian() const;
    bool isLittleEndian() const;
    void open();
    void close();
    bool isOpen() const;
    bool save();
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);
    bool atEnd() const;
    atUint64 position() const;
    atUint64 length() const;

    void writeBit   (bool val);
    void seekBit    (int bit);
    void writeUByte (atUint8 val);
    void writeByte  (atInt8 val);
    void writeUBytes(atUint8* data, atUint64 len);
    void writeBytes (atInt8* data, atUint64 len);
    void writeUint16(atUint16 val);
    void writeInt16 (atInt16 val);
    void writeUint32(atUint32 val);
    void writeInt32 (atInt32 val);
    void writeUint64(atUint64 val);
    void writeInt64 (atInt64 val);
    void writeDouble(double val);
    void writeFloat (float val);
    void writeBool  (bool val);
    void writeString(const std::string& val);
    void writeUnicode(const std::string& str);
private:
    std::string  m_filename;
    FILE*        m_fileHandle;
    Endian       m_endian;
    atUint8      m_currentByte;
    atUint64     m_bytePosition;
    atUint8      m_bitShift;
    bool         m_bitValid;
};
}
} // Athena

#ifndef FILEWRITER_BASE
#define FILEWRITER_BASE \
private: \
    typedef Athena::io::FileWriter base;

#endif // FILEWRITER_BASE
#endif // FILEWRITER_HPP
