#ifndef SKYWARDSWORDQUEST_HPP
#define SKYWARDSWORDQUEST_HPP

#include "Athena/Global.hpp"
#include "Athena/ZQuestFile.hpp"

namespace Athena
{

// TODO: Handle game specific data
class SkywardSwordQuest : public ZQuestFile
{
public:
    enum AmmoType
    {
        Arrows,
        Bombs,
        Seeds
    };

    SkywardSwordQuest(std::unique_ptr<atUint8[]>&& data, atUint32 len);

    void setPlayerName(const std::string& name);
    std::string playerName() const;

    void setRupeeCount(atUint16 value);
    atUint16 rupeeCount();
    void setAmmoCount(AmmoType type, atUint32 count);
    atUint32 ammoCount(AmmoType type);
    void setMaxHP(atUint16 val);
    atUint16 maxHP();
    float maxHearts();
    void setSpawnHP(atUint16 val);
    atUint16 spawnHP();
    float spawnHearts();
    void setCurrentHP(atUint16 val);
    atUint16 currentHP();
    float currentHearts();
    std::string currentLocation();
    std::string currentArea();
    std::string currentLocationCopy();

    void setSkipData(std::unique_ptr<atUint8[]>&& data);
    atUint8* skipData() const;


    atUint32 slotChecksum();
    atUint32 skipChecksum();
    void fixChecksums();

    void setNew(bool isNew);
    bool isNew() const;
private:
    std::unique_ptr<atUint8[]> m_skipData;
};


} // Athena
#endif // SSQUEST_HPP
