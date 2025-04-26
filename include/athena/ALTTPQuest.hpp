#pragma once

#include "athena/Global.hpp"
#include <string>
#include <vector>
#include "athena/ALTTPStructs.hpp"
#include "athena/ALTTPEnums.hpp"

namespace athena {

/*! \class ALTTPQuest
 *  \brief A Link to the Past Quest container class
 *
 * Contains all relevant data for an A Link to the Past
 * Quest entry.
 */
class ALTTPQuest {
public:
  /*!
   * \brief ALTTPQuest
   */
  ALTTPQuest();
  ~ALTTPQuest();

  /*!
   * \brief setRoomFlags
   * \param flags
   */
  void setRoomFlags(std::vector<ALTTPRoomFlags*> flags);

  /*!
   * \brief setRoomFlags
   * \param rf
   * \param id
   */
  void setRoomFlags(ALTTPRoomFlags* rf, uint32_t id);

  /*!
   * \brief roomFlags
   * \return
   */
  std::vector<ALTTPRoomFlags*> roomFlags();

  /*!
   * \brief roomFlags
   * \param id
   * \return
   */
  ALTTPRoomFlags* roomFlags(uint32_t id);

  /*!
   * \brief setOverworldEvents
   * \param events
   */
  void setOverworldEvents(std::vector<ALTTPOverworldEvent*> events);

  /*!
   * \brief setOverworldEvents
   * \param ow
   * \param id
   */
  void setOverworldEvents(ALTTPOverworldEvent* ow, uint32_t id);

  /*!
   * \brief overworldEvents
   * \return
   */
  std::vector<ALTTPOverworldEvent*> overworldEvents() const;

  /*!
   * \brief overworldEvent
   * \param id
   * \return
   */
  ALTTPOverworldEvent* overworldEvent(uint32_t id) const;

  /*!
   * \brief setInventory
   * \param inv
   */
  void setInventory(const ALTTPInventory& inv);

  /*!
   * \brief inventory
   * \return
   */
  const ALTTPInventory& inventory() const;

  /*!
   * \brief setRupeeMax
   * \param val
   */
  void setRupeeMax(uint16_t val);

  /*!
   * \brief rupeeMax
   * \return
   */
  uint16_t rupeeMax() const;

  /*!
   * \brief setRupeeCurrent
   * \param val
   */
  void setRupeeCurrent(uint16_t val);

  /*!
   * \brief rupeeCurrent
   * \return
   */
  uint16_t rupeeCurrent() const;

  /*!
   * \brief setCompasses
   * \param flags
   */
  void setCompasses(ALTTPDungeonItemFlags flags);

  /*!
   * \brief compasses
   * \return
   */
  ALTTPDungeonItemFlags compasses() const;

  /*!
   * \brief setBigKeys
   * \param flags
   */
  void setBigKeys(ALTTPDungeonItemFlags flags);

  /*!
   * \brief bigKeys
   * \return
   */
  ALTTPDungeonItemFlags bigKeys() const;

  /*!
   * \brief setDungeonMaps
   * \param flags
   */
  void setDungeonMaps(ALTTPDungeonItemFlags flags);

  /*!
   * \brief dungeonMaps
   * \return
   */
  ALTTPDungeonItemFlags dungeonMaps() const;

  /*!
   * \brief setWishingPond
   * \param val
   */
  void setWishingPond(uint16_t val);

  /*!
   * \brief wishingPond
   * \return
   */
  uint16_t wishingPond() const;

  /*!
   * \brief setHealthMax
   * \param val
   */
  void setHealthMax(uint8_t val);

  /*!
   * \brief healthMax
   * \return
   */
  uint8_t healthMax() const;

  /*!
   * \brief setHealth
   * \param val
   */
  void setHealth(uint8_t val);

  /*!
   * \brief health
   * \return
   */
  uint8_t health() const;

  /*!
   * \brief setMagicPower
   * \param val
   */
  void setMagicPower(uint8_t val);

  /*!
   * \brief magicPower
   * \return
   */
  uint8_t magicPower() const;

  /*!
   * \brief setKeys
   * \param val
   */
  void setKeys(uint8_t val);

  /*!
   * \brief keys
   * \return
   */
  uint8_t keys() const;

  /*!
   * \brief setBombUpgrades
   * \param val
   */
  void setBombUpgrades(uint8_t val);

  /*!
   * \brief bombUpgrades
   * \return
   */
  uint8_t bombUpgrades() const;

  /*!
   * \brief setArrowUpgrades
   * \param val
   */
  void setArrowUpgrades(uint8_t val);

  /*!
   * \brief arrowUpgrades
   * \return
   */
  uint8_t arrowUpgrades() const;

  /*!
   * \brief setHealthFiller
   * \param val
   */
  void setHealthFiller(uint8_t val);

