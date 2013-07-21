#ifndef ZQUEST_HPP
#define ZQUEST_HPP

#include <Types.hpp>
#include <string>
#include <vector>

namespace zelda
{
class ZQuest
{
public:
    static const Uint32 Major;
    static const Uint32 Minor;
    static const Uint32 Revision;
    static const Uint32 Build;
    static const Uint32 Version;

    static const Uint32 Magic;

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

    ZQuest();
    ZQuest(Game game, Endian endian, Uint8* data, Uint32 length);
    ~ZQuest();

    void setGame(Game game);
    Game game() const;

    void setEndian(Endian endian);
    Endian endian() const;

    void setData(Uint8* data);
    Uint8* data() const;

    void setLength(Uint32 length);
    Uint32 length() const;

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
