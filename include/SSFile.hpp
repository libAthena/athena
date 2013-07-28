#ifndef __SSFILE_HPP__
#define __SSFILE_HPP__

#include "Types.hpp"

// standard lib
#include <vector>

namespace zelda
{
class SSQuest;

class SSFile
{
public:
    enum MagicNumbers
    {
        USMagic = 0x534F5545,
        JAMagic = 0x534F554A,
        EUMagic = 0x534F5550
    };

    SSFile();
    SSFile(std::vector<SSQuest*> quests);
    ~SSFile();

    void addQuest(SSQuest* q);
    SSQuest* quest(Uint32 id);
    std::vector<SSQuest*> questList() const;

    void setRegion(Region region);
    Region region() const;
private:
    Region m_region;
    // A vector is a bit overkill
    std::vector<SSQuest*> m_quests;
    Uint32 m_numQuests;
};

}
#endif // __SSFILE_HPP__