  /*!
   * \brief healthFiller
   * \return
   */
  uint8_t healthFiller() const;

  /*!
   * \brief setMagicFiller
   * \param val
   */
  void setMagicFiller(uint8_t val);

  /*!
   * \brief magicFiller
   * \return
   */
  uint8_t magicFiller() const;

  /*!
   * \brief setPendants
   * \param val
   */
  void setPendants(ALTTPPendants val);

  /*!
   * \brief pendants
   * \return
   */
  ALTTPPendants pendants() const;

  /*!
   * \brief setBombFiller
   * \param val
   */
  void setBombFiller(uint8_t val);

  /*!
   * \brief bombFiller
   * \return
   */
  uint8_t bombFiller() const;

  /*!
   * \brief setArrowFiller
   * \param val
   */
  void setArrowFiller(uint8_t val);

  /*!
   * \brief arrowFiller
   * \return
   */
  uint8_t arrowFiller() const;

  /*!
   * \brief setArrows
   * \param val
   */
  void setArrows(uint8_t val);

  /*!
   * \brief arrows
   * \return
   */
  uint8_t arrows() const;

  /*!
   * \brief setAbilityFlags
   * \param val
   */
  void setAbilityFlags(ALTTPAbilities val);

  /*!
   * \brief abilityFlags
   * \return
   */
  ALTTPAbilities abilityFlags() const;

  /*!
   * \brief setCrystals
   * \param val
   */
  void setCrystals(ALTTPCrystals val);

  /*!
   * \brief crystals
   * \return
   */
  ALTTPCrystals crystals() const;

  /*!
   * \brief setMagicUsage
   * \param val
   */
  void setMagicUsage(ALTTPMagicUsage val);

  /*!
   * \brief magicUsage
   * \return
   */
  ALTTPMagicUsage magicUsage() const;

  /*!
   * \brief setDungeonKeys
   * \param val
   */
  void setDungeonKeys(std::vector<uint8_t> val);

  /*!
   * \brief setDungeonKeys
   * \param id
   * \param val
   */
  void setDungeonKeys(uint32_t id, uint8_t val);

  /*!
   * \brief dungeonKeys
   * \param id
   * \return
   */
  uint8_t dungeonKeys(uint32_t id) const;

  /*!
   * \brief dungeonCount
   * \return
   */
  uint32_t dungeonCount() const;

  /*!
   * \brief setProgressIndicator
   * \param val
   */
  void setProgressIndicator(ALTTPProgressIndicator val);

  /*!
   * \brief progressIndicator
   * \return
   */
  ALTTPProgressIndicator progressIndicator() const;

  /*!
   * \brief setProgressFlags1
   * \param val
   */
  void setProgressFlags1(ALTTPProgressFlags1 val);

  /*!
   * \brief progressFlags1
   * \return
   */
  ALTTPProgressFlags1 progressFlags1() const;

  /*!
   * \brief setMapIcon
   * \param val
   */
  void setMapIcon(ALTTPMapIcon val);

  /*!
   * \brief mapIcon
   * \return
   */
  ALTTPMapIcon mapIcon() const;

  /*!
   * \brief setStartLocation
   * \param val
   */
  void setStartLocation(ALTTPStartLocation val);

  /*!
   * \brief startLocation
   * \return
   */
  ALTTPStartLocation startLocation() const;

  /*!
   * \brief setProgressFlags2
   * \param val
   */
  void setProgressFlags2(ALTTPProgressFlags2 val);

  /*!
   * \brief progressFlags2
   * \return
   */
  ALTTPProgressFlags2 progressFlags2() const;

  /*!
   * \brief setLightDarkWorldIndicator
   * \param val
   */
  void setLightDarkWorldIndicator(ALTTPLightDarkWorldIndicator val);

  /*!
   * \brief lightDarkWorldIndicator
   * \return
   */
  ALTTPLightDarkWorldIndicator lightDarkWorldIndicator() const;

  /*!
   * \brief setTagAlong
   * \param val
   */
  void setTagAlong(ALTTPTagAlong val);

  /*!
   * \brief tagAlong
   * \return
   */
  ALTTPTagAlong tagAlong() const;

  /*!
   * \brief setOldManFlags
   * \param flags
   */
  void setOldManFlags(std::vector<uint8_t> flags);

  /*!
   * \brief setOldManFlag
   * \param id
   * \param val
   */
  void setOldManFlag(uint32_t id, uint8_t val);

  /*!
   * \brief oldManFlag
   * \param id
   * \return
   */
  uint8_t oldManFlag(uint32_t id);

  /*!
   * \brief oldManFlagCount
   * \return
   */
  uint32_t oldManFlagCount() const;

