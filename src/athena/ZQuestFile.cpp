#include "athena/ZQuestFile.hpp"

#include <cstring>

namespace athena {
std::vector<std::string> GameStrings;
void initGameStrings() {
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

const atUint32 ZQuestFile::Major = 2;
const atUint32 ZQuestFile::Minor = 0;
const atUint32 ZQuestFile::Revision = 0;

const atUint32 ZQuestFile::Version = Major | (Minor << 8) | (Revision << 16);

const atUint32 ZQuestFile::Magic = 'Z' | ('Q' << 8) | ('S' << 16) | (('0' + ZQuestFile::Major) << 24);

ZQuestFile::ZQuestFile() : m_game(NoGame), m_endian(Endian::Little), m_length(0) { initGameStrings(); }

ZQuestFile::ZQuestFile(ZQuestFile::Game game, Endian endian, std::unique_ptr<atUint8[]>&& data, atUint32 length,
                       const std::string& gameString)
: m_game(game), m_gameString(gameString), m_endian(endian), m_data(std::move(data)), m_length(length) {
  initGameStrings();

  if (gameString.empty() && (m_game < GameStrings.size() - 1))
    m_gameString = GameStrings[m_game];
}

void ZQuestFile::setGame(ZQuestFile::Game game) {
  m_game = game;

  if (m_game > GameStrings.size() - 1)
    return;

  m_gameString = GameStrings[m_game];
}

ZQuestFile::Game ZQuestFile::game() const { return m_game; }

void ZQuestFile::setEndian(Endian endian) { m_endian = endian; }

Endian ZQuestFile::endian() const { return m_endian; }

void ZQuestFile::setData(std::unique_ptr<atUint8[]>&& data, atUint32 length) {
  m_data = std::move(data);
  m_length = length;
}

atUint8* ZQuestFile::data() const { return m_data.get(); }

atUint32 ZQuestFile::length() const { return m_length; }

void ZQuestFile::setGameString(const std::string& gameString) { m_gameString = gameString; }

std::string ZQuestFile::gameString() const { return m_gameString; }

const std::vector<std::string> ZQuestFile::gameStringList() {
  if (GameStrings.size() <= 0)
    initGameStrings();

  return GameStrings;
}
} // namespace athena
