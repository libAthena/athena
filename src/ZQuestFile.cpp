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

namespace zelda
{

const Uint32 ZQuestFile::Major = 1;
const Uint32 ZQuestFile::Minor = 0;
const Uint32 ZQuestFile::Revision = 0;
const Uint32 ZQuestFile::Build = 0;

const Uint32 ZQuestFile::Version = Major | (Minor << 8) | (Revision << 16) | (Build << 24);

const Uint32 ZQuestFile::Magic   = 'Z' | ('Q' << 8) | ('S' << 16) | ('1' << 24);

ZQuestFile::ZQuestFile()
    : m_game(NoGame),
      m_endian(LittleEndian),
      m_data(NULL),
      m_length(0)
{
    initGameStrings();
}

ZQuestFile::ZQuestFile(ZQuestFile::Game game, Endian endian, Uint8* data, Uint32 length)
    : m_game(game),
      m_endian(endian),
      m_data(data),
      m_length(length)
{
    initGameStrings();
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
        m_data = data;
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

std::string ZQuestFile::gameString() const
{
    if (m_game > m_gameStrings.size() - 1)
        return "Unsupported Game";

    return m_gameStrings[m_game];
}

void ZQuestFile::initGameStrings()
{
    m_gameStrings.push_back("No Game");
    m_gameStrings.push_back("Legend Of Zelda");
    m_gameStrings.push_back("Adventure of Link");
    m_gameStrings.push_back("A Link to the Past");
    m_gameStrings.push_back("Links Awakening");
    m_gameStrings.push_back("Ocarina of Time");
    m_gameStrings.push_back("Ocarina of Time 3D");
    m_gameStrings.push_back("Majora's Mask");
    m_gameStrings.push_back("Oracle of Seasons");
    m_gameStrings.push_back("Oracle of Ages");
    m_gameStrings.push_back("For Swords");
    m_gameStrings.push_back("Wind Waker");
    m_gameStrings.push_back("Four Swords Adventures");
    m_gameStrings.push_back("Minish Cap");
    m_gameStrings.push_back("Twilight Princess");
    m_gameStrings.push_back("Phantom Hourglass");
    m_gameStrings.push_back("Spirit Tracks");
    m_gameStrings.push_back("Skyward Sword");
    m_gameStrings.push_back("A Link Between Worlds (Unreleased)");
}

}
