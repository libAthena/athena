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

#ifndef STREAM_HPP
#define STREAM_HPP

#include "Global.hpp"
#include "Athena/NotImplementedException.hpp"

namespace Athena
{
namespace io
{
std::ostream& operator<<(std::ostream& os, Endian& endian);

class Stream
{
public:


    virtual void setEndian(Endian)    = 0;
    virtual Endian endian()      const= 0;
    virtual bool isBigEndian()   const= 0;
    virtual bool isLittleEndian()const= 0;
    virtual bool isOpen()        const= 0;
    virtual void seek(atInt64, SeekOrigin)=0;
    virtual bool atEnd()         const= 0;
    virtual atUint64 position()    const= 0;
    virtual atUint64 length()      const= 0;
protected:
    virtual void seekBit    (int){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    // Reading
    virtual bool   readBit()         {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atUint8  readUByte()       {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atInt8   readByte()        {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atUint8* readUBytes(atUint64){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atInt8*  readBytes(atUint64) {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atUint16 readUint16()      {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atInt16  readInt16()       {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atUint32 readUint32()      {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atInt32  readInt32()       {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atUint64 readUint64()      {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual atInt64  readInt64()       {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual double readDouble()      {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual float  readFloat()       {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual bool   readBool()        {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual std::string readUnicode(atInt32){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual std::string readString(atInt32) {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    // Writing
    virtual void writeBit   (bool){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeUByte (atUint8){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeByte  (atInt8){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeUBytes(atUint8*, atUint64){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeBytes (atInt8*, atUint64){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeUint16(atUint16){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeInt16 (atInt16){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeUint32(atUint32){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeInt32 (atInt32){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeUint64(atUint64){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeInt64 (atInt64){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeDouble(double){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeFloat (float){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeBool  (bool){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeString(const std::string&){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void writeUnicode(const std::string&){THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void fill(atUint8, atUint64) {THROW_NOT_IMPLEMENTED_EXCEPTION();}
    virtual void fill(atInt8, atUint64) {THROW_NOT_IMPLEMENTED_EXCEPTION();}
};
}
}
#endif // STREAM_HPP
