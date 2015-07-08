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

#include "Athena/ALTTPFileReader.hpp"
#include "Athena/ALTTPFile.hpp"
#include "Athena/ALTTPQuest.hpp"
#include <iostream>
#include "Athena/Global.hpp"

namespace Athena
{
namespace io
{

ALTTPFileReader::ALTTPFileReader(atUint8* data, atUint64 length)
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

    for (atUint32 i = 0; i < 6; i++)
    {
        // Temporary values to use for each save
        ALTTPQuest* quest = new ALTTPQuest();
        std::vector<ALTTPRoomFlags*>        roomFlags;
        std::vector<ALTTPOverworldEvent*>   owEvents;
        std::vector<atUint8>                  dungeonKeys;
        std::vector<atUint8>                  oldmanFlags;
        std::vector<atUint8>                  unknown1;
        std::vector<atUint16>                 playerName;
        std::vector<atUint16>                 dungeonDeaths;

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
        atUint8 pendantsByte = base::readUByte();
        ALTTPPendants pendants;
        pendants.Courage = pendantsByte & 1;
        pendants.Wisdom  = (pendantsByte >> 1) & 1;
        pendants.Power   = (pendantsByte >> 2) & 1;
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
        atUint8 abilitiesByte = base::readUByte();
        ALTTPAbilities abilities;
        abilities.Nothing = abilitiesByte & 1;
        abilities.Swim = (abilitiesByte >> 1) & 1;
        abilities.Dash = (abilitiesByte >> 2) & 1;
        abilities.Pull = (abilitiesByte >> 3) & 1;
        abilities.Unknown1 = (abilitiesByte >> 4) & 1;
        abilities.Talk = (abilitiesByte >> 5) & 1;
        abilities.Read = (abilitiesByte >> 6) & 1;
        abilities.Unknown2 = (abilitiesByte >> 7) & 1;
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

        while ((j--) > 0)
        {
            oldmanFlags.push_back(base::readByte());
        }

        quest->setOldManFlags(oldmanFlags);
        quest->setBombFlag(base::readByte());

        j = 5;

        while ((j--) > 0)
        {
            unknown1.push_back(base::readByte());
        }

        quest->setUnknown1(unknown1);

        j = 6;

        while ((j--) > 0)
        {
            playerName.push_back(base::readUint16());
        }

        quest->setPlayerName(playerName);
        quest->setValid((base::readUint16() == 0x55AA));

        j = 0x0D;

        while ((j--) > 0)
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
    atUint8 flagsByte    = base::readUByte();
    flags->Chest1        = flagsByte & 1;
    flags->Chest2        = (flagsByte >> 1) & 1;
    flags->Chest3        = (flagsByte >> 2) & 1;
    flags->Chest4        = (flagsByte >> 3) & 1;
    flags->Quadrant1     = (flagsByte >> 4) & 1;
    flags->Quadrant2     = (flagsByte >> 5) & 1;
    flags->Quadrant3     = (flagsByte >> 6) & 1;
    flags->Quadrant4     = (flagsByte >> 7) & 1;
    flagsByte            = base::readUByte();
    flags->Door1         = flagsByte & 1;
    flags->Door2         = (flagsByte >> 1) & 1;
    flags->Door3         = (flagsByte >> 2) & 1;
    flags->Door4         = (flagsByte >> 3) & 1;
    flags->BossBattleWon = (flagsByte >> 4) & 1;
    flags->Key           = (flagsByte >> 5) & 1;
    flags->KeyOrChest    = (flagsByte >> 6) & 1;
    flags->ChestOrTile   = (flagsByte >> 7) & 1;

    return flags;
}

ALTTPOverworldEvent* ALTTPFileReader::readOverworldEvent()
{
    ALTTPOverworldEvent* event = new ALTTPOverworldEvent;
    atUint8 flagsByte = base::readUByte();
    event->Unused1    = flagsByte & 1;
    event->HeartPiece = (flagsByte >> 1) & 1;
    event->Overlay    = (flagsByte >> 2) & 1;
    event->Unused2    = (flagsByte >> 3) & 1;
    event->Unused3    = (flagsByte >> 4) & 1;
    event->Unused4    = (flagsByte >> 5) & 1;
    event->Set        = (flagsByte >> 6) & 1;
    event->Unused5    = (flagsByte >> 7) & 1;
    return event;
}

ALTTPDungeonItemFlags ALTTPFileReader::readDungeonFlags()
{
    ALTTPDungeonItemFlags flags;
    atUint8 flagsByte     = base::readUByte();
    flags.Unused1         = flagsByte & 1;
    flags.Unused2         = (flagsByte >> 1) & 1;
    flags.GanonsTower     = (flagsByte >> 2) & 1;
    flags.TurtleRock      = (flagsByte >> 3) & 1;
    flags.GargoylesDomain = (flagsByte >> 4) & 1;
    flags.TowerOfHera     = (flagsByte >> 5) & 1;
    flags.IcePalace       = (flagsByte >> 6) & 1;
    flags.SkullWoods      = (flagsByte >> 7) & 1;
    flagsByte             = base::readUByte();
    flags.MiseryMire      = flagsByte & 1;
    flags.DarkPalace      = (flagsByte >> 1) & 1;
    flags.SwampPalace     = (flagsByte >> 2) & 1;
    flags.HyruleCastle2   = (flagsByte >> 3) & 1;
    flags.DesertPalace    = (flagsByte >> 4) & 1;
    flags.EasternPalace   = (flagsByte >> 5) & 1;
    flags.HyruleCastle    = (flagsByte >> 6) & 1;
    flags.SewerPassage    = (flagsByte >> 7) & 1;

    aDebug() << std::hex << flags.flags1 << " " << flags.flags2 << std::dec << std::endl;
    return flags;
}

} // io
} // zelda

#endif // ATHENA_NO_SAVES
