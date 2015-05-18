#include "Athena/MCSlot.hpp"

namespace Athena
{

MCSlot::MCSlot(atUint8* data, atUint32 length)
    : ZQuestFile(ZQuestFile::MC, Endian::LittleEndian, data, length)
{
}

} // Athena
