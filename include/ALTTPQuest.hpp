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

#ifndef __ALTTP_QUEST_HPP__
#define __ALTTP_QUEST_HPP__

#include "Types.hpp"
#include <string>
#include <vector>
#include "ALTTPStructs.hpp"
#include "ALTTPEnums.hpp"

/*! \class ALTTPQuest
 *  \brief A Link to the Past Quest container class
 *
 * Contains all relevant data for an A Link to the Past
 * Quest entry.
 */
class ALTTPQuest
{
public:
    ALTTPQuest();
    ~ALTTPQuest();

    void setRoomFlags(std::vector<ALTTPRoomFlags*> flags);
    void setRoomFlags(ALTTPRoomFlags* rf, Uint32 id);
    std::vector<ALTTPRoomFlags*> roomFlags();
    ALTTPRoomFlags* roomFlags(Uint32 id);

    void setOverworldEvents(std::vector<ALTTPOverworldEvent*> events);
    void setOverworldEvents(ALTTPOverworldEvent* ow, Uint32 id);
    std::vector<ALTTPOverworldEvent*> overworldEvents() const;
    ALTTPOverworldEvent* overworldEvent(Uint32 id) const;

    void setInventory(ALTTPInventory* inv);
    ALTTPInventory* inventory() const;

    void setRupeeMax(Uint16 val);
    Uint16  rupeeMax() const;

    void setRupeeCurrent(Uint16 val);
    Uint16  rupeeCurrent() const;

    void setCompasses(ALTTPDungeonItemFlags flags);
    ALTTPDungeonItemFlags compasses() const;

    void setBigKeys(ALTTPDungeonItemFlags flags);
    ALTTPDungeonItemFlags bigKeys() const;

    void setDungeonMaps(ALTTPDungeonItemFlags flags);
    ALTTPDungeonItemFlags dungeonMaps() const;

    void setWishingPond(Uint16 val);
    Uint16 wishingPond() const;

    void setHealthMax(Uint8 val);
    Uint8   healthMax() const;

    void setHealth(Uint8 val);
    Uint8   health() const;

    void setMagicPower(Uint8 val);
    Uint8  magicPower() const;

    void setKeys(Uint8 val);
    Uint8   keys() const;

    void setBombUpgrades(Uint8 val);
    Uint8   bombUpgrades() const;

    void setArrowUpgrades(Uint8 val);
    Uint8   arrowUpgrades() const;

    void setHealthFiller(Uint8 val);
    Uint8   healthFiller() const;

    void setMagicFiller(Uint8 val);
    Uint8   magicFiller() const;

    void setPendants(ALTTPPendants val);
    ALTTPPendants pendants() const;

    void setBombFiller(Uint8 val);
    Uint8   bombFiller() const;

    void setArrowFiller(Uint8 val);
    Uint8   arrowFiller() const;
    void setArrows(Uint8 val);
    Uint8   arrows() const;

    void setAbilityFlags(ALTTPAbilities val);
    ALTTPAbilities abilityFlags() const;

    void setCrystals(ALTTPCrystals val);
    ALTTPCrystals crystals() const;

    void setMagicUsage(ALTTPMagicUsage val);
    ALTTPMagicUsage magicUsage() const;

    void setDungeonKeys(std::vector<Uint8> val);
    void setDungeonKeys(Uint32 id, Uint8 val);
    Uint8   dungeonKeys(Uint32 id) const;
    Uint32  dungeonCount() const;

    void setProgressIndicator(ALTTPProgressIndicator val);
    ALTTPProgressIndicator progressIndicator() const;

    void setProgressFlags1(ALTTPProgressFlags1 val);
    ALTTPProgressFlags1 progressFlags1() const;

    void setMapIcon(ALTTPMapIcon val);
    ALTTPMapIcon mapIcon() const;

    void setStartLocation(ALTTPStartLocation val);
    ALTTPStartLocation startLocation() const;

