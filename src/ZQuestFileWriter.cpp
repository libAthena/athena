#include "ZQuestFileWriter.hpp"
#include "InvalidOperationException.hpp"
#include "ZQuest.hpp"
#include "Compression.hpp"

namespace zelda
{

ZQuestFileWriter::ZQuestFileWriter(Uint8* data, Uint64 length)
    : base(data, length)
{
}

ZQuestFileWriter::ZQuestFileWriter(const std::string& filename)
    : base(filename)
{
}

void ZQuestFileWriter::write(ZQuest* quest, bool compress)
{
    if (!quest)
        throw InvalidOperationException("ZQuestFileWriter::writer -> quest cannot be NULL");

    base::writeUInt32(ZQuest::Magic);
    base::writeUInt32(ZQuest::Version);
    Uint8* questData = quest->data();
    Uint32 compLen = quest->length();
    if (compress)
    {
        Uint8* compData = new Uint8[quest->length() + 0x20]; // add 20 bytes because sometimes the file grows with compression
        io::Compression::compressZlib(questData, quest->length(), compData, &compLen);

        // if the compressed data is the same length or larger than the original data, just store the original
        if (compLen >= quest->length())
        {
            compLen = quest->length();
            // Delete the compressed data since we won't be using it
            delete[] compData;
            compData = NULL;
            base::writeUInt32(quest->length());
        }
        else
        {
            // Don't do delete on data
            questData = compData;
            base::writeUInt32(compLen);
        }
    }
    else
        base::writeUInt32(quest->length());

    base::writeUInt32(quest->length());
    base::writeUInt32(quest->game());
    base::writeUInt16(quest->endian() == BigEndian ? 0xFEFF : 0xFFFE);
    base::seek(0x0A);
    base::writeUBytes(questData, compLen);
}

} // zelda
