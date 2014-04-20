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

#include "Athena/SkywardSwordFileReader.hpp"
#include "Athena/SkywardSwordFile.hpp"
#include "Athena/SkywardSwordQuest.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/InvalidDataException.hpp"
#include <iostream>

namespace Athena
{
namespace io
{

SkywardSwordFileReader::SkywardSwordFileReader(Uint8* data, Uint64 length)
    : base(data, length)
{
    base::setEndian(Endian::BigEndian);
}

SkywardSwordFileReader::SkywardSwordFileReader(const std::string& filename)
    : base(filename)
{
    base::setEndian(Endian::BigEndian);
}

SkywardSwordFile* SkywardSwordFileReader::read()
{
    SkywardSwordFile* file = NULL;
    try
    {
        if (base::length() != 0xFBE0)
            THROW_INVALID_DATA_EXCEPTION("File not the expected size of 0xFBE0");

        Uint32 magic = base::readUint32();

        if (magic != SkywardSwordFile::USMagic && magic != SkywardSwordFile::JAMagic && magic != SkywardSwordFile::EUMagic)
            THROW_INVALID_DATA_EXCEPTION("Not a valid Skyward Sword save file");

        base::seek(0x01C, SeekOrigin::Begin);
        Uint32 headerSize = base::readUint32(); // Seems to be (headerSize - 1)

        if (headerSize != 0x1D)
            THROW_INVALID_DATA_EXCEPTION("Invalid header size, Corrupted data?");

        // Time to read in each slot
        file = new SkywardSwordFile;
        file->setRegion((magic == SkywardSwordFile::USMagic ? Region::NTSC: (magic == SkywardSwordFile::JAMagic ? Region::NTSCJ: Region::PAL)));
        for (int i = 0; i < 3; i++)
        {
            SkywardSwordQuest* q = new SkywardSwordQuest((Uint8*)base::readBytes(0x53C0), 0x53C0);
            Uint64 pos = base::position();
            // seek to the skip data for this particular quest
            base::seek(0xFB60 + (i * 0x24), SeekOrigin::Begin);
            q->setSkipData(base::readUBytes(0x24));
            base::seek(pos, SeekOrigin::Begin);
            file->addQuest(q);
        }
    }
    catch(...)
    {
        delete file;
        file = NULL;
        throw;
    }

    return file;
}

} // io
} // zelda