  /*!
   * \brief setBombFlag
   * \param flag
   */
  void setBombFlag(uint8_t flag);

  /*!
   * \brief bombFlag
   * \return
   */
  uint8_t bombFlag() const;

  /*!
   * \brief setUnknown1
   * \param flags
   */
  void setUnknown1(std::vector<uint8_t> flags);

  /*!
   * \brief setUnknown1
   * \param id
   * \param val
   */
  void setUnknown1(uint32_t id, uint8_t val);

  /*!
   * \brief unknown1
   * \param id
   * \return
   */
  uint8_t unknown1(uint32_t id);

  /*!
   * \brief unknown1Count
   * \return
   */
  uint32_t unknown1Count() const;

  /*!
   * \brief setPlayerName
   * \param playerName
   */
  void setPlayerName(std::vector<uint16_t> playerName);
  /*!
   * \brief setPlayerName
   * \param playerName
   */
  void setPlayerName(const std::string& playerName);
  /*!
   * \brief playerName
   * \return
   */
  std::vector<uint16_t> playerName() const;
  /*!
   * \brief playerNameToString
   * \return
   */
  std::string playerNameToString() const;

  /*!
   * \brief setValid
   * \param val
   */
  void setValid(bool val);

  /*!
   * \brief valid
   * \return
   */
  bool valid();

  /*!
   * \brief setDungeonDeathTotals
   * \param val
   */
  void setDungeonDeathTotals(std::vector<uint16_t> val);

  /*!
   * \brief setDungeonDeathTotal
   * \param id
   * \param val
   */
  void setDungeonDeathTotal(uint32_t id, uint16_t val);

  /*!
   * \brief dungeonDeathTotal
   * \param id
   * \return
   */
  uint16_t dungeonDeathTotal(uint32_t id) const;

  /*!
   * \brief dungeonDeathTotalCount
   * \return
   */
  uint16_t dungeonDeathTotalCount() const;

  /*!
   * \brief setUnknown2
   * \param val
   */
  void setUnknown2(uint16_t val);

  /*!
   * \brief unknown2
   * \return
   */
  uint16_t unknown2() const;

  /*!
   * \brief setDeathSaveCount
   * \param val
   */
  void setDeathSaveCount(uint16_t val);

  /*!
   * \brief deathSaveCount
   * \return
   */
  uint16_t deathSaveCount() const;

  /*!
   * \brief setPostGameDeathCounter
   * \param val
   */
  void setPostGameDeathCounter(int16_t val);

  /*!
   * \brief postGameDeathCounter
   * \return
   */
  int16_t postGameDeathCounter() const;

  /*!
   * \brief setChecksum
   * \param checksum
   */
  void setChecksum(uint16_t checksum);

  /*!
   * \brief checksum
   * \return
   */
  uint16_t checksum() const;

private:
  std::vector<ALTTPRoomFlags*> m_roomFlags;
  std::vector<ALTTPOverworldEvent*> m_overworldEvents;
  ALTTPInventory m_inventory;
  uint16_t m_rupeeMax;
  uint16_t m_rupeeCurrent;
  ALTTPDungeonItemFlags m_compasses;
  ALTTPDungeonItemFlags m_bigKeys;
  ALTTPDungeonItemFlags m_dungeonMaps;
  uint16_t m_wishingPond;
  uint8_t m_healthMax;
  uint8_t m_health;
  uint8_t m_magicPower;
  uint8_t m_keys;
  uint8_t m_bombUpgrades;
  uint8_t m_arrowUpgrades;
  uint8_t m_heartFiller;
  uint8_t m_magicFiller;
  ALTTPPendants m_pendants;
  uint8_t m_bombFiller;
  uint8_t m_arrowFiller;
  uint8_t m_arrows;
  ALTTPAbilities m_abilityFlags;
  ALTTPCrystals m_crystals;
  ALTTPMagicUsage m_magicUsage;
  std::vector<uint8_t> m_dungeonKeys;
  ALTTPProgressIndicator m_progressIndicator;
  ALTTPProgressFlags1 m_progressFlags1;
  ALTTPMapIcon m_mapIcon;
  ALTTPStartLocation m_startLocation;
  ALTTPProgressFlags2 m_progressFlags2;
  ALTTPLightDarkWorldIndicator m_lightDarkWorldIndicator;
  ALTTPTagAlong m_tagAlong;
  std::vector<uint8_t> m_oldManFlags;
  uint8_t m_bombFlag;
  std::vector<uint8_t> m_unknown1;
  std::vector<uint16_t> m_playerName;
  bool m_valid;
  std::vector<uint16_t> m_dungeonDeathTotals;
  uint16_t m_unknown2;
  uint16_t m_deathSaveCount;
  int16_t m_postGameDeathCounter;
  uint16_t m_checksum;
};

} // namespace athena
