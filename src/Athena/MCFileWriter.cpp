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

MCFileWriter::MCFileWriter(Uint8* data, Uint64 length)
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
Uint16 MCFileWriter::calculateSlotChecksum(Uint32 game)
{
    Uint16 first = calculateChecksum((m_data + 0x34 + (0x10 * game)), 4);
    Uint16 second = calculateChecksum((m_data + 0x80 + (0x500 * game)), 0x500);

    first = (first + second) & 0xFFFF;
    Uint16 result = first << 16;
    second = ~first&0xFFFF;
    second += 1;
    result += second;

    return result;
}

Uint16 MCFileWriter::calculateChecksum(Uint8 *data, Uint32 length)
{
    Uint16 sum = 0;
    int i = length;

    for (Uint32 j = 0; j < length; j += 2)
    {
        sum += *(Uint16*)(data + j) ^ i;
        i -= 2;
    }

    sum &= 0xFFFF;

    return sum;
}

// TODO: Rewrite this to be more optimized, the current solution takes quite a few cycles
Uint8* MCFileWriter::reverse(Uint8* data, Uint32 length)
{
    Uint32 a = 0;
    Uint32 swap;

    for (;a<--length; a++)
    {
        swap = data[a];
        data[a] = data[length];
        data[length] = swap;
    }

    return data;
}


// TODO: Rewrite this to be more optimized, unroll more??
void MCFileWriter::unscramble()
{
    if (!m_data)
        return;

    for (Uint32 i = 0; i < m_length; i += 4)
    {
        Uint32 block1 = *(Uint32*)reverse((m_data + i), 4);
        Uint32 block2 = *(Uint32*)reverse((m_data + i + 4), 4);
        *(Uint32*)(m_data + i) = block2;
        *(Uint32*)(m_data + i + 4) = block1;
    }
}

} // io
} // zelda
