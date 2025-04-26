#include "athena/ALTTPQuest.hpp"
#include <iostream>

namespace athena {

ALTTPQuest::ALTTPQuest() {}

ALTTPQuest::~ALTTPQuest() {
  m_roomFlags.clear();
  m_overworldEvents.clear();
  m_oldManFlags.clear();
  m_playerName.clear();
  m_unknown1.clear();
}

void ALTTPQuest::setRoomFlags(std::vector<ALTTPRoomFlags*> rf) { m_roomFlags = rf; }

void ALTTPQuest::setRoomFlags(ALTTPRoomFlags* rf, uint32_t id) { m_roomFlags[id] = rf; }

std::vector<ALTTPRoomFlags*> ALTTPQuest::roomFlags() { return m_roomFlags; }

ALTTPRoomFlags* ALTTPQuest::roomFlags(uint32_t id) { return m_roomFlags[id]; }

void ALTTPQuest::setOverworldEvents(std::vector<ALTTPOverworldEvent*> ow) { m_overworldEvents = ow; }

void ALTTPQuest::setOverworldEvents(ALTTPOverworldEvent* ow, uint32_t id) { m_overworldEvents[id] = ow; }

std::vector<ALTTPOverworldEvent*> ALTTPQuest::overworldEvents() const { return m_overworldEvents; }

ALTTPOverworldEvent* ALTTPQuest::overworldEvent(uint32_t id) const {
  if (id > m_overworldEvents.size() - 1) {
    atWarning("index out of range");
    return nullptr;
  }

  return m_overworldEvents[id];
}

void ALTTPQuest::setInventory(const ALTTPInventory& inv) { m_inventory = inv; }

const ALTTPInventory& ALTTPQuest::inventory() const { return m_inventory; }

void ALTTPQuest::setRupeeMax(uint16_t val) { m_rupeeMax = val; }

uint16_t ALTTPQuest::rupeeMax() const { return m_rupeeMax; }

void ALTTPQuest::setRupeeCurrent(uint16_t val) { m_rupeeCurrent = val; }

uint16_t ALTTPQuest::rupeeCurrent() const { return m_rupeeCurrent; }

void ALTTPQuest::setCompasses(ALTTPDungeonItemFlags flags) { m_compasses = flags; }

ALTTPDungeonItemFlags ALTTPQuest::compasses() const { return m_compasses; }

void ALTTPQuest::setBigKeys(ALTTPDungeonItemFlags flags) { m_bigKeys = flags; }

ALTTPDungeonItemFlags ALTTPQuest::bigKeys() const { return m_bigKeys; }

void ALTTPQuest::setDungeonMaps(ALTTPDungeonItemFlags flags) { m_dungeonMaps = flags; }

ALTTPDungeonItemFlags ALTTPQuest::dungeonMaps() const { return m_dungeonMaps; }

void ALTTPQuest::setWishingPond(uint16_t val) { m_wishingPond = val; }

uint16_t ALTTPQuest::wishingPond() const { return m_wishingPond; }

void ALTTPQuest::setHealthMax(uint8_t val) { m_healthMax = val; }

uint8_t ALTTPQuest::healthMax() const { return m_healthMax; }

void ALTTPQuest::setHealth(uint8_t val) { m_health = val; }

uint8_t ALTTPQuest::health() const { return m_health; }

void ALTTPQuest::setMagicPower(uint8_t val) { m_magicPower = val; }

uint8_t ALTTPQuest::magicPower() const { return m_magicPower; }

void ALTTPQuest::setKeys(uint8_t val) { m_keys = val; }

uint8_t ALTTPQuest::keys() const { return m_keys; }

void ALTTPQuest::setBombUpgrades(uint8_t val) { m_bombUpgrades = val; }

uint8_t ALTTPQuest::bombUpgrades() const { return m_bombUpgrades; }

void ALTTPQuest::setArrowUpgrades(uint8_t val) { m_arrowUpgrades = val; }

uint8_t ALTTPQuest::arrowUpgrades() const { return m_arrowUpgrades; }

void ALTTPQuest::setHealthFiller(uint8_t val) { m_heartFiller = val; }

uint8_t ALTTPQuest::healthFiller() const { return m_heartFiller; }
void ALTTPQuest::setMagicFiller(uint8_t val) { m_heartFiller = val; }

uint8_t ALTTPQuest::magicFiller() const { return m_heartFiller; }

void ALTTPQuest::setPendants(ALTTPPendants val) { m_pendants = val; }

ALTTPPendants ALTTPQuest::pendants() const { return m_pendants; }

void ALTTPQuest::setBombFiller(uint8_t val) { m_bombFiller = val; }

uint8_t ALTTPQuest::bombFiller() const { return m_bombFiller; }

void ALTTPQuest::setArrowFiller(uint8_t val) { m_arrowFiller = val; }

uint8_t ALTTPQuest::arrowFiller() const { return m_arrowFiller; }
void ALTTPQuest::setArrows(uint8_t val) { m_arrows = val; }

uint8_t ALTTPQuest::arrows() const { return m_arrows; }

void ALTTPQuest::setAbilityFlags(ALTTPAbilities val) { m_abilityFlags = val; }

ALTTPAbilities ALTTPQuest::abilityFlags() const { return m_abilityFlags; }

void ALTTPQuest::setCrystals(ALTTPCrystals val) { m_crystals = val; }

ALTTPCrystals ALTTPQuest::crystals() const { return m_crystals; }

void ALTTPQuest::setMagicUsage(ALTTPMagicUsage val) { m_magicUsage = val; }

ALTTPMagicUsage ALTTPQuest::magicUsage() const { return m_magicUsage; }

void ALTTPQuest::setDungeonKeys(std::vector<uint8_t> val) { m_dungeonKeys = val; }

void ALTTPQuest::setDungeonKeys(uint32_t id, uint8_t val) {
  if (id > m_dungeonKeys.size() - 1) {
    atWarning("index out of range");
    return;
  }

  m_dungeonKeys[id] = val;
}

uint8_t ALTTPQuest::dungeonKeys(uint32_t id) const {
  if (id > m_dungeonKeys.size() - 1) {
    atWarning("index out of range");
    return 0;
  }

  return m_dungeonKeys[id];
}

uint32_t ALTTPQuest::dungeonCount() const { return (uint32_t)m_dungeonKeys.size(); }

void ALTTPQuest::setProgressIndicator(ALTTPProgressIndicator val) { m_progressIndicator = val; }

ALTTPProgressIndicator ALTTPQuest::progressIndicator() const { return m_progressIndicator; }

void ALTTPQuest::setProgressFlags1(ALTTPProgressFlags1 val) { m_progressFlags1 = val; }

ALTTPProgressFlags1 ALTTPQuest::progressFlags1() const { return m_progressFlags1; }

void ALTTPQuest::setMapIcon(ALTTPMapIcon val) { m_mapIcon = val; }

ALTTPMapIcon ALTTPQuest::mapIcon() const { return m_mapIcon; }

void ALTTPQuest::setStartLocation(ALTTPStartLocation val) { m_startLocation = val; }

ALTTPStartLocation ALTTPQuest::startLocation() const { return m_startLocation; }

void ALTTPQuest::setProgressFlags2(ALTTPProgressFlags2 val) { m_progressFlags2 = val; }

ALTTPProgressFlags2 ALTTPQuest::progressFlags2() const { return m_progressFlags2; }

void ALTTPQuest::setLightDarkWorldIndicator(ALTTPLightDarkWorldIndicator val) { m_lightDarkWorldIndicator = val; }

ALTTPLightDarkWorldIndicator ALTTPQuest::lightDarkWorldIndicator() const { return m_lightDarkWorldIndicator; }

void ALTTPQuest::setTagAlong(ALTTPTagAlong val) { m_tagAlong = val; }

ALTTPTagAlong ALTTPQuest::tagAlong() const { return m_tagAlong; }

void ALTTPQuest::setOldManFlags(std::vector<uint8_t> flags) { m_oldManFlags = flags; }

void ALTTPQuest::setOldManFlag(uint32_t id, uint8_t val) {
  if (id > m_oldManFlags.size() - 1) {
    atWarning("index out of range");
    return;
  }

  m_oldManFlags[id] = val;
}

uint8_t ALTTPQuest::oldManFlag(uint32_t id) {
  if (id > m_oldManFlags.size() - 1) {
    atWarning("index out of range");
    return 0;
  }
  return m_oldManFlags[id];
}

uint32_t ALTTPQuest::oldManFlagCount() const { return (uint32_t)m_oldManFlags.size(); }

void ALTTPQuest::setBombFlag(uint8_t flag) { m_bombFlag = flag; }

uint8_t ALTTPQuest::bombFlag() const { return m_bombFlag; }

void ALTTPQuest::setUnknown1(std::vector<uint8_t> flags) { m_unknown1 = flags; }

void ALTTPQuest::setUnknown1(uint32_t id, uint8_t val) {
  if (id > m_unknown1.size()) {
    atWarning("index out of range");
    return;
  }

  m_unknown1[id] = val;
}

uint8_t ALTTPQuest::unknown1(uint32_t id) {
  if (id > m_unknown1.size()) {
    atWarning("index out of range");
    return 0;
  }

  return m_unknown1[id];
}

uint32_t ALTTPQuest::unknown1Count() const { return (uint32_t)m_unknown1.size(); }

void ALTTPQuest::setPlayerName(std::vector<uint16_t> playerName) { m_playerName = playerName; }

void ALTTPQuest::setPlayerName(const std::string& playerName) {
  if (playerName == std::string() || playerName.size() > 6) {
    atWarning("index out of range");
    return;
  }

  m_playerName.clear();

  for (uint32_t i = 0; i < 6; i++) {
    if (i > playerName.size() - 1) {
      m_playerName.push_back((uint16_t)0xA9);
      continue;
    }

    char c = playerName[i];

    if (c >= 'A' && c <= 'P' && c != 'I') {

      m_playerName.push_back((uint16_t)(c - 'A'));
      continue;
    }

    if (c >= 'Q' && c <= 'Z') {
      std::cout << std::hex << (uint16_t)((c - 'Q') + 0x20) << std::endl;
      m_playerName.push_back((uint16_t)((c - 'Q') + 0x20));
      continue;
    }

    if (c >= 'a' && c <= 'f') {
      std::cout << std::hex << (uint16_t)((c - 'a') + 0x2A) << std::endl;
      m_playerName.push_back((uint16_t)((c - 'a') + 0x2A));
      continue;
    }

    if (c >= 'g' && c <= 'v') {
      if (c == 'k') {
        m_playerName.push_back(0x42);
        continue;
      }

      if (c == 'i') {
        m_playerName.push_back(0x44);
        continue;
      }

      m_playerName.push_back((uint16_t)((c - 'g') + 0x40));
      continue;
    }

    if (c >= 'w' && c <= 'z') {
      m_playerName.push_back((uint16_t)((c - 'w') + 0x60));
      continue;
    }

    if (c >= '0' && c <= '9') {
      m_playerName.push_back((uint16_t)((c - '0') + 0x64));
      continue;
    }

    if (c == '-' || c == '.') {
      m_playerName.push_back((uint16_t)(c - '-') + 0x80);
      continue;
    }

    switch (c) {
    case '?':
      m_playerName.push_back(0x6E);
      break;

    case '!':
      m_playerName.push_back(0x6F);
      break;

    case ',':
      m_playerName.push_back(0x82);
      break;

    case '(':
      m_playerName.push_back(0x85);
      break;

    case ')':
      m_playerName.push_back(0x86);
      break;

    case 'I':
      m_playerName.push_back(0xAF);
      break;
    }
  }
}

std::vector<uint16_t> ALTTPQuest::playerName() const { return m_playerName; }

std::string ALTTPQuest::playerNameToString() const {
  std::string ret;

  for (int16_t c : m_playerName) {
    if (c >= 0x00 && c <= 0x0F) {
      ret.push_back((char)('A' + c));
      continue;
    }

    if (c >= 0x20 && c <= 0x29) {
      ret.push_back((char)('Q' + (c - 0x20)));
      continue;
    }

    if (c >= 0x2A && c <= 0x2F) {
      ret.push_back((char)('a' + (c - 0x2A)));
      continue;
    }

    if (c >= 0x40 && c <= 0x4F) {
      if (c == 0x42) {
        ret.push_back('k');
        continue;
      }

      if (c == 0x44) {
        ret.push_back('i');
        continue;
      }

      ret.push_back((char)('g' + (c - 0x40)));
    }

    if (c >= 0x60 && c <= 0x63) {
      ret.push_back((char)('w' + (c - 0x60)));
      continue;
    }

    if (c >= 0x64 && c <= 0x6D) {
      ret.push_back((char)('0' + (c - 0x64)));
      continue;
    }

    if (c == 0x80 || c == 0x81) {
      ret.push_back((char)('-' + (c - 0x80)));
      continue;
    }

    switch (c) {
    case 0x6E:
      ret.push_back('?');
      break;

    case 0x6F:
      ret.push_back('!');
      break;

    case 0x82:
      ret.push_back(',');
      break;

    case 0x85:
      ret.push_back('(');
      break;

    case 0x86:
      ret.push_back(')');
      break;

    case 0xAF:
      ret.push_back('I');
      break;
    }
  }

  return ret;
}

void ALTTPQuest::setValid(bool val) { m_valid = val; }

bool ALTTPQuest::valid() { return m_valid; }

void ALTTPQuest::setDungeonDeathTotals(std::vector<uint16_t> val) { m_dungeonDeathTotals = val; }

void ALTTPQuest::setDungeonDeathTotal(uint32_t id, uint16_t val) {
  if (id > m_dungeonDeathTotals.size()) {
    atWarning("index out of range");
    return;
  }

  m_dungeonDeathTotals[id] = val;
}

uint16_t ALTTPQuest::dungeonDeathTotal(uint32_t id) const {
  if (id > m_dungeonDeathTotals.size()) {
    atWarning("index out of range");
    return 0;
  }

  return m_dungeonDeathTotals[id];
}

uint16_t ALTTPQuest::dungeonDeathTotalCount() const { return (uint16_t)m_dungeonDeathTotals.size(); }

void ALTTPQuest::setUnknown2(uint16_t val) { m_unknown2 = val; }

uint16_t ALTTPQuest::unknown2() const { return m_unknown2; }

void ALTTPQuest::setDeathSaveCount(uint16_t val) { m_deathSaveCount = val; }
uint16_t ALTTPQuest::deathSaveCount() const { return m_deathSaveCount; }

void ALTTPQuest::setPostGameDeathCounter(int16_t val) { m_postGameDeathCounter = val; }

int16_t ALTTPQuest::postGameDeathCounter() const { return m_postGameDeathCounter; }

void ALTTPQuest::setChecksum(uint16_t checksum) { m_checksum = checksum; }

uint16_t ALTTPQuest::checksum() const { return m_checksum; }

} // namespace athena
