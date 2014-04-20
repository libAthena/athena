// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#include "Athena/SkywardSwordFile.hpp"
#include "Athena/SkywardSwordQuest.hpp"
#include "Athena/InvalidOperationException.hpp"

namespace Athena
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

void SkywardSwordFile::addQuest(Athena::SkywardSwordQuest *q)
{
    // Do not allow more than 3 quests
    if (m_quests.size() >= 3)
        return;

    m_quests.push_back(q);
}

SkywardSwordQuest *SkywardSwordFile::quest(Uint32 id)
{
    if (id > m_quests.size() - 1)
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

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
