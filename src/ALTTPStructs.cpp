// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#include "ALTTPStructs.hpp"

std::string ALTTPInventory::bowType()
{
    static const char* BowTypeStr[] =
    {
        "None",
        "Bow & Arrows",
        "Bow & Silver Arrows"
    };

    return BowTypeStr[(Uint32)Bow];
}

std::string ALTTPInventory::boomerangType()
{
    static const char* BoomerangTypStr[] =
    {
        "None",
        "Blue Boomerang",
        "Red Boomerang"
    };

    return BoomerangTypStr[(Uint32)Boomerang];
}

std::string ALTTPInventory::magicType()
{
    static const char* MagicTypeStr[] =
    {
        "None",
        "Mushroom",
        "Magic Powder"
    };

    return MagicTypeStr[(Uint32)Magic];
}

std::string ALTTPInventory::armorType()
{
    static const char* ArmorTypeStr[] =
    {
        "Green Jerkin",
        "Blue Mail",
        "Red Mail"
    };

    return ArmorTypeStr[(Uint32)Armor];
}

std::string ALTTPInventory::bottleType(Uint32 bottle)
{
    static const char* BottleTypeStr[]=
    {
        "None",
        "Mushroom", // No Use
        "Empty",
        "Red Potion",
        "Blue Potion",
        "Fairy",
        "Bee",
        "Good Bee"
    };

    return BottleTypeStr[(Uint32)BottleTypes[bottle]];
}

