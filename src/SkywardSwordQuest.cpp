#include "SkywardSwordQuest.hpp"

namespace zelda
{
SkywardSwordQuest::SkywardSwordQuest(Uint8 *data, Uint32 len)
    : ZQuestFile(ZQuestFile::SS, BigEndian, data, len),
      m_skipData(NULL),
      m_skipLength(0)
{
}

void SkywardSwordQuest::setSkipData(const Uint8 *data, Uint32 len)
{
    if (m_skipData)
    {
        delete[] m_skipData;
        m_skipData = NULL;
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
