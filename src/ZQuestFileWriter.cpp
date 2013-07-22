// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#include "ZQuestFileWriter.hpp"
#include "InvalidOperationException.hpp"
#include "ZQuestFile.hpp"
#include "Compression.hpp"

namespace zelda
{
namespace io
{

ZQuestFileWriter::ZQuestFileWriter(Uint8* data, Uint64 length)
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
        throw error::InvalidOperationException("ZQuestFileWriter::write -> quest cannot be NULL");

    base::writeUInt32(ZQuestFile::Magic);
    base::writeUInt32(ZQuestFile::Version);
    Uint8* questData = quest->data();
    Uint32 compLen;
    if (compress)
    {
        Uint8* compData = new Uint8[quest->length() + 0x40]; // add 20 bytes because sometimes the file grows with compression
        compLen = quest->length() + 0x40;
        compLen = io::Compression::compressZlib(questData, quest->length(), compData, compLen);

        // if the compressed data is the same length or larger than the original data, just store the original
        if (compLen >= quest->length() || compLen <= 0)
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
    {
        compLen = quest->length();
        base::writeUInt32(quest->length());
    }

    base::writeUInt32(quest->length());
    base::writeUInt32(quest->game());
    base::writeUInt16(quest->endian() == BigEndian ? 0xFEFF : 0xFFFE);
    base::seek(0x0A);
    base::writeUBytes(questData, compLen);

    base::save();
    // Delete compressed data to preven memory leaks
    if (questData != quest->data())
    {
        delete[] questData;
        questData = NULL;
    }
}

} // io
} // zelda
