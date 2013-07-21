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

#include "ALTTPFileReader.hpp"
#include "ALTTPFile.hpp"
#include "ALTTPQuest.hpp"
#include <iostream>

namespace zelda
{

ALTTPFileReader::ALTTPFileReader(Uint8* data, Uint64 length)
    : BinaryReader(data, length)
{
}

ALTTPFileReader::ALTTPFileReader(const std::string& filename)
    : BinaryReader(filename)
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

        quest->setInventory((ALTTPInventory*)this->readBytes(sizeof(ALTTPInventory)));
        quest->setRupeeMax(this->readUInt16());
        quest->setRupeeCurrent(this->readUInt16());
        quest->setCompasses(readDungeonFlags());
        quest->setBigKeys(readDungeonFlags());
        quest->setDungeonMaps(readDungeonFlags());
        quest->setWishingPond(this->readUInt16());
        quest->setHealthMax(this->readByte());
        quest->setHealth(this->readByte());
        quest->setMagicPower(this->readByte());
        quest->setKeys(this->readByte());
        quest->setBombUpgrades(this->readByte());
        quest->setArrowUpgrades(this->readByte());
        quest->setHealthFiller(this->readByte());
        quest->setMagicFiller(this->readByte());
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
        quest->setBombFiller(this->readByte());
        quest->setArrowFiller(this->readByte());
        quest->setArrows(this->readByte());
        this->seek(1);
        ALTTPAbilities abilities;
        abilities.Nothing = this->readBit();
        abilities.Swim = this->readBit();
        abilities.Dash = this->readBit();
        abilities.Pull = this->readBit();
        abilities.Unknown1 = this->readBit();
        abilities.Talk = this->readBit();
        abilities.Read = this->readBit();
        abilities.Unknown2 = this->readBit();
        quest->setAbilityFlags(abilities);
        quest->setCrystals((ALTTPCrystals&)*this->readBytes(sizeof(ALTTPCrystals)));
        quest->setMagicUsage((ALTTPMagicUsage&)*this->readBytes(sizeof(ALTTPMagicUsage)));

        j = 0x10;
        while ((j--) > 0)
        {
            dungeonKeys.push_back(this->readByte());
        }

        quest->setDungeonKeys(dungeonKeys);
        seek(0x039);
        quest->setProgressIndicator((ALTTPProgressIndicator)this->readByte());
        quest->setProgressFlags1((ALTTPProgressFlags1&)*this->readBytes(sizeof(ALTTPProgressFlags1)));
        quest->setMapIcon((ALTTPMapIcon)this->readByte());
        quest->setStartLocation((ALTTPStartLocation)this->readByte());
        quest->setProgressFlags2((ALTTPProgressFlags2&)*this->readBytes(sizeof(ALTTPProgressFlags2)));
        quest->setLightDarkWorldIndicator((ALTTPLightDarkWorldIndicator&)*this->readBytes(1));
        this->seek(1);
        quest->setTagAlong((ALTTPTagAlong)this->readByte());

        j = 6;
        while((j--) > 0)
        {
            oldmanFlags.push_back(this->readByte());
        }

        quest->setOldManFlags(oldmanFlags);
        quest->setBombFlag(this->readByte());

        j = 5;
        while((j--) > 0)
        {
            unknown1.push_back(this->readByte());
        }

        quest->setUnknown1(unknown1);

        j = 6;
        while((j--) > 0)
        {
            playerName.push_back(this->readUInt16());
        }

        quest->setPlayerName(playerName);
        quest->setValid((this->readUInt16() == 0x55AA));

        j = 0x0D;
        while((j--) > 0)
        {
            dungeonDeaths.push_back(this->readUInt16());
        }
        quest->setDungeonDeathTotals(dungeonDeaths);

        quest->setUnknown2(this->readUInt16());
        quest->setDeathSaveCount(this->readUInt16());
        quest->setPostGameDeathCounter(this->readInt16());

        this->seek(0xF7);

        quest->setChecksum(this->readUInt16());

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
    flags->Chest1        = readBit();
    flags->Chest2        = readBit();
    flags->Chest3        = readBit();
    flags->Chest4        = readBit();
    flags->Quadrant1     = readBit();
    flags->Quadrant2     = readBit();
    flags->Quadrant3     = readBit();
    flags->Quadrant4     = readBit();
    flags->Door1         = readBit();
    flags->Door2         = readBit();
    flags->Door3         = readBit();
    flags->Door4         = readBit();
    flags->BossBattleWon = readBit();
    flags->Key           = readBit();
    flags->KeyOrChest    = readBit();
    flags->ChestOrTile   = readBit();

    return flags;
}

ALTTPOverworldEvent* ALTTPFileReader::readOverworldEvent()
{
    ALTTPOverworldEvent* event = new ALTTPOverworldEvent;
    event->Unused1    = readBit();
    event->HeartPiece = readBit();
    event->Overlay    = readBit();
    event->Unused2    = readBit();
    event->Unused3    = readBit();
    event->Unused4    = readBit();
    event->Set        = readBit();
    event->Unused5    = readBit();
    return event;
}

ALTTPDungeonItemFlags ALTTPFileReader::readDungeonFlags()
{
    ALTTPDungeonItemFlags flags;
    flags.Unused1         = readBit();
    flags.GanonsTower     = readBit();
    flags.TurtleRock      = readBit();
    flags.GargoylesDomain = readBit();
    flags.TowerOfHera     = readBit();
    flags.IcePalace       = readBit();
    flags.SkullWoods      = readBit();
    flags.MiseryMire      = readBit();
    flags.DarkPalace      = readBit();
    flags.SwampPalace     = readBit();
    flags.HyruleCastle2   = readBit();
    flags.DesertPalace    = readBit();
    flags.EasternPalace   = readBit();
    flags.HyruleCastle    = readBit();
    flags.SewerPassage    = readBit();

    return flags;
}

} // zelda
