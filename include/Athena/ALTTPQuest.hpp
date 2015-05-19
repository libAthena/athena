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

#ifndef ALTTP_QUEST_HPP
#define ALTTP_QUEST_HPP

#include "Types.hpp"
#include <string>
#include <vector>
#include "Athena/ALTTPStructs.hpp"
#include "Athena/ALTTPEnums.hpp"

namespace Athena
{

/*! \class ALTTPQuest
 *  \brief A Link to the Past Quest container class
 *
 * Contains all relevant data for an A Link to the Past
 * Quest entry.
 */
class ALTTPQuest
{
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
    void setRoomFlags(ALTTPRoomFlags* rf, atUint32 id);

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
    ALTTPRoomFlags* roomFlags(atUint32 id);

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
    void setOverworldEvents(ALTTPOverworldEvent* ow, atUint32 id);

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
    ALTTPOverworldEvent* overworldEvent(atUint32 id) const;

    /*!
     * \brief setInventory
     * \param inv
     */
    void setInventory(ALTTPInventory* inv);

    /*!
     * \brief inventory
     * \return
     */
    ALTTPInventory* inventory() const;

    /*!
     * \brief setRupeeMax
     * \param val
     */
    void setRupeeMax(atUint16 val);

    /*!
     * \brief rupeeMax
     * \return
     */
    atUint16  rupeeMax() const;

    /*!
     * \brief setRupeeCurrent
     * \param val
     */
    void setRupeeCurrent(atUint16 val);

    /*!
     * \brief rupeeCurrent
     * \return
     */
    atUint16  rupeeCurrent() const;

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
    void setWishingPond(atUint16 val);

    /*!
     * \brief wishingPond
     * \return
     */
    atUint16 wishingPond() const;

    /*!
     * \brief setHealthMax
     * \param val
     */
    void setHealthMax(atUint8 val);

    /*!
     * \brief healthMax
     * \return
     */
    atUint8   healthMax() const;

    /*!
     * \brief setHealth
     * \param val
     */
    void setHealth(atUint8 val);

    /*!
     * \brief health
     * \return
     */
    atUint8   health() const;

    /*!
     * \brief setMagicPower
     * \param val
     */
    void setMagicPower(atUint8 val);

    /*!
     * \brief magicPower
     * \return
     */
    atUint8  magicPower() const;

    /*!
     * \brief setKeys
     * \param val
     */
    void setKeys(atUint8 val);

    /*!
     * \brief keys
     * \return
     */
    atUint8   keys() const;

    /*!
     * \brief setBombUpgrades
     * \param val
     */
    void setBombUpgrades(atUint8 val);

    /*!
     * \brief bombUpgrades
     * \return
     */
    atUint8   bombUpgrades() const;

    /*!
     * \brief setArrowUpgrades
     * \param val
     */
    void setArrowUpgrades(atUint8 val);

    /*!
     * \brief arrowUpgrades
     * \return
     */
    atUint8   arrowUpgrades() const;

    /*!
     * \brief setHealthFiller
     * \param val
     */
    void setHealthFiller(atUint8 val);

    /*!
     * \brief healthFiller
     * \return
     */
    atUint8   healthFiller() const;

    /*!
     * \brief setMagicFiller
     * \param val
     */
    void setMagicFiller(atUint8 val);

    /*!
     * \brief magicFiller
     * \return
     */
    atUint8   magicFiller() const;

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
    void setBombFiller(atUint8 val);

    /*!
     * \brief bombFiller
     * \return
     */
    atUint8   bombFiller() const;

    /*!
     * \brief setArrowFiller
     * \param val
     */
    void setArrowFiller(atUint8 val);

    /*!
     * \brief arrowFiller
     * \return
     */
    atUint8   arrowFiller() const;

    /*!
     * \brief setArrows
     * \param val
     */
    void setArrows(atUint8 val);

    /*!
     * \brief arrows
     * \return
     */
    atUint8   arrows() const;

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
    \

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
    void setDungeonKeys(std::vector<atUint8> val);

    /*!
     * \brief setDungeonKeys
     * \param id
     * \param val
     */
    void setDungeonKeys(atUint32 id, atUint8 val);

    /*!
     * \brief dungeonKeys
     * \param id
     * \return
     */
    atUint8   dungeonKeys(atUint32 id) const;

    /*!
     * \brief dungeonCount
     * \return
     */
    atUint32  dungeonCount() const;

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
    void setOldManFlags(std::vector<atUint8> flags);

    /*!
     * \brief setOldManFlag
     * \param id
     * \param val
     */
    void setOldManFlag(atUint32 id, atUint8 val);

    /*!
     * \brief oldManFlag
     * \param id
     * \return
     */
    atUint8 oldManFlag(atUint32 id);

