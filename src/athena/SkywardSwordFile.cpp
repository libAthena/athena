#include "athena/SkywardSwordFile.hpp"
#include "athena/SkywardSwordQuest.hpp"

namespace athena
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

void SkywardSwordFile::addQuest(athena::SkywardSwordQuest* q)
{
    // Do not allow more than 3 quests
    if (m_quests.size() >= 3)
        return;

    m_quests.push_back(q);
}

SkywardSwordQuest* SkywardSwordFile::quest(atUint32 id)
{
    if (id > m_quests.size() - 1)
    {
        atWarning("index out of range");
        return nullptr;
    }

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
