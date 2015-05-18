#ifndef MCSLOT_HPP
#define MCSLOT_HPP

#include "Athena/Global.hpp"
#include "Athena/ZQuestFile.hpp"

namespace Athena
{
class MCSlot : public ZQuestFile
{
public:
    MCSlot(atUint8* data, atUint32 length);
};

} // Athena

#endif // MCSLOT_HPP