    /*!
     * \brief oldManFlagCount
     * \return
     */
    atUint32 oldManFlagCount() const;

    /*!
     * \brief setBombFlag
     * \param flag
     */
    void setBombFlag(atUint8 flag);

    /*!
     * \brief bombFlag
     * \return
     */
    atUint8 bombFlag() const;

    /*!
     * \brief setUnknown1
     * \param flags
     */
    void setUnknown1(std::vector<atUint8> flags);

    /*!
     * \brief setUnknown1
     * \param id
     * \param val
     */
    void setUnknown1(atUint32 id, atUint8 val);

    /*!
     * \brief unknown1
     * \param id
     * \return
     */
    atUint8 unknown1(atUint32 id);

    /*!
     * \brief unknown1Count
     * \return
     */
    atUint32 unknown1Count() const;

    /*!
     * \brief setPlayerName
     * \param playerName
     */
    void setPlayerName(std::vector<atUint16> playerName);
    /*!
     * \brief setPlayerName
     * \param playerName
     */
    void setPlayerName(const std::string& playerName);
    /*!
     * \brief playerName
     * \return
     */
    std::vector<atUint16> playerName() const;
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
    void setDungeonDeathTotals(std::vector<atUint16> val);

    /*!
     * \brief setDungeonDeathTotal
     * \param id
     * \param val
     */
    void setDungeonDeathTotal(atUint32 id, atUint16 val);

    /*!
     * \brief dungeonDeathTotal
     * \param id
     * \return
     */
    atUint16 dungeonDeathTotal(atUint32 id) const;

    /*!
     * \brief dungeonDeathTotalCount
     * \return
     */
    atUint16 dungeonDeathTotalCount() const;

    /*!
     * \brief setUnknown2
     * \param val
     */
    void setUnknown2(atUint16 val);

    /*!
     * \brief unknown2
     * \return
     */
    atUint16 unknown2() const;

    /*!
     * \brief setDeathSaveCount
     * \param val
     */
    void setDeathSaveCount(atUint16 val);

    /*!
     * \brief deathSaveCount
     * \return
     */
    atUint16 deathSaveCount() const;

    /*!
     * \brief setPostGameDeathCounter
     * \param val
     */
    void setPostGameDeathCounter(atInt16 val);

    /*!
     * \brief postGameDeathCounter
     * \return
     */
    atInt16 postGameDeathCounter() const;

    /*!
     * \brief setChecksum
     * \param checksum
     */
    void setChecksum(atUint16 checksum);

    /*!
     * \brief checksum
     * \return
     */
    atUint16 checksum() const;
private:
    std::vector<ALTTPRoomFlags*>      m_roomFlags;
    std::vector<ALTTPOverworldEvent*> m_overworldEvents;
    ALTTPInventory*                   m_inventory;
    atUint16                          m_rupeeMax;
    atUint16                          m_rupeeCurrent;
    ALTTPDungeonItemFlags             m_compasses;
    ALTTPDungeonItemFlags             m_bigKeys;
    ALTTPDungeonItemFlags             m_dungeonMaps;
    atUint16                          m_wishingPond;
    atUint8                           m_healthMax;
    atUint8                           m_health;
    atUint8                           m_magicPower;
    atUint8                           m_keys;
    atUint8                           m_bombUpgrades;
    atUint8                           m_arrowUpgrades;
    atUint8                           m_heartFiller;
    atUint8                           m_magicFiller;
    ALTTPPendants                     m_pendants;
    atUint8                           m_bombFiller;
    atUint8                           m_arrowFiller;
    atUint8                           m_arrows;
    ALTTPAbilities                    m_abilityFlags;
    ALTTPCrystals                     m_crystals;
    ALTTPMagicUsage                   m_magicUsage;
    std::vector<atUint8>              m_dungeonKeys;
    ALTTPProgressIndicator            m_progressIndicator;
    ALTTPProgressFlags1               m_progressFlags1;
    ALTTPMapIcon                      m_mapIcon;
    ALTTPStartLocation                m_startLocation;
    ALTTPProgressFlags2               m_progressFlags2;
    ALTTPLightDarkWorldIndicator      m_lightDarkWorldIndicator;
    ALTTPTagAlong                     m_tagAlong;
    std::vector<atUint8>              m_oldManFlags;
    atUint8                           m_bombFlag;
    std::vector<atUint8>              m_unknown1;
    std::vector<atUint16>             m_playerName;
    bool                              m_valid;
    std::vector<atUint16>             m_dungeonDeathTotals;
    atUint16                          m_unknown2;
    atUint16                          m_deathSaveCount;
    atInt16                           m_postGameDeathCounter;
    atUint16                          m_checksum;
};

} // zelda

#endif // ALTTP_QUEST_HPP
#endif // ATHENA_NO_SAVES
