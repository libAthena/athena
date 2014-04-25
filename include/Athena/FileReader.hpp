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

#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include "Stream.hpp"
#include <string>
#include <cstdio>

namespace Athena
{
namespace io
{
class FileReader : public Stream
{
public:
    FileReader(const std::string& filename);
    ~FileReader();
    std::string filename() const;

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


    void   seekBit(int);
    bool   readBit();
    Uint8  readUByte();
    Int8   readByte();
    Uint8* readUBytes(Uint64 len);
    Int8*  readBytes(Uint64 len);
    Uint16 readUint16();
    Int16  readInt16();
    Uint32 readUint32();
    Int32  readInt32();
    Uint64 readUint64();
    Int64  readInt64();
    double readDouble();
    float  readFloat();
    bool   readBool();
    std::string readString();
    std::string readUnicode();
private:
    std::string  m_filename;
    FILE*        m_fileHandle;
    Endian       m_endian;
    Uint8        m_currentByte;
    Uint8        m_bitShift;
    bool         m_bitValid;
};
} // io
} // Athena

#ifndef FILEREADER_BASE
#define FILEREADER_BASE \
private: \
    typedef Athena::io::FileReader base;

#endif // FILEREADER_BASE

#endif // FILESTREAM_HPP
