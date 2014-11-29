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

#include "Athena/ALTTPQuest.hpp"
#include "Athena/InvalidOperationException.hpp"
#include <iostream>

namespace Athena
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

void ALTTPQuest::setRoomFlags(ALTTPRoomFlags* rf, atUint32 id)
{
    m_roomFlags[id] = rf;
}

std::vector<ALTTPRoomFlags*> ALTTPQuest::roomFlags()
{
    return m_roomFlags;
}

ALTTPRoomFlags* ALTTPQuest::roomFlags(atUint32 id)
{
    return m_roomFlags[id];
}

void ALTTPQuest::setOverworldEvents(std::vector<ALTTPOverworldEvent*> ow)
{
    m_overworldEvents = ow;
}

void ALTTPQuest::setOverworldEvents(ALTTPOverworldEvent* ow, atUint32 id)
{
    m_overworldEvents[id] = ow;
}

std::vector<ALTTPOverworldEvent*> ALTTPQuest::overworldEvents() const
{
    return m_overworldEvents;
}

ALTTPOverworldEvent* ALTTPQuest::overworldEvent(atUint32 id) const
{
    if (id > m_overworldEvents.size() - 1)
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");
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

void ALTTPQuest::setRupeeMax(atUint16 val)
{
    m_rupeeMax = val;
}

atUint16 ALTTPQuest::rupeeMax() const
{
    return m_rupeeMax;
}

void ALTTPQuest::setRupeeCurrent(atUint16 val)
{
    m_rupeeCurrent = val;
}

atUint16 ALTTPQuest::rupeeCurrent() const
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

void ALTTPQuest::setWishingPond(atUint16 val)
{
    m_wishingPond = val;
}

atUint16 ALTTPQuest::wishingPond() const
{
    return m_wishingPond;
}

void ALTTPQuest::setHealthMax(atUint8 val)
{
    m_healthMax = val;
}

atUint8 ALTTPQuest::healthMax() const
{
    return m_healthMax;
}

void ALTTPQuest::setHealth(atUint8 val)
{
    m_health = val;
}

atUint8 ALTTPQuest::health() const
{
    return m_health;
}

void ALTTPQuest::setMagicPower(atUint8 val)
{
    m_magicPower = val;
}

atUint8 ALTTPQuest::magicPower() const
{
    return m_magicPower;
}

void ALTTPQuest::setKeys(atUint8 val)
{
    m_keys = val;
}

atUint8 ALTTPQuest::keys() const
{
    return m_keys;
}

void ALTTPQuest::setBombUpgrades(atUint8 val)
{
    m_bombUpgrades = val;
}

atUint8 ALTTPQuest::bombUpgrades() const
{
    return m_bombUpgrades;
}

void ALTTPQuest::setArrowUpgrades(atUint8 val)
{
    m_arrowUpgrades = val;
}

atUint8 ALTTPQuest::arrowUpgrades() const
{
    return m_arrowUpgrades;
}

void ALTTPQuest::setHealthFiller(atUint8 val)
{
    m_heartFiller = val;
}

atUint8 ALTTPQuest::healthFiller() const
{
    return m_heartFiller;
}
void ALTTPQuest::setMagicFiller(atUint8 val)
{
    m_heartFiller = val;
}

atUint8 ALTTPQuest::magicFiller() const
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

void ALTTPQuest::setBombFiller(atUint8 val)
{
    m_bombFiller = val;
}

atUint8 ALTTPQuest::bombFiller() const
{
    return m_bombFiller;
}

void ALTTPQuest::setArrowFiller(atUint8 val)
{
    m_arrowFiller = val;
}

atUint8 ALTTPQuest::arrowFiller() const
{
    return m_arrowFiller;
}
void ALTTPQuest::setArrows(atUint8 val)
{
    m_arrows = val;
}

atUint8 ALTTPQuest::arrows() const
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

void ALTTPQuest::setDungeonKeys(std::vector<atUint8> val)
{
    m_dungeonKeys = val;
}

void ALTTPQuest::setDungeonKeys(atUint32 id, atUint8 val)
{
    if (id > m_dungeonKeys.size() - 1)
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    m_dungeonKeys[id] = val;
}

atUint8 ALTTPQuest::dungeonKeys(atUint32 id) const
{
    if (id > m_dungeonKeys.size() - 1)
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    return m_dungeonKeys[id];
}

atUint32 ALTTPQuest::dungeonCount() const
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

void ALTTPQuest::setOldManFlags(std::vector<atUint8> flags)
{
    m_oldManFlags = flags;
}

void ALTTPQuest::setOldManFlag(atUint32 id, atUint8 val)
{
    if (id > m_oldManFlags.size() - 1)
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    m_oldManFlags[id] = val;
}

atUint8 ALTTPQuest::oldManFlag(atUint32 id)
{
    if (id > m_oldManFlags.size() - 1)
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    return m_oldManFlags[id];
}

atUint32 ALTTPQuest::oldManFlagCount() const
{
    return m_oldManFlags.size();
}

void ALTTPQuest::setBombFlag(atUint8 flag)
{
    m_bombFlag = flag;
}

atUint8 ALTTPQuest::bombFlag() const
{
    return m_bombFlag;
}

void ALTTPQuest::setUnknown1(std::vector<atUint8> flags)
{
    m_unknown1 = flags;
}

void ALTTPQuest::setUnknown1(atUint32 id, atUint8 val)
{
    if (id > m_unknown1.size())
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    m_unknown1[id] = val;
}

atUint8 ALTTPQuest::unknown1(atUint32 id)
{
    if (id > m_unknown1.size())
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    return m_unknown1[id];
}

atUint32 ALTTPQuest::unknown1Count() const
{
    return m_unknown1.size();
}

void ALTTPQuest::setPlayerName(std::vector<atUint16> playerName)
{
    m_playerName = playerName;
}

void ALTTPQuest::setPlayerName(const std::string& playerName)
{
    if (playerName == std::string() || playerName.size() > 6)
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    m_playerName.clear();

    for (atUint32 i = 0; i < 6; i++)
    {
        if (i > playerName.size() - 1)
        {
            m_playerName.push_back((atUint16)0xA9);
            continue;
        }
        char c = playerName[i];
        if (c >= 'A' && c <= 'P' && c != 'I')
        {

            m_playerName.push_back((atUint16)(c - 'A'));
            continue;
        }
        if (c >= 'Q' && c <= 'Z')
        {
            std::cout << std::hex << (atUint16)((c - 'Q') + 0x20) << std::endl;
            m_playerName.push_back((atUint16)((c - 'Q') + 0x20));
            continue;
        }
        if (c >= 'a' && c <= 'f')
        {
            std::cout << std::hex << (atUint16)((c - 'a') + 0x2A) << std::endl;
            m_playerName.push_back((atUint16)((c - 'a') + 0x2A));
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
            m_playerName.push_back((atUint16)((c - 'g') + 0x40));
            continue;
        }
        if (c >= 'w' && c <= 'z')
        {
            m_playerName.push_back((atUint16)((c - 'w') + 0x60));
            continue;
        }
        if (c >= '0' && c <= '9')
        {
            m_playerName.push_back((atUint16)((c - '0') + 0x64));
            continue;
        }
        if (c == '-' || c == '.')
        {
            m_playerName.push_back((atUint16)(c - '-') + 0x80);
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

std::vector<atUint16> ALTTPQuest::playerName() const
{
    return m_playerName;
}

std::string ALTTPQuest::playerNameToString() const
{
    std::string ret;

    for (atInt16 c : m_playerName)
    {
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

void ALTTPQuest::setDungeonDeathTotals(std::vector<atUint16> val)
{
    m_dungeonDeathTotals = val;
}

void ALTTPQuest::setDungeonDeathTotal(atUint32 id, atUint16 val)
{
    if (id > m_dungeonDeathTotals.size())
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    m_dungeonDeathTotals[id] = val;
}

atUint16 ALTTPQuest::dungeonDeathTotal(atUint32 id) const
{
    if (id > m_dungeonDeathTotals.size())
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    return m_dungeonDeathTotals[id];
}

atUint16 ALTTPQuest::dungeonDeathTotalCount() const
{
    return m_dungeonDeathTotals.size();
}

void ALTTPQuest::setUnknown2(atUint16 val)
{
    m_unknown2 = val;
}

atUint16 ALTTPQuest::unknown2() const
{
    return m_unknown2;
}

void ALTTPQuest::setDeathSaveCount(atUint16 val)
{
    m_deathSaveCount = val;
}
atUint16 ALTTPQuest::deathSaveCount() const
{
    return m_deathSaveCount;
}

void ALTTPQuest::setPostGameDeathCounter(atInt16 val)
{
    m_postGameDeathCounter = val;
}

atInt16 ALTTPQuest::postGameDeathCounter() const
{
    return m_postGameDeathCounter;
}

void ALTTPQuest::setChecksum(atUint16 checksum)
{
    m_checksum = checksum;
}

atUint16 ALTTPQuest::checksum() const
{
    return m_checksum;
}

} // zelda
#endif // ATHENA_NO_SAVES
