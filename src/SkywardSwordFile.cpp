#include "SkywardSwordFile.hpp"
#include "SkywardSwordQuest.hpp"
#include "InvalidOperationException.hpp"

namespace zelda
{

SkywardSwordFile::SkywardSwordFile()
    : m_numQuests(0)
{
}

SkywardSwordFile::SkywardSwordFile(std::vector<SkywardSwordQuest*> quests)
    : m_numQuests(0)
{
    m_quests = quests;
}

SkywardSwordFile::~SkywardSwordFile()
{
}

void SkywardSwordFile::addQuest(zelda::SkywardSwordQuest *q)
{
    // Do not allow more than 3 quests
    if (m_quests.size() >= 3)
        return;

    m_quests.push_back(q);
}

SkywardSwordQuest *SkywardSwordFile::quest(Uint32 id)
{
    if (id > m_quests.size() - 1)
        throw zelda::error::InvalidOperationException("SSFile::quest -> id cannot be "
                                                      "greater than the number of quests");

    return m_quests[id];
}

std::vector<SkywardSwordQuest*> SkywardSwordFile::questList() const
{
    return m_quests;
}

void SkywardSwordFile::setRegion(Region region)
{
    m_region = region;
}

Region SkywardSwordFile::region() const
{
    return m_region;
}


} // zelda
