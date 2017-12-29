#include "athena/ZQuestFileWriter.hpp"
#include "athena/ZQuestFile.hpp"
#include "athena/Compression.hpp"
#include "athena/Checksums.hpp"

namespace athena::io
{

ZQuestFileWriter::ZQuestFileWriter(atUint8* data, atUint64 length)
    : MemoryCopyWriter(data, length)
{
}

ZQuestFileWriter::ZQuestFileWriter(const std::string& filename)
    : MemoryCopyWriter(filename)
{
}

void ZQuestFileWriter::write(ZQuestFile* quest, bool compress)
{
    if (!quest)
    {
        atError("quest cannot be NULL");
        return;
    }

    writeUint32(ZQuestFile::Magic);
    writeUint32(ZQuestFile::Version);
    atUint8* questData = quest->data();
    atUint32 compLen;

    if (compress)
    {
        atUint8* compData = new atUint8[quest->length() + 0x40]; // add 20 bytes because sometimes the file grows with compression
        compLen = quest->length() + 0x40;
        compLen = io::Compression::compressZlib(questData, quest->length(), compData, compLen);

        // if the compressed data is the same length or larger than the original data, just store the original
        if (compLen >= quest->length() || compLen <= 0)
        {
            compLen = quest->length();
            // Delete the compressed data since we won't be using it
            delete[] compData;
            compData = NULL;
            writeUint32(quest->length());
        }
        else
        {
            // Don't do delete on data
            questData = compData;
            writeUint32(compLen);
        }
    }
    else
    {
        compLen = quest->length();
        writeUint32(quest->length());
    }

    writeUint32(quest->length());
    writeBytes((atInt8*)quest->gameString().substr(0, 0x0A).c_str(), 0x0A);
    writeUint16(quest->endian() == Endian::BigEndian ? 0xFFFE : 0xFEFF);
    writeUint32(athena::Checksums::crc32(questData, compLen));
    writeUBytes(questData, compLen);

    save();

    // Delete compressed data to prevent memory leaks
    if (questData != quest->data())
    {
        delete[] questData;
        questData = NULL;
    }
}

} // zelda
