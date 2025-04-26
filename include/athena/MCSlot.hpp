#pragma once

#include "athena/Global.hpp"
#include "athena/ZQuestFile.hpp"

namespace athena {
class MCSlot : public ZQuestFile {
public:
  MCSlot(std::unique_ptr<uint8_t[]>&& data, uint32_t length);
};

} // namespace athena
