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

#ifndef __MCFILE_HPP__
#define __MCFILE_HPP__

#include "Athena/Global.hpp"

namespace Athena
{

class MCSlot;
/*! \class MCFile
 *  \brief The Minish Cap data container class class
 *
 * Contains all relevant data for a The Minish Cap save,
 * file.
 */
class MCFile
{
public:
    static constexpr char* VERSION_EU_JP = (char*)"AGBZELDA:THE MINISH CAP:ZELDA 3\0";
    static constexpr char* VERSION_US    = (char*)"AGBZELDA:THE MINISH CAP:ZELDA 5\0";
    enum SlotType
    {
        New     = 0x54494E49,
        Valid   = 0x4D435A33,
        Deleted = 0x466C6544
    };

    MCFile();

    static atUint8* unscramble(atUint8* data, atUint32 length);
private:
    MCSlot* m_slots[3];
};

} // zelda
#endif // __MCFILE_HPP__
