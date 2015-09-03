#include "Athena/SkywardSwordFileReader.hpp"
#include "Athena/SkywardSwordFile.hpp"
#include "Athena/SkywardSwordQuest.hpp"
#include <iostream>

namespace Athena
{
namespace io
{

SkywardSwordFileReader::SkywardSwordFileReader(atUint8* data, atUint64 length)
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

    if (base::length() != 0xFBE0)
    {
        atError("File not the expected size of 0xFBE0");
        return nullptr;
    }

    atUint32 magic = base::readUint32();

    if (magic != SkywardSwordFile::USMagic && magic != SkywardSwordFile::JAMagic && magic != SkywardSwordFile::EUMagic)
    {
        atError("Not a valid Skyward Sword save file");
        return nullptr;
    }

    base::seek(0x01C, SeekOrigin::Begin);
    atUint32 headerSize = base::readUint32(); // Seems to be (headerSize - 1)

    if (headerSize != 0x1D)
    {
        atError("Invalid header size, Corrupted data?");
        return nullptr;
    }

    // Time to read in each slot
    file = new SkywardSwordFile;
    file->setRegion((magic == SkywardSwordFile::USMagic ? Region::NTSC : (magic == SkywardSwordFile::JAMagic ? Region::NTSCJ : Region::PAL)));

    for (int i = 0; i < 3; i++)
    {
        SkywardSwordQuest* q = new SkywardSwordQuest(base::readUBytes(0x53C0), 0x53C0);
        atUint64 pos = base::position();
        // seek to the skip data for this particular quest
        base::seek(0xFB60 + (i * 0x24), SeekOrigin::Begin);
        q->setSkipData(base::readUBytes(0x24));
        base::seek(pos, SeekOrigin::Begin);
        file->addQuest(q);
    }

    return file;
}

} // io
} // zelda
