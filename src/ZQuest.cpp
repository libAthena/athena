#include "ZQuest.hpp"

namespace zelda
{

const Uint32 ZQuest::Major = 1;
const Uint32 ZQuest::Minor = 0;
const Uint32 ZQuest::Revision = 0;
const Uint32 ZQuest::Build = 0;

const Uint32 ZQuest::Version = Major | (Minor << 8) | (Revision << 16) | (Build << 24);

const Uint32 ZQuest::Magic   = 'Z' | ('Q' << 8) | ('S' << 16) | ('1' << 24);

ZQuest::ZQuest()
    : m_game(NoGame),
      m_endian(LittleEndian),
      m_data(NULL),
      m_length(0)
{
    initGameStrings();
}

ZQuest::ZQuest(ZQuest::Game game, Endian endian, Uint8* data, Uint32 length)
    : m_game(game),
      m_endian(endian),
      m_data(data),
      m_length(length)
{
    initGameStrings();
}

ZQuest::~ZQuest()
{
    delete[] m_data;
    m_data = NULL;
    m_length = 0;
}

void ZQuest::setGame(ZQuest::Game game)
{
    m_game = game;
}

ZQuest::Game ZQuest::game() const
{
    return m_game;
}

void ZQuest::setEndian(Endian endian)
{
    m_endian = endian;
}

Endian ZQuest::endian() const
{
    return m_endian;
}

void ZQuest::setData(Uint8* data)
{
    // ensure we're not overwritting our data without freeing first
    // or assigning unnecessisarily
    if (!m_data || m_data != data)
    {
        delete[] m_data;
        m_data = data;
    }
}

Uint8* ZQuest::data() const
{
    return m_data;
}

void ZQuest::setLength(Uint32 length)
{
    m_length = length;
}

Uint32 ZQuest::length() const
{
    return m_length;
}

std::string ZQuest::gameString() const
{
    if (m_game > m_gameStrings.size() - 1)
        return "Unsupported Game";

    return m_gameStrings[m_game];
}

void ZQuest::initGameStrings()
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
