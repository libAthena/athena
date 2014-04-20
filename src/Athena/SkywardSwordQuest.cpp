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

#include "Athena/SkywardSwordQuest.hpp"

namespace Athena
{
SkywardSwordQuest::SkywardSwordQuest(Uint8 *data, Uint32 len)
    : ZQuestFile(ZQuestFile::SS, Endian::BigEndian, data, len),
      m_skipData(nullptr),
      m_skipLength(0)
{
}

void SkywardSwordQuest::setSkipData(const Uint8 *data, Uint32 len)
{
    if (m_skipData)
    {
        delete[] m_skipData;
        m_skipData = nullptr;
    }

    m_skipData = (Uint8*)data;
    m_skipLength = len;
}

Uint8 *SkywardSwordQuest::skipData() const
{
    return m_skipData;
}

Uint32 SkywardSwordQuest::skipLength() const
{
    return m_skipLength;
}

} // zelda
