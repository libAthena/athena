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

#include "Athena/ALTTPFile.hpp"
#include "Athena/ALTTPQuest.hpp"

#include "Athena/InvalidOperationException.hpp"

namespace Athena
{
ALTTPFile::ALTTPFile()
{}

ALTTPFile::ALTTPFile(std::vector<ALTTPQuest*> quests, std::vector<ALTTPQuest*> backup)
    : m_quests(quests)
    , m_backup(backup)
{
}

void ALTTPFile::setQuest(Uint32 id, ALTTPQuest* val)
{
    if (id > m_quests.size())
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    m_quests[id] = val;
}

std::vector<ALTTPQuest*> ALTTPFile::questList() const
{
    return m_quests;
}
ALTTPQuest* ALTTPFile::quest(Uint32 id) const
{
    if (id > m_quests.size())
        THROW_INVALID_OPERATION_EXCEPTION("index out of range");

    return m_quests[id];
}

Uint32 ALTTPFile::questCount() const
{
    return m_quests.size();
}
} // zelda
