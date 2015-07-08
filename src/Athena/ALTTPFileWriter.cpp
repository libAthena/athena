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

#include "Athena/ALTTPFileWriter.hpp"
#include "Athena/ALTTPFile.hpp"
#include "Athena/ALTTPQuest.hpp"
#include <iostream>

namespace Athena
{

namespace io
{

ALTTPFileWriter::ALTTPFileWriter(atUint8* data, atUint64 length)
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

    for (atUint32 i = 0; i < 6; i++)
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

        base::writeBytes((atInt8*)quest->inventory(), sizeof(ALTTPInventory));
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
        atUint8 pendantsByte = 0;
        pendantsByte |= pendants.Courage;
        pendantsByte |= pendants.Wisdom << 1;
        pendantsByte |= pendants.Power << 2;
        base::writeUByte(pendantsByte);
        base::writeByte(quest->bombFiller());
        base::writeByte(quest->arrowFiller());
        base::writeByte(quest->arrows());
        base::seek(1);
        ALTTPAbilities abilities = quest->abilityFlags();
        atUint8 abilitiesByte = 0;
        abilitiesByte |= abilities.Nothing;
        abilitiesByte |= abilities.Swim << 1;
        abilitiesByte |= abilities.Dash << 2;
        abilitiesByte |= abilities.Pull << 3;
        abilitiesByte |= abilities.Unknown1 << 4;
        abilitiesByte |= abilities.Talk << 5;
        abilitiesByte |= abilities.Read << 6;
        abilitiesByte |= abilities.Unknown2 << 7;
        base::writeUByte(abilitiesByte);
        ALTTPCrystals crystals = quest->crystals();
        base::writeBytes((atInt8*)&crystals, sizeof(ALTTPCrystals));
        ALTTPMagicUsage magicUsage = quest->magicUsage();
        base::writeBytes((atInt8*)&magicUsage, sizeof(ALTTPMagicUsage));

        for (int j = 0; j < 0x010; j++)
            base::writeByte(quest->dungeonKeys(j));

        base::seek(0x039);
        base::writeByte((atInt8)quest->progressIndicator());
        ALTTPProgressFlags1 progress1 = quest->progressFlags1();
        base::writeBytes((atInt8*)&progress1, sizeof(ALTTPProgressFlags1));
        base::writeByte(quest->mapIcon());
        base::writeByte(quest->startLocation());
        ALTTPProgressFlags2 progress2 = quest->progressFlags2();
        base::writeBytes((atInt8*)&progress2, sizeof(ALTTPProgressFlags2));
        ALTTPLightDarkWorldIndicator indicator = quest->lightDarkWorldIndicator();
        base::writeBytes((atInt8*)&indicator, 1);
        base::seek(1);
        base::writeByte(quest->tagAlong());

        for (int j = 0; j < 6; j++)
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
    atUint8 flagsByte = 0;
    flagsByte |= flags->Chest1;
    flagsByte |= flags->Chest2 << 1;
    flagsByte |= flags->Chest3 << 2;
    flagsByte |= flags->Chest4 << 3;
    flagsByte |= flags->Quadrant1 << 4;
    flagsByte |= flags->Quadrant2 << 5;
    flagsByte |= flags->Quadrant3 << 6;
    flagsByte |= flags->Quadrant4 << 7;
    base::writeUByte(flagsByte);
    flagsByte = 0;
    flagsByte |= flags->Door1;
    flagsByte |= flags->Door2 << 1;
    flagsByte |= flags->Door3 << 2;
    flagsByte |= flags->Door4 << 3;
    flagsByte |= flags->BossBattleWon << 4;
    flagsByte |= flags->Key << 5;
    flagsByte |= flags->KeyOrChest << 6;
    flagsByte |= flags->ChestOrTile << 7;
    base::writeUByte(flagsByte);
}

void ALTTPFileWriter::writeOverworldEvent(ALTTPOverworldEvent* event)
{
    atUint8 flagsByte = 0;
    flagsByte |= event->Unused1;
    flagsByte |= event->HeartPiece << 1;
    flagsByte |= event->Overlay << 2;
    flagsByte |= event->Unused2 << 3;
    flagsByte |= event->Unused3 << 4;
    flagsByte |= event->Unused4 << 5;
    flagsByte |= event->Set << 6;
    flagsByte |= event->Unused5 << 7;
    base::writeUByte(flagsByte);
}

void ALTTPFileWriter::writeDungeonItems(ALTTPDungeonItemFlags flags)
{
    atUint8 flagsByte = 0;
    flagsByte |= flags.Unused1;
    flagsByte |= flags.Unused2 << 1;
    flagsByte |= flags.GanonsTower << 2;
    flagsByte |= flags.TurtleRock << 3;
    flagsByte |= flags.TowerOfHera << 4;
    flagsByte |= flags.IcePalace << 5;
    flagsByte |= flags.SkullWoods << 6;
    flagsByte |= flags.MiseryMire << 7;
    base::writeUByte(flagsByte);
    flagsByte = 0;
    flagsByte |= flags.DarkPalace;
    flagsByte |= flags.SwampPalace << 1;
    flagsByte |= flags.HyruleCastle2 << 2;
    flagsByte |= flags.DesertPalace << 3;
    flagsByte |= flags.EasternPalace << 4;
    flagsByte |= flags.HyruleCastle << 5;
    flagsByte |= flags.SewerPassage << 6;
    base::writeUByte(flagsByte);
}

atUint16 ALTTPFileWriter::calculateChecksum(atUint32 game)
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
    atUint16 sum = 0;

    for (atUint32 i = 0; i < 0x4FE; i += 2)
        // Add each word one by one
        sum += *(atUint16*)(m_data + (i + (0x500 * game)));

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
#endif // ATHENA_NO_SAVES
