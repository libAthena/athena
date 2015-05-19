#ifndef ATHENA_NO_SAVES
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

#include "Athena/MCFileWriter.hpp"

namespace Athena
{
namespace io
{

MCFileWriter::MCFileWriter(atUint8* data, atUint64 length)
    : base(data, length)
{
}

MCFileWriter::MCFileWriter(const std::string& filename)
    : base(filename)
{
}

// TODO: Check the implementation, it seems to work fine, however it's not exactly correct,
// looking at the disassembly, MC seems to do some weird checking that isn't being done with this solution
// need to figure out what it's doing and whether it's relevant to the checksum.
atUint16 MCFileWriter::calculateSlotChecksum(atUint32 game)
{
    atUint16 first = calculateChecksum((m_data + 0x34 + (0x10 * game)), 4);
    atUint16 second = calculateChecksum((m_data + 0x80 + (0x500 * game)), 0x500);

    first = (first + second) & 0xFFFF;
    atUint16 result = first << 16;
    second = ~first & 0xFFFF;
    second += 1;
    result += second;

    return result;
}

atUint16 MCFileWriter::calculateChecksum(atUint8* data, atUint32 length)
{
    atUint16 sum = 0;
    int i = length;

    for (atUint32 j = 0; j < length; j += 2)
    {
        sum += *(atUint16*)(data + j) ^ i;
        i -= 2;
    }

    sum &= 0xFFFF;

    return sum;
}

} // io
} // zelda
#endif // ATHENA_NO_SAVES
