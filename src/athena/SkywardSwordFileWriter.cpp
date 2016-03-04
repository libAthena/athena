#include "athena/SkywardSwordFileWriter.hpp"
#include "athena/SkywardSwordFile.hpp"
#include "athena/SkywardSwordQuest.hpp"

namespace athena
{
namespace io
{

SkywardSwordFileWriter::SkywardSwordFileWriter(atUint8* data, atUint64 len)
    : base(data, len)
{
    base::setEndian(Endian::BigEndian);
}

SkywardSwordFileWriter::SkywardSwordFileWriter(const std::string& filename)
    : base(filename)
{
    base::setEndian(Endian::BigEndian);
}

void SkywardSwordFileWriter::write(SkywardSwordFile* file)
{
    if (!file)
    {
        atError("file cannot be NULL");
        return;
    }

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
        {
            atError("q->data() not 0x53C0 bytes in length");
            return;
        }

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
