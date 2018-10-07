#pragma once

#include "athena/Global.hpp"

// standard lib
#include <vector>

namespace athena
{
class SkywardSwordQuest;

enum class Region
{
    NTSC,
    NTSCJ,
    PAL
};

class SkywardSwordFile
{
public:
    enum MagicNumbers
    {
        USMagic = 0x534F5545,
        JAMagic = 0x534F554A,
        EUMagic = 0x534F5550
    };

    SkywardSwordFile();
    SkywardSwordFile(std::vector<SkywardSwordQuest*> quests);
    ~SkywardSwordFile();

    void addQuest(SkywardSwordQuest* q);
    SkywardSwordQuest* quest(atUint32 id);
    std::vector<SkywardSwordQuest*> questList() const;

    void setRegion(Region region);
    Region region() const;

private:
    Region m_region;
    // A vector is a bit overkill
    std::vector<SkywardSwordQuest*> m_quests;
    atUint32 m_numQuests;
};

}
