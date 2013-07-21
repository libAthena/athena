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

#ifndef ZQUEST_HPP
#define ZQUEST_HPP

#include <Types.hpp>
#include <string>
#include <vector>

namespace zelda
{
/*!
 * \brief The ZQuest class
 */
class ZQuest
{
public:
    /*!
     * \brief Major
     */
    static const Uint32 Major;
    /*!
     * \brief Minor
     */
    static const Uint32 Minor;
    /*!
     * \brief Revision
     */
    static const Uint32 Revision;
    /*!
     * \brief Build
     */
    static const Uint32 Build;
    /*!
     * \brief Version
     */
    static const Uint32 Version;

    /*!
     * \brief Magic
     */
    static const Uint32 Magic;

    /*!
     * \brief The Game enum
     */
    enum Game
    {
        NoGame,
        LegendofZelda,
        AdventureOfLink,
        ALinkToThePast,
        LinksAwakening,
        OcarinaOfTime,
        OcarinaOfTime3D,
        MajorasMask,
        OracleOfSeasons,
        OracleOfAges,
        FourSwords,
        WindWaker,
        FourSwordsAdventures,
        MinishCap,
        TwilightPrincess,
        PhantomHourglass,
        SpiritTracks,
        SkywardSword,
        ALinkBetweenWorlds // Not released
    };

    /*!
     * \brief ZQuest
     */
    ZQuest();

    /*!
     * \brief ZQuest
     * \param game
     * \param endian
     * \param data
     * \param length
     */
    ZQuest(Game game, Endian endian, Uint8* data, Uint32 length);
    ~ZQuest();

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
     * \param data
     */
    void setData(Uint8* data);

    /*!
     * \brief data
     * \return
     */
    Uint8* data() const;

    /*!
     * \brief setLength
     * \param length
     */
    void setLength(Uint32 length);

    /*!
     * \brief length
     * \return
     */
    Uint32 length() const;

    /*!
     * \brief gameString
     * \return
     */
    std::string gameString() const;
private:
    Game   m_game;
    Endian m_endian;
    Uint8* m_data;
    Uint32 m_length;

    // Game strings support
    std::vector<std::string> m_gameStrings;
    void initGameStrings();
};
} // zelda

#endif // ZQUEST_HPP
