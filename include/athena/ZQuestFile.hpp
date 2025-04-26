#ifndef ATHENA_NO_ZQUEST
#ifndef ZQUEST_HPP
#define ZQUEST_HPP

#include "athena/Global.hpp"
#include <string>
#include <vector>
#include <memory>

#define ZQUEST_VERSION_CHECK(major, minor, revision) (major | (minor << 8) | (revision << 16))

namespace athena {
/*!
 * \brief ZQuestFile is an export format for save data.
 */
class ZQuestFile {
public:
  /*!
   * \brief The current major version of the ZQuest format
   */
  static const uint32_t Major;
  /*!
   * \brief The current minor version of the ZQuest format
   */
  static const uint32_t Minor;
  /*!
   * \brief The current revision of the ZQuest format
   */
  static const uint32_t Revision;
  /*!
   * \brief The current version of the ZQuest format
   */
  static const uint32_t Version;

  /*!
   * \brief The magic number used to identify the file e.g. "ZQS1"
   */
  static const uint32_t Magic;

  /*!
   * \enum Game
   * \brief The list of games currently supported by ZQuest
   */
  enum Game {
    NoGame, //!< None or Unsupported
    LoZ,    //!< Legend of Zelda
    AoL,    //!< Adventure of Link
    ALttP,  //!< A Link to the Past
    LA,     //!< Links Awakening
    OoT,    //!< Ocarin of Time
    OoT3D,  //!< Ocarina of Time 3D
    MM,     //!< Majora's Mask
    OoS,    //!< Oracle of Season
    OoA,    //!< Oracle of Ages
    FS,     //!< Four Swords
    WW,     //!< Wind Waker
    FSA,    //!< Four Swords Adventures
    MC,     //!< Minish Cap
    TP,     //!< Twilight Princess
    PH,     //!< Phantom Hourglass
    ST,     //!< Spirit Tracks
    SS,     //!< Skyward Sword
    ALBW,   //!< A Link Between Worlds
    // Add more games here

    // This must always be last
    GameCount //!< Total number of supported games
  };

  /*!
   * \brief ZQuest
   */
  ZQuestFile();

  /*!
   * \brief ZQuest
   * \param game
   * \param endian
   * \param data
   * \param length
   */
  ZQuestFile(Game game, Endian endian, std::unique_ptr<uint8_t[]>&& data, uint32_t length,
             const std::string& gameString = std::string());

  /*!
   * \brief setGame
   * \param game
   */
  void setGame(Game game);

  /*!
   * \brief game
   * \return
   */
  Game game() const;

  /*!
   * \brief setEndian
   * \param endian
   */
  void setEndian(Endian endian);

  /*!
   * \brief endian
   * \return
   */
  Endian endian() const;

  /*!
   * \brief setData
   * \param data   The data to assign
   * \param length The length of the data
   */
  void setData(std::unique_ptr<uint8_t[]>&& data, uint32_t length);

  /*!
   * \brief data
   * \return
   */
  uint8_t* data() const;

  /*!
   * \brief length
   * \return
   */
  uint32_t length() const;

  void setGameString(const std::string& gameString);
  /*!
   * \brief gameString
   * \return
   */
  std::string gameString() const;

  static const std::vector<std::string> gameStringList();

protected:
  Game m_game;
  std::string m_gameString;
  Endian m_endian;
  std::unique_ptr<uint8_t[]> m_data;
  uint32_t m_length;

  // Game strings support
};
} // namespace athena

#endif // ZQUEST_HPP
#endif // ATHENA_NO_ZQUEST
