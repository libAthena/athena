#include "athena/MCSlot.hpp"

namespace athena
{

MCSlot::MCSlot(std::unique_ptr<atUint8[]>&& data, atUint32 length)
    : ZQuestFile(ZQuestFile::MC, Endian::LittleEndian, std::move(data), length)
{
}

} // Athena
