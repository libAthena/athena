#include "athena/SkywardSwordQuest.hpp"
#include "athena/Checksums.hpp"
#include "athena/Utility.hpp"
#include "athena/utf8proc.h"
#include <sstream>

namespace athena {

namespace priv {
static const uint32_t NAME_OFFSET = 0x08D4;
static const uint32_t RUPEE_COUNT_OFFSET = 0x0A5E;
static const uint32_t AMMO_COUNT_OFFSET = 0x0A60;
static const uint32_t MAX_HP_OFFSET = 0x5302;
static const uint32_t SPAWN_HP_OFFSET = 0x5304;
static const uint32_t CURRENT_HP_OFFSET = 0x5306;
static const uint32_t ROOM_ID_OFFSET = 0x5309;
static const uint32_t CURRENT_LOCATION_OFFSET = 0x531C;
static const uint32_t CURRENT_AREA_OFFSET = 0x533C;
static const uint32_t CURRENT_LOCATION_COPY_OFFSET = 0x535C;
static const uint32_t CHECKSUM_OFFSET = 0x53BC;
static const uint32_t ISNEW_OFFSET = 0x53AD;

static const uint32_t SKIP_CHECKSUM_OFFSET = 0x20;
} // namespace priv

union AmmoValues {
  struct {
    uint32_t arrows : 7;
    uint32_t bombs : 7;
    uint32_t : 9;
    uint32_t seeds : 7;
    uint32_t : 2;
  };
  uint32_t value;
};

SkywardSwordQuest::SkywardSwordQuest(std::unique_ptr<uint8_t[]>&& data, uint32_t len)
: ZQuestFile(ZQuestFile::SS, Endian::Big, std::move(data), len) {}

void SkywardSwordQuest::setSkipData(std::unique_ptr<uint8_t[]>&& data) { m_skipData = std::move(data); }

uint8_t* SkywardSwordQuest::skipData() const { return m_skipData.get(); }

void SkywardSwordQuest::setPlayerName(const std::string& name) {
  if (name.length() > 8)
    atDebug("WARNING: name cannot be greater than 8 characters, automatically truncating");

  const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(name.c_str());
  for (uint32_t i = 0; i < 8; i++) {
    uint16_t& c = *(uint16_t*)(m_data.get() + priv::NAME_OFFSET + (i * 2));

    if (!*buf) {
      c = 0;
      continue;
    }

    utf8proc_int32_t wc;
    utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
    if (len < 0) {
      atError("invalid UTF-8 string");
      return;
    }
    buf += len;
    c = uint16_t(wc);
    utility::BigUint16(c);
  }
}

std::string SkywardSwordQuest::playerName() const {
  std::string val;

  for (uint32_t i = 0; i < 8; i++) {
    uint16_t c = *(uint16_t*)(m_data.get() + priv::NAME_OFFSET + (i * 2));

    if (c == 0)
      break;

    utility::BigUint16(c);
    utf8proc_uint8_t mb[4];
    utf8proc_ssize_t cs = utf8proc_encode_char(utf8proc_int32_t(c), mb);
    if (cs)
      val.append(reinterpret_cast<char*>(mb), cs);
  }

  return val;
}

void SkywardSwordQuest::setRupeeCount(uint16_t value) {
  uint16_t& tmp = *(uint16_t*)(m_data.get() + priv::RUPEE_COUNT_OFFSET);
  tmp = value;
  utility::BigUint16(tmp);
}

uint16_t SkywardSwordQuest::rupeeCount() {
  uint16_t ret = *(uint16_t*)(m_data.get() + priv::RUPEE_COUNT_OFFSET);
  return utility::BigUint16(ret);
}

void SkywardSwordQuest::setAmmoCount(SkywardSwordQuest::AmmoType type, uint32_t count) {
  AmmoValues& values = *(AmmoValues*)(m_data.get() + priv::AMMO_COUNT_OFFSET);
  utility::BigUint32(values.value);

  switch (type) {
  case Arrows:
    values.arrows = count;
    break;

  case Bombs:
    values.bombs = count;
    break;

  case Seeds:
    values.seeds = count;
    break;
  }

  utility::BigUint32(values.value);
}

uint32_t SkywardSwordQuest::ammoCount(AmmoType type) {
  AmmoValues values = *(AmmoValues*)(m_data.get() + priv::AMMO_COUNT_OFFSET);
  utility::BigUint32(values.value);

  switch (type) {
  case Arrows:
    return values.arrows;

  case Bombs:
    return values.bombs;

  case Seeds:
    return values.seeds;

  default:
    return 0;
  }
}

void SkywardSwordQuest::setMaxHP(uint16_t val) {
  *(uint16_t*)(m_data.get() + priv::MAX_HP_OFFSET) = utility::BigUint16(val);
}

uint16_t SkywardSwordQuest::maxHP() {
  uint16_t ret = *(uint16_t*)(m_data.get() + priv::MAX_HP_OFFSET);
  return utility::BigUint16(ret);
}

float SkywardSwordQuest::maxHearts() { return (maxHP() / 4.f); }

void SkywardSwordQuest::setSpawnHP(uint16_t val) {
  *(uint16_t*)(m_data.get() + priv::SPAWN_HP_OFFSET) = utility::BigUint16(val);
}

uint16_t SkywardSwordQuest::spawnHP() {
  uint16_t ret = *(uint16_t*)(m_data.get() + priv::SPAWN_HP_OFFSET);
  return utility::BigUint16(ret);
}

float SkywardSwordQuest::spawnHearts() { return (spawnHP() / 4.f); }

void SkywardSwordQuest::setCurrentHP(uint16_t val) {
  *(uint16_t*)(m_data.get() + priv::CURRENT_HP_OFFSET) = utility::BigUint16(val);
}

uint16_t SkywardSwordQuest::currentHP() {
  uint16_t ret = *(uint16_t*)(m_data.get() + priv::CURRENT_HP_OFFSET);
  return utility::BigUint16(ret);
}

float SkywardSwordQuest::currentHearts() { return (currentHP() / 4.f); }

std::string SkywardSwordQuest::currentLocation() {
  return std::string((char*)m_data.get() + priv::CURRENT_LOCATION_OFFSET);
}

std::string SkywardSwordQuest::currentArea() { return std::string((char*)m_data.get() + priv::CURRENT_AREA_OFFSET); }

std::string SkywardSwordQuest::currentLocationCopy() {
  return std::string((char*)m_data.get() + priv::CURRENT_LOCATION_COPY_OFFSET);
}

uint32_t SkywardSwordQuest::slotChecksum() {
  uint32_t ret = *(uint32_t*)(m_data.get() + priv::CHECKSUM_OFFSET);
  utility::BigUint32(ret);

  return ret;
}

uint32_t SkywardSwordQuest::skipChecksum() {
  uint32_t ret = *(uint32_t*)(m_skipData.get() + priv::SKIP_CHECKSUM_OFFSET);
  utility::BigUint32(ret);

  return ret;
}

void SkywardSwordQuest::fixChecksums() {
  uint32_t checksum = checksums::crc32(m_data.get(), priv::CHECKSUM_OFFSET);
  utility::BigUint32(checksum);
  *(uint32_t*)(m_data.get() + priv::CHECKSUM_OFFSET) = checksum;

  checksum = checksums::crc32(m_skipData.get(), priv::SKIP_CHECKSUM_OFFSET);
  utility::BigUint32(checksum);
  *(uint32_t*)(m_skipData.get() + priv::SKIP_CHECKSUM_OFFSET) = checksum;
}

void SkywardSwordQuest::setNew(bool isNew) { *(bool*)(m_data.get() + priv::ISNEW_OFFSET) = isNew; }

bool SkywardSwordQuest::isNew() const { return *(bool*)(m_data.get() + priv::ISNEW_OFFSET); }

} // namespace athena
