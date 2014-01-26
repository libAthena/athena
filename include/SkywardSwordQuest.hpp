#ifndef SKYWARDSWORDQUEST_HPP
#define SKYWARDSWORDQUEST_HPP

#include "ZQuestFile.hpp"

namespace zelda
{

// TODO: Handle game specific data
class SkywardSwordQuest : public ZQuestFile
{
public:
    SkywardSwordQuest(Uint8* data, Uint32 len);

    // TODO: Is len really needed?
    void setSkipData(const Uint8* data, Uint32 len = 0x24);
    Uint8* skipData() const;

    Uint32 skipLength() const;

private:
    Uint8* m_skipData;
    Uint32 m_skipLength;
};


} // zelda
#endif // SSQUEST_HPP
