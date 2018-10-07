#pragma once

#include "athena/Global.hpp"
#include "athena/ZQuestFile.hpp"

namespace athena
{
class MCSlot : public ZQuestFile
{
public:
    MCSlot(std::unique_ptr<atUint8[]>&& data, atUint32 length);
};

} // Athena

