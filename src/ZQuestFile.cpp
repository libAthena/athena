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

#include "ZQuestFile.hpp"
#include <string.h>

namespace zelda
{
std::vector<std::string> GameStrings;
void initGameStrings()
{
    // Populate game strings
    GameStrings.push_back("No Game");
    GameStrings.push_back("Legend Of Zelda");
    GameStrings.push_back("Adventure of Link");
    GameStrings.push_back("A Link to the Past");
    GameStrings.push_back("Links Awakening");
    GameStrings.push_back("Ocarina of Time");
    GameStrings.push_back("Ocarina of Time 3D");
    GameStrings.push_back("Majora's Mask");
    GameStrings.push_back("Oracle of Seasons");
    GameStrings.push_back("Oracle of Ages");
    GameStrings.push_back("For Swords");
    GameStrings.push_back("Wind Waker");
    GameStrings.push_back("Four Swords Adventures");
    GameStrings.push_back("Minish Cap");
    GameStrings.push_back("Twilight Princess");
    GameStrings.push_back("Phantom Hourglass");
    GameStrings.push_back("Spirit Tracks");
    GameStrings.push_back("Skyward Sword");
    GameStrings.push_back("A Link Between Worlds");
}

const Uint32 ZQuestFile::Major = 2;
const Uint32 ZQuestFile::Minor = 0;
const Uint32 ZQuestFile::Revision = 0;

const Uint32 ZQuestFile::Version = Major | (Minor << 8) | (Revision << 16);

const Uint32 ZQuestFile::Magic   = 'Z' | ('Q' << 8) | ('S' << 16) | (('0' + ZQuestFile::Major) << 24);

ZQuestFile::ZQuestFile()
    : m_game(NoGame),
      m_endian(LittleEndian),
      m_data(NULL),
      m_length(0)
{
    initGameStrings();
}

ZQuestFile::ZQuestFile(ZQuestFile::Game game, Endian endian, Uint8* data, Uint32 length, const std::string& gameString)
    : m_game(game),
      m_gameString(gameString),
      m_endian(endian),
      m_data(data),
      m_length(length)
{
    initGameStrings();
    if (gameString.empty() && (m_game < GameStrings.size() - 1))
        m_gameString = GameStrings[m_game];
}

ZQuestFile::~ZQuestFile()
{
    delete[] m_data;
    m_data = NULL;
    m_length = 0;
}

void ZQuestFile::setGame(ZQuestFile::Game game)
{
    m_game = game;
    if (m_game > GameStrings.size() - 1)
        return;

    m_gameString = GameStrings[m_game];
}

ZQuestFile::Game ZQuestFile::game() const
{
    return m_game;
}

void ZQuestFile::setEndian(Endian endian)
{
    m_endian = endian;
}

Endian ZQuestFile::endian() const
{
    return m_endian;
}

void ZQuestFile::setData(Uint8* data, Uint32 length)
{
    // ensure we're not overwritting our data without freeing first
    // or assigning unnecessisarily
    if (!m_data || m_data != data)
    {
        delete[] m_data;
        m_data = new Uint8[length];
        // Why is this memcpy needed?
        // I get SIGABRT without it, need to research
        memcpy(m_data, data, length);
        m_length = length;
    }
}

Uint8* ZQuestFile::data() const
{
    return m_data;
}


Uint32 ZQuestFile::length() const
{
    return m_length;
}

void ZQuestFile::setGameString(const std::string& gameString)
{
    m_gameString = gameString;
}

std::string ZQuestFile::gameString() const
{
    return m_gameString;
}

const std::vector<std::string> ZQuestFile::gameStringList()
{
    if (GameStrings.size() <= 0)
        initGameStrings();
    return GameStrings;
}
}
