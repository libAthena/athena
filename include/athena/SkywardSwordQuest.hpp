#ifndef SKYWARDSWORDQUEST_HPP
#define SKYWARDSWORDQUEST_HPP

#include "athena/Global.hpp"
#include "athena/ZQuestFile.hpp"

namespace athena {

// TODO: Handle game specific data
class SkywardSwordQuest : public ZQuestFile {
public:
  enum AmmoType { Arrows, Bombs, Seeds };

  SkywardSwordQuest(std::unique_ptr<uint8_t[]>&& data, uint32_t len);

  void setPlayerName(const std::string& name);
  std::string playerName() const;

  void setRupeeCount(uint16_t value);
  uint16_t rupeeCount();
  void setAmmoCount(AmmoType type, uint32_t count);
  uint32_t ammoCount(AmmoType type);
  void setMaxHP(uint16_t val);
  uint16_t maxHP();
  float maxHearts();
  void setSpawnHP(uint16_t val);
  uint16_t spawnHP();
  float spawnHearts();
  void setCurrentHP(uint16_t val);
  uint16_t currentHP();
  float currentHearts();
  std::string currentLocation();
  std::string currentArea();
  std::string currentLocationCopy();

  void setSkipData(std::unique_ptr<uint8_t[]>&& data);
  uint8_t* skipData() const;

  uint32_t slotChecksum();
  uint32_t skipChecksum();
  void fixChecksums();

  void setNew(bool isNew);
  bool isNew() const;

private:
  std::unique_ptr<uint8_t[]> m_skipData;
};

} // namespace athena
#endif // SSQUEST_HPP
