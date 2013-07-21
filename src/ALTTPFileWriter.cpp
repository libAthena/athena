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

#include "ALTTPFileWriter.hpp"
#include "ALTTPFile.hpp"
#include "ALTTPQuest.hpp"
#include <iostream>

namespace zelda
{

ALTTPFileWriter::ALTTPFileWriter(Uint8* data, Uint64 length)
    : BinaryWriter(data, length)
{
}

ALTTPFileWriter::ALTTPFileWriter(const std::string& filename)
    : BinaryWriter(filename)
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
            this->writeRoomFlags(quest->roomFlags(j));
        }

        for (int j = 0; j < 0x0C0; j++)
        {
            this->writeOverworldEvent(quest->overworldEvent(j));
        }

        this->writeBytes((Int8*)quest->inventory(), sizeof(ALTTPInventory));
        this->writeUInt16(quest->rupeeMax());
        this->writeUInt16(quest->rupeeCurrent());
        this->writeDungeonItems(quest->compasses());
        this->writeDungeonItems(quest->bigKeys());
        this->writeDungeonItems(quest->dungeonMaps());
        this->writeUInt16(quest->wishingPond());
        this->writeByte(quest->healthMax());
        this->writeByte(quest->health());
        this->writeByte(quest->magicPower());
        this->writeByte(quest->keys());
        this->writeByte(quest->bombUpgrades());
        this->writeByte(quest->arrowUpgrades());
        this->writeByte(quest->healthFiller());
        this->writeByte(quest->magicFiller());
        ALTTPPendants pendants = quest->pendants();
        this->writeBit(pendants.Courage);
        this->writeBit(pendants.Wisdom);
        this->writeBit(pendants.Power);
        this->writeByte(quest->bombFiller());
        this->writeByte(quest->arrowFiller());
        this->writeByte(quest->arrows());
        this->seek(1);
        ALTTPAbilities abilities = quest->abilityFlags();
        this->writeBit(abilities.Nothing);
        this->writeBit(abilities.Swim);
        this->writeBit(abilities.Dash);
        this->writeBit(abilities.Pull);
        this->writeBit(abilities.Unknown1);
        this->writeBit(abilities.Talk);
        this->writeBit(abilities.Read);
        this->writeBit(abilities.Unknown2);
        ALTTPCrystals crystals = quest->crystals();
        this->writeBytes((Int8*)&crystals, sizeof(ALTTPCrystals));
        ALTTPMagicUsage magicUsage = quest->magicUsage();
        this->writeBytes((Int8*)&magicUsage, sizeof(ALTTPMagicUsage));

        for (int j = 0; j < 0x010; j++)
            this->writeByte(quest->dungeonKeys(j));

        seek(0x039);
        this->writeByte((Int8)quest->progressIndicator());
        ALTTPProgressFlags1 progress1 = quest->progressFlags1();
        this->writeBytes((Int8*)&progress1, sizeof(ALTTPProgressFlags1));
        this->writeByte(quest->mapIcon());
        this->writeByte(quest->startLocation());
        ALTTPProgressFlags2 progress2 = quest->progressFlags2();
        this->writeBytes((Int8*)&progress2, sizeof(ALTTPProgressFlags2));
        ALTTPLightDarkWorldIndicator indicator = quest->lightDarkWorldIndicator();
        this->writeBytes((Int8*)&indicator, 1);
        this->seek(1);
        this->writeByte(quest->tagAlong());

        for(int j = 0; j < 6; j++)
            this->writeByte(quest->oldManFlag(j));

        this->writeByte(quest->bombFlag());

        for (int j = 0; j < 5; j++)
            this->writeByte(quest->unknown1(j));

        for (int j = 0; j < 6; j++)
            this->writeUInt16(quest->playerName()[j]);

        this->writeUInt16((quest->valid() == true ? 0x55AA : 0));

        for (int j = 0; j < 0x0D; j++)
            this->writeUInt16(quest->dungeonDeathTotal(j));


        this->writeUInt16(quest->unknown2());
        this->writeUInt16(quest->deathSaveCount());
        this->writeUInt16(quest->postGameDeathCounter());

        this->seek(0xF7);
        this->writeUInt16(calculateChecksum(i));
    }
}

void ALTTPFileWriter::writeRoomFlags(ALTTPRoomFlags* flags)
{
    writeBit(flags->Chest1);
    writeBit(flags->Chest2);
    writeBit(flags->Chest3);
    writeBit(flags->Chest4);
    writeBit(flags->Quadrant1);
    writeBit(flags->Quadrant2);
    writeBit(flags->Quadrant3);
    writeBit(flags->Quadrant4);
    writeBit(flags->Door1);
    writeBit(flags->Door2);
    writeBit(flags->Door3);
    writeBit(flags->Door4);
    writeBit(flags->BossBattleWon);
    writeBit(flags->Key);
    writeBit(flags->KeyOrChest);
    writeBit(flags->ChestOrTile);
}

void ALTTPFileWriter::writeOverworldEvent(ALTTPOverworldEvent* event)
{
    writeBit(event->Unused1);
    writeBit(event->HeartPiece);
    writeBit(event->Overlay);
    writeBit(event->Unused2);
    writeBit(event->Unused3);
    writeBit(event->Unused4);
    writeBit(event->Set);
    writeBit(event->Unused5);
}

void ALTTPFileWriter::writeDungeonItems(ALTTPDungeonItemFlags flags)
{
    writeBit(flags.Unused1);
    writeBit(flags.Unused2);
    writeBit(flags.GanonsTower);
    writeBit(flags.TurtleRock);
    writeBit(flags.TowerOfHera);
    writeBit(flags.IcePalace);
    writeBit(flags.SkullWoods);
    writeBit(flags.MiseryMire);
    writeBit(flags.DarkPalace);
    writeBit(flags.SwampPalace);
    writeBit(flags.HyruleCastle2);
    writeBit(flags.DesertPalace);
    writeBit(flags.EasternPalace);
    writeBit(flags.HyruleCastle);
    writeBit(flags.SewerPassage);
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

} // zelda
