#pragma once

#include "Types.hpp"

namespace athena {

enum BowType : char { BowNone, BowArrows, BowSilverArrows };

enum BoomerangType : char { BoomerangNone, BoomerangBlue, BoomerangRed };

enum MagicType : char { MagicNone, MagicMushroom, MagicPowder };

enum ArmorType : char { GreenJerkin, BlueMail, RedMail };

enum BottleType : char {
  BottleNone,
  BottleMushroom, // No Use
  BottleEmpty,
  BottleRedPotion,
  BottleBluePotion,
  BottleFairy,
  BottleBee,
  BottleGoodBee
};

enum ALTTPStartLocation { LinksHouse = 0x00, Sanctuary = 0x01, Any = 0x05 };

enum ALTTPProgressIndicator { LinkInBed, InCastleWithSword, CompletedFirstDungeon, BeatenAghanim };

enum ALTTPMapIcon {
  Nothing = 0x00,             //?
  CrossInKakariko = 0x01,     //?
  CrossAtFirstDungeon = 0x02, //
  Pendant = 0x03,
  MasterSword = 0x04,
  AganhimCastle = 0x05,
  Crystal1 = 0x06,
  AllCrystals = 0x07,
  AganhimGanonTower = 0x08
};

enum ALTTPTagAlong {
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

} // namespace athena
