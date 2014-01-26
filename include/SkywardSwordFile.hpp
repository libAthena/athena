#ifndef __SSFILE_HPP__
#define __SSFILE_HPP__

#include "Types.hpp"

// standard lib
#include <vector>

namespace zelda
{
class SkywardSwordQuest;

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
    SkywardSwordQuest* quest(Uint32 id);
    std::vector<SkywardSwordQuest*> questList() const;

    void setRegion(Region region);
    Region region() const;
private:
    Region m_region;
    // A vector is a bit overkill
    std::vector<SkywardSwordQuest*> m_quests;
    Uint32 m_numQuests;
};

}
#endif // __SSFILE_HPP__
