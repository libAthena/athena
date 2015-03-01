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

class IStream
{
public:
    virtual ~IStream() {}

    virtual void setEndian(Endian)    = 0;
    virtual Endian endian()      const= 0;
    virtual bool isBigEndian()   const= 0;
    virtual bool isLittleEndian()const= 0;
    virtual bool isOpen()        const= 0;
    virtual void seek(atInt64, SeekOrigin)=0;
    virtual bool atEnd()         const= 0;
    virtual atUint64 position()    const= 0;
    virtual atUint64 length()      const= 0;
    virtual void seekBit    (int)=0;
};
}
}
#endif // STREAM_HPP
