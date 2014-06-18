#ifndef ATHENA_NO_ZQUEST
// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#include "Athena/ZQuestFileWriter.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/ZQuestFile.hpp"
#include "Athena/Compression.hpp"
#include "Athena/Checksums.hpp"

namespace Athena
{
namespace io
{

ZQuestFileWriter::ZQuestFileWriter(atUint8* data, atUint64 length)
    : base(data, length)
{
}

ZQuestFileWriter::ZQuestFileWriter(const std::string& filename)
    : base(filename)
{
}

void ZQuestFileWriter::write(ZQuestFile* quest, bool compress)
{
    if (!quest)
        THROW_INVALID_OPERATION_EXCEPTION("quest cannot be NULL");

    base::writeUint32(ZQuestFile::Magic);
    base::writeUint32(ZQuestFile::Version);
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
            base::writeUint32(quest->length());
        }
        else
        {
            // Don't do delete on data
            questData = compData;
            base::writeUint32(compLen);
        }
    }
    else
    {
        compLen = quest->length();
        base::writeUint32(quest->length());
    }

    base::writeUint32(quest->length());
    base::writeBytes((atInt8*)quest->gameString().substr(0, 0x0A).c_str(), 0x0A);
    base::writeUint16(quest->endian() == Endian::BigEndian ? 0xFFFE : 0xFEFF);
    base::writeUint32(Athena::Checksums::crc32(questData, compLen));
    base::writeUBytes(questData, compLen);

    base::save();
    // Delete compressed data to prevent memory leaks
    if (questData != quest->data())
    {
        delete[] questData;
        questData = NULL;
    }
}

} // io
} // zelda

#endif // ATHENA_NO_ZQUEST
