#if !defined(ATHENA_NO_SAVES) && !defined(ATHENA_NO_ZQUEST)
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

#include "Athena/SkywardSwordFileWriter.hpp"
#include "Athena/SkywardSwordFile.hpp"
#include "Athena/SkywardSwordQuest.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/InvalidDataException.hpp"

namespace Athena
{
namespace io
{

SkywardSwordFileWriter::SkywardSwordFileWriter(atUint8 *data, atUint64 len)
    : base(data, len)
{
    base::setEndian(Endian::BigEndian);
}

SkywardSwordFileWriter::SkywardSwordFileWriter(const std::string &filename)
    : base(filename)
{
    base::setEndian(Endian::BigEndian);
}

void SkywardSwordFileWriter::write(SkywardSwordFile *file)
{
    if (!file)
        THROW_INVALID_OPERATION_EXCEPTION("file cannot be NULL");

    atUint32 magic = (file->region() == Region::NTSC ? SkywardSwordFile::USMagic :
                   (file->region() == Region::NTSCJ ? SkywardSwordFile::JAMagic : SkywardSwordFile::EUMagic));

    base::writeUint32(magic);
    base::seek(0x1C, SeekOrigin::Begin);
    base::writeUint32(0x1D);

    std::vector<SkywardSwordQuest*> quests = file->questList();
    int i = 0;
    for (SkywardSwordQuest* q : quests)
    {
        if (q->length() != 0x53C0)
            THROW_INVALID_DATA_EXCEPTION("q->data() not 0x53C0 bytes in length");
        // Update the checksums
        q->fixChecksums();
        // Write the save data
        base::writeUBytes(q->data(), q->length());
        atUint64 pos = base::position();
        // Write the slots skip data
        base::seek(0xFB60 + (i * 0x24), SeekOrigin::Begin);
        base::writeUBytes(q->skipData(), 0x24);
        base::seek(pos, SeekOrigin::Begin);
        i++;
    }

    // write those padding bytes
    base::seek(0xFBE0, SeekOrigin::Begin);
    save();
}

} // io
} // zelda
#endif // ATHENA_NO_SAVES
