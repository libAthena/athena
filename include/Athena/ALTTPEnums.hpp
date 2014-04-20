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

#ifndef __ALTTP_ENUMS_HPP__
#define __ALTTP_ENUMS_HPP__

#ifndef __DOXYGEN_IGNORE__

#include "Types.hpp"

namespace Athena
{

enum BowType : char
{
    BowNone,
    BowArrows,
    BowSilverArrows
};

enum BoomerangType : char
{
    BoomerangNone,
    BoomerangBlue,
    BoomerangRed
};

enum MagicType : char
{
    MagicNone,
    MagicMushroom,
    MagicPowder
};

enum ArmorType : char
{
    GreenJerkin,
    BlueMail,
    RedMail
};

enum BottleType : char
{
    BottleNone,
    BottleMushroom, // No Use
    BottleEmpty,
    BottleRedPotion,
    BottleBluePotion,
    BottleFairy,
    BottleBee,
    BottleGoodBee
};


enum ALTTPStartLocation
{
    LinksHouse = 0x00,
    Sanctuary  = 0x01,
    Any        = 0x05
};

enum ALTTPProgressIndicator
{
    LinkInBed,
    InCastleWithSword,
    CompletedFirstDungeon,
    BeatenAghanim
};

enum ALTTPMapIcon
{
    Nothing = 0x00, //?
    CrossInKakariko = 0x01, //?
    CrossAtFirstDungeon = 0x02, //
    Pendant = 0x03,
    MasterSword = 0x04,
    AganhimCastle = 0x05,
    Crystal1 = 0x06,
    AllCrystals = 0x07,
    AganhimGanonTower = 0x08
};

enum ALTTPTagAlong
{
    Noone,
    Zelda,
    Unknown1,
    Oldman,
    ZeldaMessage,
    Blind,
    DwarfFrog,
    DwarfLW,
    Kiki,
    Unknown2,
    TheifsChest,
    AfterBoss
};

} // zelda
#endif // __DOXYGEN_IGNORE__
#endif // __ALTTP_ENUMS_HPP__
