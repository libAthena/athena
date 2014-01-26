#include "SkywardSwordFileReader.hpp"
#include "SkywardSwordFile.hpp"
#include "SkywardSwordQuest.hpp"
#include "InvalidOperationException.hpp"
#include <iostream>

namespace zelda
{
namespace io
{

SkywardSwordFileReader::SkywardSwordFileReader(Uint8* data, Uint64 length)
    : base(data, length)
{
    base::setEndianess(BigEndian);
}

SkywardSwordFileReader::SkywardSwordFileReader(const std::string& filename)
    : base(filename)
{
    base::setEndianess(BigEndian);
}

SkywardSwordFile* SkywardSwordFileReader::read()
{
    SkywardSwordFile* file = NULL;
    if (base::length() != 0xFBE0)
        throw zelda::error::InvalidOperationException("SSFileReader::read -> File not the expected size of 0xFBE0");

    Uint32 magic = base::readUInt32();

    if (magic != SkywardSwordFile::USMagic && magic != SkywardSwordFile::JAMagic && magic != SkywardSwordFile::EUMagic)
        throw zelda::error::InvalidOperationException("SSFileReader::read -> Not a valid Skyward Sword save file");

    base::seek(0x01C, base::Beginning);
    Uint32 headerSize = base::readUInt32(); // Seems to be (headerSize - 1)

    if (headerSize != 0x1D)
        throw zelda::error::InvalidOperationException("SSFileHeader::read -> Invalid header size, Corrupted data?");

    // Time to read in each slot
    file = new SkywardSwordFile;
    file->setRegion((magic == SkywardSwordFile::USMagic ? NTSCURegion : (magic == SkywardSwordFile::JAMagic ? NTSCJRegion : PALRegion)));
    for (int i = 0; i < 3; i++)
    {
        SkywardSwordQuest* q = new SkywardSwordQuest((Uint8*)base::readBytes(0x53C0), 0x53C0);
        Uint64 pos = base::position();
        // seek to the skip data for this particular quest
        base::seek(0xFB60 + (i * 0x24), base::Beginning);
        q->setSkipData(base::readUBytes(0x24));
        base::seek(pos, base::Beginning);
        file->addQuest(q);
    }

    return file;
}

} // io
} // zelda