    void setProgressFlags2(ALTTPProgressFlags2 val);
    ALTTPProgressFlags2 progressFlags2() const;

    void setLightDarkWorldIndicator(ALTTPLightDarkWorldIndicator val);
    ALTTPLightDarkWorldIndicator lightDarkWorldIndicator() const;

    void setTagAlong(ALTTPTagAlong val);
    ALTTPTagAlong tagAlong() const;

    void setOldManFlags(std::vector<Uint8> flags);
    void setOldManFlag(Uint32 id, Uint8 val);
    Uint8 oldManFlag(Uint32 id);
    Uint32 oldManFlagCount() const;

    void setBombFlag(Uint8 flag);
    Uint8 bombFlag() const;

    void setUnknown1(std::vector<Uint8> flags);
    void setUnknown1(Uint32 id, Uint8 val);
    Uint8 unknown1(Uint32 id);
    Uint32 unknown1Count() const;

    void setPlayerName(std::vector<Uint16> playerName);
    void setPlayerName(const std::string& playerName);
    std::vector<Uint16> playerName() const;
    std::string playerNameToString() const;

    void setValid(bool val);
    bool valid();

    void setDungeonDeathTotals(std::vector<Uint16> val);
    void setDungeonDeathTotal(Uint32 id, Uint16 val);
    Uint16 dungeonDeathTotal(Uint32 id) const;
    Uint16 dungeonDeathTotalCount() const;

    void setUnknown2(Uint16 val);
    Uint16 unknown2() const;

    void setDeathSaveCount(Uint16 val);
    Uint16 deathSaveCount() const;

    void setPostGameDeathCounter(Int16 val);
    Int16 postGameDeathCounter() const;

    void setChecksum(Uint16 checksum);
    Uint16 checksum() const;
private:
    std::vector<ALTTPRoomFlags*>      m_roomFlags;
    std::vector<ALTTPOverworldEvent*> m_overworldEvents;
    ALTTPInventory*                   m_inventory;
    Uint16                            m_rupeeMax;
    Uint16                            m_rupeeCurrent;
    ALTTPDungeonItemFlags             m_compasses;
    ALTTPDungeonItemFlags             m_bigKeys;
    ALTTPDungeonItemFlags             m_dungeonMaps;
    Uint16                            m_wishingPond;
    Uint8                             m_healthMax;
    Uint8                             m_health;
    Uint8                             m_magicPower;
    Uint8                             m_keys;
    Uint8                             m_bombUpgrades;
    Uint8                             m_arrowUpgrades;
    Uint8                             m_heartFiller;
    Uint8                             m_magicFiller;
    ALTTPPendants                     m_pendants;
    Uint8                             m_bombFiller;
    Uint8                             m_arrowFiller;
    Uint8                             m_arrows;
    ALTTPAbilities                    m_abilityFlags;
    ALTTPCrystals                     m_crystals;
    ALTTPMagicUsage                   m_magicUsage;
    std::vector<Uint8>                m_dungeonKeys;
    ALTTPProgressIndicator            m_progressIndicator;
    ALTTPProgressFlags1               m_progressFlags1;
    ALTTPMapIcon                      m_mapIcon;
    ALTTPStartLocation                m_startLocation;
    ALTTPProgressFlags2               m_progressFlags2;
    ALTTPLightDarkWorldIndicator      m_lightDarkWorldIndicator;
    ALTTPTagAlong                     m_tagAlong;
    std::vector<Uint8>                m_oldManFlags;
    Uint8                             m_bombFlag;
    std::vector<Uint8>                m_unknown1;
    std::vector<Uint16>               m_playerName;
    Uint16                            m_valid;
    std::vector<Uint16>               m_dungeonDeathTotals;
    Uint16                            m_unknown2;
    Uint16                            m_deathSaveCount;
    Int16                             m_postGameDeathCounter;
    Uint16                            m_checksum;
};


#endif // __ALTTP_QUEST_HPP__
