#include "SSFile.hpp"
#include "SSQuest.hpp"
#include "InvalidOperationException.hpp"

namespace zelda
{

SSFile::SSFile()
    : m_numQuests(0)
{
}

SSFile::SSFile(std::vector<SSQuest*> quests)
    : m_numQuests(0)
{
    m_quests = quests;
}

SSFile::~SSFile()
{
}

void SSFile::addQuest(zelda::SSQuest *q)
{
    // Do not allow more than 3 quests
    if (m_quests.size() >= 3)
        return;

    m_quests.push_back(q);
}

SSQuest *SSFile::quest(Uint32 id)
{
    if (id > m_quests.size() - 1)
        throw zelda::error::InvalidOperationException("SSFile::quest -> id cannot be "
                                                      "greater than the number of quests");

    return m_quests[id];
}

std::vector<SSQuest*> SSFile::questList() const
{
    return m_quests;
}

void SSFile::setRegion(Region region)
{
    m_region = region;
}

Region SSFile::region() const
{
    return m_region;
}


} // zelda
