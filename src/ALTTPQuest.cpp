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

#include "ALTTPQuest.hpp"
#include "InvalidOperationException.hpp"
#include <iostream>

namespace zelda
{

ALTTPQuest::ALTTPQuest()
{
}

ALTTPQuest::~ALTTPQuest()
{
    m_roomFlags.clear();
    m_overworldEvents.clear();
    m_oldManFlags.clear();
    m_playerName.clear();
    m_unknown1.clear();
}

void ALTTPQuest::setRoomFlags(std::vector<ALTTPRoomFlags*> rf)
{
    m_roomFlags = rf;
}

void ALTTPQuest::setRoomFlags(ALTTPRoomFlags* rf, Uint32 id)
{
    m_roomFlags[id] = rf;
}

std::vector<ALTTPRoomFlags*> ALTTPQuest::roomFlags()
{
    return m_roomFlags;
}

ALTTPRoomFlags* ALTTPQuest::roomFlags(Uint32 id)
{
    return m_roomFlags[id];
}

void ALTTPQuest::setOverworldEvents(std::vector<ALTTPOverworldEvent*> ow)
{
    m_overworldEvents = ow;
}

void ALTTPQuest::setOverworldEvents(ALTTPOverworldEvent* ow, Uint32 id)
{
    m_overworldEvents[id] = ow;
}

std::vector<ALTTPOverworldEvent*> ALTTPQuest::overworldEvents() const
{
    return m_overworldEvents;
}

ALTTPOverworldEvent* ALTTPQuest::overworldEvent(Uint32 id) const
{
    if (id > m_overworldEvents.size() - 1)
        throw error::InvalidOperationException("ALTTPQuest::overworldEvents -> index out of range");
    return m_overworldEvents[id];
}

void ALTTPQuest::setInventory(ALTTPInventory* inv)
{
    m_inventory = inv;
}

ALTTPInventory* ALTTPQuest::inventory() const
{
    return m_inventory;
}

void ALTTPQuest::setRupeeMax(Uint16 val)
{
    m_rupeeMax = val;
}

Uint16 ALTTPQuest::rupeeMax() const
{
    return m_rupeeMax;
}

void ALTTPQuest::setRupeeCurrent(Uint16 val)
{
    m_rupeeCurrent = val;
}

Uint16 ALTTPQuest::rupeeCurrent() const
{
    return m_rupeeCurrent;
}

void ALTTPQuest::setCompasses(ALTTPDungeonItemFlags flags)
{
    m_compasses = flags;
}

ALTTPDungeonItemFlags ALTTPQuest::compasses() const
{
    return m_compasses;
}

void ALTTPQuest::setBigKeys(ALTTPDungeonItemFlags flags)
{
    m_bigKeys = flags;
}

ALTTPDungeonItemFlags ALTTPQuest::bigKeys() const
{
    return m_bigKeys;
}

void ALTTPQuest::setDungeonMaps(ALTTPDungeonItemFlags flags)
{
    m_dungeonMaps = flags;
}

ALTTPDungeonItemFlags ALTTPQuest::dungeonMaps() const
{
    return m_dungeonMaps;
}

void ALTTPQuest::setWishingPond(Uint16 val)
{
    m_wishingPond = val;
}

Uint16 ALTTPQuest::wishingPond() const
{
    return m_wishingPond;
}

void ALTTPQuest::setHealthMax(Uint8 val)
{
    m_healthMax = val;
}

Uint8 ALTTPQuest::healthMax() const
{
    return m_healthMax;
}

void ALTTPQuest::setHealth(Uint8 val)
{
    m_health = val;
}

Uint8 ALTTPQuest::health() const
{
    return m_health;
}

void ALTTPQuest::setMagicPower(Uint8 val)
{
    m_magicPower = val;
}

Uint8 ALTTPQuest::magicPower() const
{
    return m_magicPower;
}

void ALTTPQuest::setKeys(Uint8 val)
{
    m_keys = val;
}

Uint8 ALTTPQuest::keys() const
{
    return m_keys;
}

void ALTTPQuest::setBombUpgrades(Uint8 val)
{
    m_bombUpgrades = val;
}

Uint8 ALTTPQuest::bombUpgrades() const
{
    return m_bombUpgrades;
}

void ALTTPQuest::setArrowUpgrades(Uint8 val)
{
    m_arrowUpgrades = val;
}

Uint8 ALTTPQuest::arrowUpgrades() const
{
    return m_arrowUpgrades;
}

void ALTTPQuest::setHealthFiller(Uint8 val)
{
    m_heartFiller = val;
}

Uint8 ALTTPQuest::healthFiller() const
{
    return m_heartFiller;
}
void ALTTPQuest::setMagicFiller(Uint8 val)
{
    m_heartFiller = val;
}

Uint8 ALTTPQuest::magicFiller() const
{
    return m_heartFiller;
}

void ALTTPQuest::setPendants(ALTTPPendants val)
{
    m_pendants = val;
}

ALTTPPendants ALTTPQuest::pendants() const
{
    return m_pendants;
}

void ALTTPQuest::setBombFiller(Uint8 val)
{
    m_bombFiller = val;
}

Uint8 ALTTPQuest::bombFiller() const
{
    return m_bombFiller;
}

void ALTTPQuest::setArrowFiller(Uint8 val)
{
    m_arrowFiller = val;
}

Uint8 ALTTPQuest::arrowFiller() const
{
    return m_arrowFiller;
}
void ALTTPQuest::setArrows(Uint8 val)
{
    m_arrows = val;
}

Uint8 ALTTPQuest::arrows() const
{
    return m_arrows;
}

void ALTTPQuest::setAbilityFlags(ALTTPAbilities val)
{
    m_abilityFlags = val;
}

ALTTPAbilities ALTTPQuest::abilityFlags() const
{
    return m_abilityFlags;
}

void ALTTPQuest::setCrystals(ALTTPCrystals val)
{
    m_crystals = val;
}

ALTTPCrystals ALTTPQuest::crystals() const
{
    return m_crystals;
}

void ALTTPQuest::setMagicUsage(ALTTPMagicUsage val)
{
    m_magicUsage = val;
}

ALTTPMagicUsage ALTTPQuest::magicUsage() const
{
    return m_magicUsage;
}

void ALTTPQuest::setDungeonKeys(std::vector<Uint8> val)
{
    m_dungeonKeys = val;
}

void ALTTPQuest::setDungeonKeys(Uint32 id, Uint8 val)
{
    if (id > m_dungeonKeys.size() - 1)
        throw error::InvalidOperationException("ALTTPQuest::setDungeonKeys -> index out of range");

    m_dungeonKeys[id] = val;
}

Uint8 ALTTPQuest::dungeonKeys(Uint32 id) const
{
    if (id > m_dungeonKeys.size() - 1)
        throw error::InvalidOperationException("ALTTPQuest::dungeonKeys -> index out of range");

    return m_dungeonKeys[id];
}

Uint32 ALTTPQuest::dungeonCount() const
{
    return m_dungeonKeys.size();
}


void ALTTPQuest::setProgressIndicator(ALTTPProgressIndicator val)
{
    m_progressIndicator = val;
}

ALTTPProgressIndicator ALTTPQuest::progressIndicator() const
{
    return m_progressIndicator;
}

void ALTTPQuest::setProgressFlags1(ALTTPProgressFlags1 val)
{
    m_progressFlags1 = val;
}

ALTTPProgressFlags1 ALTTPQuest::progressFlags1() const
{
    return m_progressFlags1;
}

void ALTTPQuest::setMapIcon(ALTTPMapIcon val)
{
    m_mapIcon = val;
}

ALTTPMapIcon ALTTPQuest::mapIcon() const
{
    return m_mapIcon;
}

void ALTTPQuest::setStartLocation(ALTTPStartLocation val)
{
    m_startLocation = val;
}

ALTTPStartLocation ALTTPQuest::startLocation() const
{
    return m_startLocation;
}

void ALTTPQuest::setProgressFlags2(ALTTPProgressFlags2 val)
{
    m_progressFlags2 = val;
}

ALTTPProgressFlags2 ALTTPQuest::progressFlags2() const
{
    return m_progressFlags2;
}

void ALTTPQuest::setLightDarkWorldIndicator(ALTTPLightDarkWorldIndicator val)
{
    m_lightDarkWorldIndicator = val;
}

ALTTPLightDarkWorldIndicator ALTTPQuest::lightDarkWorldIndicator() const
{
    return m_lightDarkWorldIndicator;
}

void ALTTPQuest::setTagAlong(ALTTPTagAlong val)
{
    m_tagAlong = val;
}

ALTTPTagAlong ALTTPQuest::tagAlong() const
{
    return m_tagAlong;
}

void ALTTPQuest::setOldManFlags(std::vector<Uint8> flags)
{
    m_oldManFlags = flags;
}

void ALTTPQuest::setOldManFlag(Uint32 id, Uint8 val)
{
    if (id > m_oldManFlags.size() - 1)
        throw error::InvalidOperationException("ALTTPQuest::setOldManFlag -> index out of range");

    m_oldManFlags[id] = val;
}

Uint8 ALTTPQuest::oldManFlag(Uint32 id)
{
    if (id > m_oldManFlags.size() - 1)
        throw error::InvalidOperationException("ALTTPQuest::oldManFlag -> index out of range");

    return m_oldManFlags[id];
}

Uint32 ALTTPQuest::oldManFlagCount() const
{
    return m_oldManFlags.size();
}

void ALTTPQuest::setBombFlag(Uint8 flag)
{
    m_bombFlag = flag;
}

Uint8 ALTTPQuest::bombFlag() const
{
    return m_bombFlag;
}

void ALTTPQuest::setUnknown1(std::vector<Uint8> flags)
{
    m_unknown1 = flags;
}

void ALTTPQuest::setUnknown1(Uint32 id, Uint8 val)
{
    if (id > m_unknown1.size())
        throw error::InvalidOperationException("ALTTPQuest::setUnknown1) -> index out of range");

    m_unknown1[id] = val;
}

Uint8 ALTTPQuest::unknown1(Uint32 id)
{
    if (id > m_unknown1.size())
        throw error::InvalidOperationException("ALTTPQuest::unknown1 -> index out of range");

    return m_unknown1[id];
}

Uint32 ALTTPQuest::unknown1Count() const
{
    return m_unknown1.size();
}

void ALTTPQuest::setPlayerName(std::vector<Uint16> playerName)
{
    m_playerName = playerName;
}

void ALTTPQuest::setPlayerName(const std::string& playerName)
{
    if (playerName == std::string() || playerName.size() > 6)
        throw error::InvalidOperationException("ALTTPQuest::setPlayerName -> playerName invalid");

    m_playerName.clear();

    for (Uint32 i = 0; i < 6; i++)
    {
        if (i > playerName.size() - 1)
        {
            m_playerName.push_back((Uint16)0xA9);
            continue;
        }
        char c = playerName[i];
        if (c >= 'A' && c <= 'P' && c != 'I')
        {

            m_playerName.push_back((Uint16)(c - 'A'));
            continue;
        }
        if (c >= 'Q' && c <= 'Z')
        {
            std::cout << std::hex << (Uint16)((c - 'Q') + 0x20) << std::endl;
            m_playerName.push_back((Uint16)((c - 'Q') + 0x20));
            continue;
        }
        if (c >= 'a' && c <= 'f')
        {
            std::cout << std::hex << (Uint16)((c - 'a') + 0x2A) << std::endl;
            m_playerName.push_back((Uint16)((c - 'a') + 0x2A));
            continue;
        }
        if (c >= 'g' && c <= 'v')
        {
            if (c == 'k')
            {
                m_playerName.push_back(0x42);
                continue;
            }
            if (c == 'i')
            {
                m_playerName.push_back(0x44);
                continue;
            }
            m_playerName.push_back((Uint16)((c - 'g') + 0x40));
            continue;
        }
        if (c >= 'w' && c <= 'z')
        {
            m_playerName.push_back((Uint16)((c - 'w') + 0x60));
            continue;
        }
        if (c >= '0' && c <= '9')
        {
            m_playerName.push_back((Uint16)((c - '0') + 0x64));
            continue;
        }
        if (c == '-' || c == '.')
        {
            m_playerName.push_back((Uint16)(c - '-') + 0x80);
            continue;
        }

        switch(c)
        {
            case '?' : m_playerName.push_back(0x6E); break;
            case '!' : m_playerName.push_back(0x6F); break;
            case ',' : m_playerName.push_back(0x82); break;
            case '(' : m_playerName.push_back(0x85); break;
            case ')' : m_playerName.push_back(0x86); break;
            case 'I' : m_playerName.push_back(0xAF); break;
        }
    }
}

std::vector<Uint16> ALTTPQuest::playerName() const
{
    return m_playerName;
}

std::string ALTTPQuest::playerNameToString() const
{
    std::string ret;
    std::vector<Uint16>::const_iterator iter = m_playerName.begin();

    for (; iter != m_playerName.end(); ++iter)
    {
        Int16 c = *iter;

        if (c >= 0x00 && c <= 0x0F)
        {
            ret.push_back((char)('A' + c));
            continue;
        }
        if (c >= 0x20 && c <= 0x29)
        {
            ret.push_back((char)('Q' + (c - 0x20)));
            continue;
        }
        if (c >= 0x2A && c <= 0x2F)
        {
            ret.push_back((char)('a' + (c - 0x2A)));
            continue;
        }
        if (c >= 0x40 && c <= 0x4F)
        {
            if (c == 0x42)
            {
                ret.push_back('k');
                continue;
            }
            if (c == 0x44)
            {
                ret.push_back('i');
                continue;
            }
            ret.push_back((char)('g' + (c - 0x40)));
        }
        if (c >= 0x60 && c <= 0x63)
        {
            ret.push_back((char)('w' + (c - 0x60)));
            continue;
        }
        if (c >= 0x64 && c <= 0x6D)
        {
            ret.push_back((char)('0' + (c - 0x64)));
            continue;
        }
        if (c == 0x80 || c == 0x81)
        {
            ret.push_back((char)('-' + (c - 0x80)));
            continue;
        }

        switch(c)
        {
            case 0x6E: ret.push_back('?'); break;
            case 0x6F: ret.push_back('!'); break;
            case 0x82: ret.push_back(','); break;
            case 0x85: ret.push_back('('); break;
            case 0x86: ret.push_back(')'); break;
            case 0xAF: ret.push_back('I'); break;
        }
    }
    return ret;
}

void ALTTPQuest::setValid(bool val)
{
    m_valid = val;
}

bool ALTTPQuest::valid()
{
    return m_valid;
}

void ALTTPQuest::setDungeonDeathTotals(std::vector<Uint16> val)
{
    m_dungeonDeathTotals = val;
}

void ALTTPQuest::setDungeonDeathTotal(Uint32 id, Uint16 val)
{
    if (id > m_dungeonDeathTotals.size())
        throw error::InvalidOperationException("ALTTPQuest::setDungeonDeathTotal -> index out of range");

    m_dungeonDeathTotals[id] = val;
}

Uint16 ALTTPQuest::dungeonDeathTotal(Uint32 id) const
{
    if (id > m_dungeonDeathTotals.size())
        throw error::InvalidOperationException("ALTTPQuest::setDungeonDeathTotal -> index out of range");

    return m_dungeonDeathTotals[id];
}

Uint16 ALTTPQuest::dungeonDeathTotalCount() const
{
    return m_dungeonDeathTotals.size();
}

void ALTTPQuest::setUnknown2(Uint16 val)
{
    m_unknown2 = val;
}

Uint16 ALTTPQuest::unknown2() const
{
    return m_unknown2;
}

void ALTTPQuest::setDeathSaveCount(Uint16 val)
{
    m_deathSaveCount = val;
}
Uint16 ALTTPQuest::deathSaveCount() const
{
    return m_deathSaveCount;
}

void ALTTPQuest::setPostGameDeathCounter(Int16 val)
{
    m_postGameDeathCounter = val;
}

Int16 ALTTPQuest::postGameDeathCounter() const
{
    return m_postGameDeathCounter;
}

void ALTTPQuest::setChecksum(Uint16 checksum)
{
    m_checksum = checksum;
}

Uint16 ALTTPQuest::checksum() const
{
    return m_checksum;
}

} // zelda
