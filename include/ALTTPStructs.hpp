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

#ifndef __ALTTP_STRUCTS_HPP__
#define __ALTTP_STRUCTS_HPP__

#ifndef __DOXYGEN_IGNORE__

#include <string>
#include "Types.hpp"

namespace zelda
{

struct ALTTPRoomFlags
{
    bool Chest1:1;
    bool Chest2:1;
    bool Chest3:1;
    bool Chest4:1;
    bool Quadrant1:1;
    bool Quadrant2:1;
    bool Quadrant3:1;
    bool Quadrant4:1;
    bool Door1:1;
    bool Door2:1;
    bool Door3:1;
    bool Door4:1;
    bool BossBattleWon:1;
    bool Key:1;
    bool KeyOrChest:1;
    bool ChestOrTile:1;
};

struct ALTTPOverworldEvent
{
    bool Unused1:1;
    bool HeartPiece:1;
    bool Overlay:1;
    bool Unused2:1;
    bool Unused3:1;
    bool Unused4:1;
    bool Set:1;
    bool Unused5:1;
};

struct ALTTPInventory
{
    char Bow;
    char Boomerang;
    bool Hookshot;
    char Bombs; // Bomb count
    char Magic;
    bool FireRod;
    bool IceRod;
    bool Bombos;
    bool Ether;
    bool Quake;
    bool Torch;
    bool Hammer;
    char Flute;
    bool BugNet;
    bool Book;
    bool Bottles;
    bool Somaria;
    bool Byrna;
    bool MagicCape;
    char MagicMirror;
    char Gloves;
    char Boots;
    bool Flippers;
    bool MoonPearl;
    char Unused; //?
    char Sword;
    char Shield;
    char Armor;
    char BottleTypes[4];
};

/*! \struct ALTTPLightDarkWorldIndicator
 */
struct ALTTPLightDarkWorldIndicator
{
    bool Unused1:1;
    bool Unused2:1;
    bool Unused3:1;
    bool Unused4:1;
    bool Unused5:1;
    bool Unused6:1;
    bool IsDarkWorld:1;
    bool Unused7:1;
};


struct ALTTPDungeonItemFlags
{
    bool Unused1:1;
    bool Unused2:1;
    bool GanonsTower:1;
    bool TurtleRock:1;
    bool GargoylesDomain:1;
    bool TowerOfHera:1;
    bool IcePalace:1;
    bool SkullWoods:1;
    bool MiseryMire:1;
    bool DarkPalace:1;
    bool SwampPalace:1;
    bool HyruleCastle2:1; // Doesn't exists in orignal game
    bool DesertPalace:1;
    bool EasternPalace:1;
    bool HyruleCastle:1; // Doesn't exist in original game
    bool SewerPassage:1; // Doesn't exist in original game
};

struct ALTTPPendants
{
    bool Courage:1;
    bool Wisdom:1;
    bool Power:1;
    bool Unused1:1;
    bool Unused2:1;
    bool Unused3:1;
    bool Unused4:1;
    bool Unused5:1;
};

struct ALTTPAbilities
{
    char Nothing:1; //?
    char Swim:1;
    char Dash:1;
    char Pull:1;
    char Unknown1:1; //---
    char Talk:1;
    char Read:1;
    char Unknown2:1; //---
};

struct ALTTPCrystals
{
    bool MiseryMire:1;
    bool DarkPalace:1;
    bool IcePalace:1;
    bool TurtleRock:1;
    bool SwampPalace:1;
    bool GargoyleDomain:1;
    bool SkullWoods:1;
};

struct ALTTPMagicUsage
{
    bool Normal:1;
    bool Half:1;
    bool Quarter:1;
    bool Unused1:1;
    bool Unused2:1;
    bool Unused3:1;
    bool Unused4:1;
    bool Unused5:1;
};


struct ALTTPProgressFlags1
{
    bool UncleSecretPassage:1;
    bool DyingPriest:1; //?
    bool ZeldaSanctuary:1; //?
    bool Unused1:1;
    bool UncleLeftHouse:1;
    bool BookOfMudora:1;//? Math says it's a guess need to investigate
    bool DwarfPartner:1; //?
    bool Unused2:1;
};


struct ALTTPProgressFlags2
{
    bool BottleFromBum:1;
    bool BottleFromSalesMen:1;
    bool Unused1:1; //?
    bool FluteBoy:1;
    bool ThiefsChest:1;
    bool SavedSmithPartner:1;
    bool Unused2:1; //?
    bool SmithsHaveSword:1;
};

}

#endif // __DOXYGEN_IGNORE__
#endif // __ALTTP_STRUCTS_HPP__
