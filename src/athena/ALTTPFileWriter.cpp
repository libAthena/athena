#include "athena/ALTTPFileWriter.hpp"
#include "athena/ALTTPFile.hpp"
#include "athena/ALTTPQuest.hpp"
#include <iostream>

namespace athena {

namespace io {

ALTTPFileWriter::ALTTPFileWriter(uint8_t* data, uint64_t length) : MemoryCopyWriter(data, length) {}

ALTTPFileWriter::ALTTPFileWriter(const std::string& filename) : MemoryCopyWriter(filename) {}

void ALTTPFileWriter::writeFile(ALTTPFile* file) {
  ALTTPQuest* quest = NULL;

  for (uint32_t i = 0; i < 6; i++) {
    if (i < 3)
      quest = file->quest(i);
    else
      quest = file->quest(i - 3);

    for (int j = 0; j < 0x140; j++) {
      writeRoomFlags(quest->roomFlags(j));
    }

    for (int j = 0; j < 0x0C0; j++) {
      writeOverworldEvent(quest->overworldEvent(j));
    }

    writeBytes((int8_t*)&quest->inventory(), sizeof(ALTTPInventory));
    writeUint16(quest->rupeeMax());
    writeUint16(quest->rupeeCurrent());
    writeDungeonItems(quest->compasses());
    writeDungeonItems(quest->bigKeys());
    writeDungeonItems(quest->dungeonMaps());
    writeUint16(quest->wishingPond());
    writeByte(quest->healthMax());
    writeByte(quest->health());
    writeByte(quest->magicPower());
    writeByte(quest->keys());
    writeByte(quest->bombUpgrades());
    writeByte(quest->arrowUpgrades());
    writeByte(quest->healthFiller());
    writeByte(quest->magicFiller());
    ALTTPPendants pendants = quest->pendants();
    uint8_t pendantsByte = 0;
    pendantsByte |= pendants.Courage;
    pendantsByte |= pendants.Wisdom << 1;
    pendantsByte |= pendants.Power << 2;
    writeUByte(pendantsByte);
    writeByte(quest->bombFiller());
    writeByte(quest->arrowFiller());
    writeByte(quest->arrows());
    seek(1);
    ALTTPAbilities abilities = quest->abilityFlags();
    uint8_t abilitiesByte = 0;
    abilitiesByte |= abilities.Nothing;
    abilitiesByte |= abilities.Swim << 1;
    abilitiesByte |= abilities.Dash << 2;
    abilitiesByte |= abilities.Pull << 3;
    abilitiesByte |= abilities.Unknown1 << 4;
    abilitiesByte |= abilities.Talk << 5;
    abilitiesByte |= abilities.Read << 6;
    abilitiesByte |= abilities.Unknown2 << 7;
    writeUByte(abilitiesByte);
    ALTTPCrystals crystals = quest->crystals();
    writeBytes((int8_t*)&crystals, sizeof(ALTTPCrystals));
    ALTTPMagicUsage magicUsage = quest->magicUsage();
    writeBytes((int8_t*)&magicUsage, sizeof(ALTTPMagicUsage));

    for (int j = 0; j < 0x010; j++)
      writeByte(quest->dungeonKeys(j));

    seek(0x039);
    writeByte((int8_t)quest->progressIndicator());
    ALTTPProgressFlags1 progress1 = quest->progressFlags1();
    writeBytes((int8_t*)&progress1, sizeof(ALTTPProgressFlags1));
    writeByte(quest->mapIcon());
    writeByte(quest->startLocation());
    ALTTPProgressFlags2 progress2 = quest->progressFlags2();
    writeBytes((int8_t*)&progress2, sizeof(ALTTPProgressFlags2));
    ALTTPLightDarkWorldIndicator indicator = quest->lightDarkWorldIndicator();
    writeBytes((int8_t*)&indicator, 1);
    seek(1);
    writeByte(quest->tagAlong());

    for (int j = 0; j < 6; j++)
      writeByte(quest->oldManFlag(j));

    writeByte(quest->bombFlag());

    for (int j = 0; j < 5; j++)
      writeByte(quest->unknown1(j));

    for (int j = 0; j < 6; j++)
      writeUint16(quest->playerName()[j]);

    writeUint16((quest->valid() == true ? 0x55AA : 0));

    for (int j = 0; j < 0x0D; j++)
      writeUint16(quest->dungeonDeathTotal(j));

    writeUint16(quest->unknown2());
    writeUint16(quest->deathSaveCount());
    writeUint16(quest->postGameDeathCounter());

    seek(0xF7);
    writeUint16(calculateChecksum(i));
  }
}

void ALTTPFileWriter::writeRoomFlags(ALTTPRoomFlags* flags) {
  uint8_t flagsByte = 0;
  flagsByte |= flags->Chest1;
  flagsByte |= flags->Chest2 << 1;
  flagsByte |= flags->Chest3 << 2;
  flagsByte |= flags->Chest4 << 3;
  flagsByte |= flags->Quadrant1 << 4;
  flagsByte |= flags->Quadrant2 << 5;
  flagsByte |= flags->Quadrant3 << 6;
  flagsByte |= flags->Quadrant4 << 7;
  writeUByte(flagsByte);
  flagsByte = 0;
  flagsByte |= flags->Door1;
  flagsByte |= flags->Door2 << 1;
  flagsByte |= flags->Door3 << 2;
  flagsByte |= flags->Door4 << 3;
  flagsByte |= flags->BossBattleWon << 4;
  flagsByte |= flags->Key << 5;
  flagsByte |= flags->KeyOrChest << 6;
  flagsByte |= flags->ChestOrTile << 7;
  writeUByte(flagsByte);
}

void ALTTPFileWriter::writeOverworldEvent(ALTTPOverworldEvent* event) {
  uint8_t flagsByte = 0;
  flagsByte |= event->Unused1;
  flagsByte |= event->HeartPiece << 1;
  flagsByte |= event->Overlay << 2;
  flagsByte |= event->Unused2 << 3;
  flagsByte |= event->Unused3 << 4;
  flagsByte |= event->Unused4 << 5;
  flagsByte |= event->Set << 6;
  flagsByte |= event->Unused5 << 7;
  writeUByte(flagsByte);
}

void ALTTPFileWriter::writeDungeonItems(ALTTPDungeonItemFlags flags) {
  uint8_t flagsByte = 0;
  flagsByte |= flags.Unused1;
  flagsByte |= flags.Unused2 << 1;
  flagsByte |= flags.GanonsTower << 2;
  flagsByte |= flags.TurtleRock << 3;
  flagsByte |= flags.TowerOfHera << 4;
  flagsByte |= flags.IcePalace << 5;
  flagsByte |= flags.SkullWoods << 6;
  flagsByte |= flags.MiseryMire << 7;
  writeUByte(flagsByte);
  flagsByte = 0;
  flagsByte |= flags.DarkPalace;
  flagsByte |= flags.SwampPalace << 1;
  flagsByte |= flags.HyruleCastle2 << 2;
  flagsByte |= flags.DesertPalace << 3;
  flagsByte |= flags.EasternPalace << 4;
  flagsByte |= flags.HyruleCastle << 5;
  flagsByte |= flags.SewerPassage << 6;
  writeUByte(flagsByte);
}

uint16_t ALTTPFileWriter::calculateChecksum(uint32_t game) {
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
  uint16_t sum = 0;

  for (uint32_t i = 0; i < 0x4FE; i += 2)
    // Add each word one by one
    sum += *(uint16_t*)(m_data + (i + (0x500 * game)));

  // Subtract it from 0x5a5a to get our true checksum
  return (0x5a5a - sum);

  /*
   * There is one caveat to this algorithm however,
   * It makes it difficult to manually edit this in a hex editor since it's not a common
   * algorithm and most hexeditor with built in checksum calculators won't have it, however it's
   * it's extremely basic, making it a non-issue really.
   */
}

} // namespace io
} // namespace athena
