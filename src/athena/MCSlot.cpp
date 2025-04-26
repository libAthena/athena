#include "athena/MCSlot.hpp"

namespace athena {

MCSlot::MCSlot(std::unique_ptr<uint8_t[]>&& data, uint32_t length)
: ZQuestFile(ZQuestFile::MC, Endian::Little, std::move(data), length) {}

} // namespace athena
