#ifndef SSQUEST_HPP
#define SSQUEST_HPP

#include "ZQuestFile.hpp"

namespace zelda
{

// TODO: Handle game specific data
class SSQuest : public ZQuestFile
{
public:
    SSQuest(Uint8* data, Uint32 len);

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
