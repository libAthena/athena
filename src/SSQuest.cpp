#include "SSQuest.hpp"

namespace zelda
{
SSQuest::SSQuest(Uint8 *data, Uint32 len)
    : ZQuestFile(ZQuestFile::SS, BigEndian, data, len),
      m_skipData(NULL),
      m_skipLength(0)
{
}

void SSQuest::setSkipData(const Uint8 *data, Uint32 len)
{
    if (m_skipData)
    {
        delete[] m_skipData;
        m_skipData = NULL;
    }

    m_skipData = (Uint8*)data;
    m_skipLength = len;
}

Uint8 *SSQuest::skipData() const
{
    return m_skipData;
}

Uint32 SSQuest::skipLength() const
{
    return m_skipLength;
}

} // zelda
