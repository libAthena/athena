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

#include "Athena/MCFile.hpp"

namespace Athena
{

const char MCFile::VERSION_EU_JP[33] = "AGBZELDA:THE MINISH CAP:ZELDA 3\0";
const char MCFile::VERSION_US[33]    = "AGBZELDA:THE MINISH CAP:ZELDA 5\0";
MCFile::MCFile()
{
}

// TODO: Rewrite this to be more optimized, the current solution takes quite a few cycles
atUint8* reverse(atUint8* data, atUint32 length)
{
    atUint32 a = 0;
    atUint32 swap;

    for (; a < --length; a++)
    {
        swap = data[a];
        data[a] = data[length];
        data[length] = swap;
    }

    return data;
}

atUint8* MCFile::unscramble(atUint8* data, atUint64 length)
{
    if (!data)
        return nullptr;

    for (atUint32 i = 0; i < length; i += 8)
    {
        atUint32 block1 = *(atUint32*)reverse((data + i), 4);
        atUint32 block2 = *(atUint32*)reverse((data + i + 4), 4);
        *(atUint32*)(data + i) = block2;
        *(atUint32*)(data + i + 4) = block1;
    }

    return data;
}

} // zelda
