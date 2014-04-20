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

#include "Athena/ALTTPFileReader.hpp"
#include "Athena/ALTTPFile.hpp"
#include "Athena/ALTTPQuest.hpp"
#include <iostream>

namespace Athena
{
namespace io
{

ALTTPFileReader::ALTTPFileReader(Uint8* data, Uint64 length)
    : base(data, length)
{
}

ALTTPFileReader::ALTTPFileReader(const std::string& filename)
    : base(filename)
{
}

ALTTPFile* ALTTPFileReader::readFile()
{
    std::vector<ALTTPQuest*> quests;
    std::vector<ALTTPQuest*> backup;

    for (Uint32 i = 0; i < 6; i++)
    {
        // Temporary values to use for each save
        ALTTPQuest* quest = new ALTTPQuest();
        std::vector<ALTTPRoomFlags*>        roomFlags;
        std::vector<ALTTPOverworldEvent*>   owEvents;
        std::vector<Uint8>                  dungeonKeys;
        std::vector<Uint8>                  oldmanFlags;
        std::vector<Uint8>                  unknown1;
        std::vector<Uint16>                 playerName;
        std::vector<Uint16>                 dungeonDeaths;

        int j = 0x140;
        while ((j--) > 0)
        {
            roomFlags.push_back(readRoomFlags());
        }
        quest->setRoomFlags(roomFlags);

        j = 0x0C0;
        while ((j--) > 0)
            owEvents.push_back(readOverworldEvent());

        quest->setOverworldEvents(owEvents);

        quest->setInventory((ALTTPInventory*)base::readBytes(sizeof(ALTTPInventory)));
        quest->setRupeeMax(base::readUint16());
        quest->setRupeeCurrent(base::readUint16());
        quest->setCompasses(readDungeonFlags());
        quest->setBigKeys(readDungeonFlags());
        quest->setDungeonMaps(readDungeonFlags());
        quest->setWishingPond(base::readUint16());
        quest->setHealthMax(base::readByte());
        quest->setHealth(base::readByte());
        quest->setMagicPower(base::readByte());
        quest->setKeys(base::readByte());
        quest->setBombUpgrades(base::readByte());
        quest->setArrowUpgrades(base::readByte());
        quest->setHealthFiller(base::readByte());
        quest->setMagicFiller(base::readByte());
        ALTTPPendants pendants;
        pendants.Courage = readBit();
        pendants.Wisdom = readBit();
        pendants.Power = readBit();
        pendants.Unused1 = false;
        pendants.Unused2 = false;
        pendants.Unused3 = false;
        pendants.Unused4 = false;
        pendants.Unused5 = false;
        quest->setPendants(pendants);
        quest->setBombFiller(base::readByte());
        quest->setArrowFiller(base::readByte());
        quest->setArrows(base::readByte());
        base::seek(1);
        ALTTPAbilities abilities;
        abilities.Nothing = base::readBit();
        abilities.Swim = base::readBit();
        abilities.Dash = base::readBit();
        abilities.Pull = base::readBit();
        abilities.Unknown1 = base::readBit();
        abilities.Talk = base::readBit();
        abilities.Read = base::readBit();
        abilities.Unknown2 = base::readBit();
        quest->setAbilityFlags(abilities);
        quest->setCrystals((ALTTPCrystals&)*base::readBytes(sizeof(ALTTPCrystals)));
        quest->setMagicUsage((ALTTPMagicUsage&)*base::readBytes(sizeof(ALTTPMagicUsage)));

        j = 0x10;
        while ((j--) > 0)
        {
            dungeonKeys.push_back(base::readByte());
        }

        quest->setDungeonKeys(dungeonKeys);
        base::seek(0x039);
        quest->setProgressIndicator((ALTTPProgressIndicator)base::readByte());
        quest->setProgressFlags1((ALTTPProgressFlags1&)*base::readBytes(sizeof(ALTTPProgressFlags1)));
        quest->setMapIcon((ALTTPMapIcon)base::readByte());
        quest->setStartLocation((ALTTPStartLocation)base::readByte());
        quest->setProgressFlags2((ALTTPProgressFlags2&)*base::readBytes(sizeof(ALTTPProgressFlags2)));
        quest->setLightDarkWorldIndicator((ALTTPLightDarkWorldIndicator&)*base::readBytes(1));
        base::seek(1);
        quest->setTagAlong((ALTTPTagAlong)base::readByte());

        j = 6;
        while((j--) > 0)
        {
            oldmanFlags.push_back(base::readByte());
        }

        quest->setOldManFlags(oldmanFlags);
        quest->setBombFlag(base::readByte());

        j = 5;
        while((j--) > 0)
        {
            unknown1.push_back(base::readByte());
        }

        quest->setUnknown1(unknown1);

        j = 6;
        while((j--) > 0)
        {
            playerName.push_back(base::readUint16());
        }

        quest->setPlayerName(playerName);
        quest->setValid((base::readUint16() == 0x55AA));

        j = 0x0D;
        while((j--) > 0)
        {
            dungeonDeaths.push_back(base::readUint16());
        }
        quest->setDungeonDeathTotals(dungeonDeaths);

        quest->setUnknown2(base::readUint16());
        quest->setDeathSaveCount(base::readUint16());
        quest->setPostGameDeathCounter(base::readInt16());

        base::seek(0xF7);

        quest->setChecksum(base::readUint16());

        if (i < 3)
            quests.push_back(quest);
        else
            backup.push_back(quest);
    }

    return new ALTTPFile(quests, backup);
}

ALTTPRoomFlags* ALTTPFileReader::readRoomFlags()
{
    ALTTPRoomFlags* flags = new ALTTPRoomFlags;
    flags->Chest1        = base::readBit();
    flags->Chest2        = base::readBit();
    flags->Chest3        = base::readBit();
    flags->Chest4        = base::readBit();
    flags->Quadrant1     = base::readBit();
    flags->Quadrant2     = base::readBit();
    flags->Quadrant3     = base::readBit();
    flags->Quadrant4     = base::readBit();
    flags->Door1         = base::readBit();
    flags->Door2         = base::readBit();
    flags->Door3         = base::readBit();
    flags->Door4         = base::readBit();
    flags->BossBattleWon = base::readBit();
    flags->Key           = base::readBit();
    flags->KeyOrChest    = base::readBit();
    flags->ChestOrTile   = base::readBit();

    return flags;
}

ALTTPOverworldEvent* ALTTPFileReader::readOverworldEvent()
{
    ALTTPOverworldEvent* event = new ALTTPOverworldEvent;
    event->Unused1    = base::readBit();
    event->HeartPiece = base::readBit();
    event->Overlay    = base::readBit();
    event->Unused2    = base::readBit();
    event->Unused3    = base::readBit();
    event->Unused4    = base::readBit();
    event->Set        = base::readBit();
    event->Unused5    = base::readBit();
    return event;
}

ALTTPDungeonItemFlags ALTTPFileReader::readDungeonFlags()
{
    ALTTPDungeonItemFlags flags;
    flags.Unused1         = base::readBit();
    flags.GanonsTower     = base::readBit();
    flags.TurtleRock      = base::readBit();
    flags.GargoylesDomain = base::readBit();
    flags.TowerOfHera     = base::readBit();
    flags.IcePalace       = base::readBit();
    flags.SkullWoods      = base::readBit();
    flags.MiseryMire      = base::readBit();
    flags.DarkPalace      = base::readBit();
    flags.SwampPalace     = base::readBit();
    flags.HyruleCastle2   = base::readBit();
    flags.DesertPalace    = base::readBit();
    flags.EasternPalace   = base::readBit();
    flags.HyruleCastle    = base::readBit();
    flags.SewerPassage    = base::readBit();

    return flags;
}

} // io
} // zelda
