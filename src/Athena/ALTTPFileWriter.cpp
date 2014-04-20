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

#include "Athena/ALTTPFileWriter.hpp"
#include "Athena/ALTTPFile.hpp"
#include "Athena/ALTTPQuest.hpp"
#include <iostream>

namespace Athena
{

namespace io
{

ALTTPFileWriter::ALTTPFileWriter(Uint8* data, Uint64 length)
    : base(data, length)
{
}

ALTTPFileWriter::ALTTPFileWriter(const std::string& filename)
    : base(filename)
{
}

void ALTTPFileWriter::writeFile(ALTTPFile* file)
{
    ALTTPQuest* quest = NULL;
    for (Uint32 i = 0; i < 6; i++)
    {
        if (i < 3)
            quest = file->quest(i);
        else
            quest = file->quest(i - 3);

        for (int j = 0; j < 0x140; j++)
        {
            writeRoomFlags(quest->roomFlags(j));
        }

        for (int j = 0; j < 0x0C0; j++)
        {
            writeOverworldEvent(quest->overworldEvent(j));
        }

        base::writeBytes((Int8*)quest->inventory(), sizeof(ALTTPInventory));
        base::writeUint16(quest->rupeeMax());
        base::writeUint16(quest->rupeeCurrent());
        writeDungeonItems(quest->compasses());
        writeDungeonItems(quest->bigKeys());
        writeDungeonItems(quest->dungeonMaps());
        base::writeUint16(quest->wishingPond());
        base::writeByte(quest->healthMax());
        base::writeByte(quest->health());
        base::writeByte(quest->magicPower());
        base::writeByte(quest->keys());
        base::writeByte(quest->bombUpgrades());
        base::writeByte(quest->arrowUpgrades());
        base::writeByte(quest->healthFiller());
        base::writeByte(quest->magicFiller());
        ALTTPPendants pendants = quest->pendants();
        base::writeBit(pendants.Courage);
        base::writeBit(pendants.Wisdom);
        base::writeBit(pendants.Power);
        base::writeByte(quest->bombFiller());
        base::writeByte(quest->arrowFiller());
        base::writeByte(quest->arrows());
        base::seek(1);
        ALTTPAbilities abilities = quest->abilityFlags();
        base::writeBit(abilities.Nothing);
        base::writeBit(abilities.Swim);
        base::writeBit(abilities.Dash);
        base::writeBit(abilities.Pull);
        base::writeBit(abilities.Unknown1);
        base::writeBit(abilities.Talk);
        base::writeBit(abilities.Read);
        base::writeBit(abilities.Unknown2);
        ALTTPCrystals crystals = quest->crystals();
        base::writeBytes((Int8*)&crystals, sizeof(ALTTPCrystals));
        ALTTPMagicUsage magicUsage = quest->magicUsage();
        base::writeBytes((Int8*)&magicUsage, sizeof(ALTTPMagicUsage));

        for (int j = 0; j < 0x010; j++)
            base::writeByte(quest->dungeonKeys(j));

        base::seek(0x039);
        base::writeByte((Int8)quest->progressIndicator());
        ALTTPProgressFlags1 progress1 = quest->progressFlags1();
        base::writeBytes((Int8*)&progress1, sizeof(ALTTPProgressFlags1));
        base::writeByte(quest->mapIcon());
        base::writeByte(quest->startLocation());
        ALTTPProgressFlags2 progress2 = quest->progressFlags2();
        base::writeBytes((Int8*)&progress2, sizeof(ALTTPProgressFlags2));
        ALTTPLightDarkWorldIndicator indicator = quest->lightDarkWorldIndicator();
        base::writeBytes((Int8*)&indicator, 1);
        base::seek(1);
        base::writeByte(quest->tagAlong());

        for(int j = 0; j < 6; j++)
            base::writeByte(quest->oldManFlag(j));

        base::writeByte(quest->bombFlag());

        for (int j = 0; j < 5; j++)
            base::writeByte(quest->unknown1(j));

        for (int j = 0; j < 6; j++)
            base::writeUint16(quest->playerName()[j]);

        base::writeUint16((quest->valid() == true ? 0x55AA : 0));

        for (int j = 0; j < 0x0D; j++)
            base::writeUint16(quest->dungeonDeathTotal(j));


        base::writeUint16(quest->unknown2());
        base::writeUint16(quest->deathSaveCount());
        base::writeUint16(quest->postGameDeathCounter());

        base::seek(0xF7);
        base::writeUint16(calculateChecksum(i));
    }
}

void ALTTPFileWriter::writeRoomFlags(ALTTPRoomFlags* flags)
{
    base::writeBit(flags->Chest1);
    base::writeBit(flags->Chest2);
    base::writeBit(flags->Chest3);
    base::writeBit(flags->Chest4);
    base::writeBit(flags->Quadrant1);
    base::writeBit(flags->Quadrant2);
    base::writeBit(flags->Quadrant3);
    base::writeBit(flags->Quadrant4);
    base::writeBit(flags->Door1);
    base::writeBit(flags->Door2);
    base::writeBit(flags->Door3);
    base::writeBit(flags->Door4);
    base::writeBit(flags->BossBattleWon);
    base::writeBit(flags->Key);
    base::writeBit(flags->KeyOrChest);
    base::writeBit(flags->ChestOrTile);
}

void ALTTPFileWriter::writeOverworldEvent(ALTTPOverworldEvent* event)
{
    base::writeBit(event->Unused1);
    base::writeBit(event->HeartPiece);
    base::writeBit(event->Overlay);
    base::writeBit(event->Unused2);
    base::writeBit(event->Unused3);
    base::writeBit(event->Unused4);
    base::writeBit(event->Set);
    base::writeBit(event->Unused5);
}

void ALTTPFileWriter::writeDungeonItems(ALTTPDungeonItemFlags flags)
{
    base::writeBit(flags.Unused1);
    base::writeBit(flags.Unused2);
    base::writeBit(flags.GanonsTower);
    base::writeBit(flags.TurtleRock);
    base::writeBit(flags.TowerOfHera);
    base::writeBit(flags.IcePalace);
    base::writeBit(flags.SkullWoods);
    base::writeBit(flags.MiseryMire);
    base::writeBit(flags.DarkPalace);
    base::writeBit(flags.SwampPalace);
    base::writeBit(flags.HyruleCastle2);
    base::writeBit(flags.DesertPalace);
    base::writeBit(flags.EasternPalace);
    base::writeBit(flags.HyruleCastle);
    base::writeBit(flags.SewerPassage);
}

Uint16 ALTTPFileWriter::calculateChecksum(Uint32 game)
{
    /*
     * ALTTP's checksum is very basic
     * It adds each word up and then subtracts the sum from 0x5a5a
     * The number seems pretty arbitrary, but it enables the game to differentiate
     * it from a number that just happens to equal the sum outright, preventing "false positives."
     *
     * Ignoring the algorithm for figuring out it's position in the buffer the equation is basically:
     * s = s + w
     * s = (0x5a5a - s);
     * s == sum
     * w == current word
     *
     * For those who don't know a word is a two byte pair, i.e 0xFF and 0xFE constitutes a word.
     */

    // First we start at 0
    Uint16 sum = 0;
    for (Uint32 i = 0; i < 0x4FE; i += 2)
        // Add each word one by one
        sum += *(Uint16*)(m_data + (i + (0x500 * game)));

    // Subtract it from 0x5a5a to get our true checksum
    return (0x5a5a - sum);

    /*
     * There is one caveat to this algorithm however,
     * It makes it difficult to manually edit this in a hex editor since it's not a common
     * algorithm and most hexeditor with built in checksum calculators won't have it, however it's
     * it's extremely basic, making it a non-issue really.
     */
}

} // io
} // zelda
