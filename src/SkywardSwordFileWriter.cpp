#include "SkywardSwordFileWriter.hpp"
#include "SkywardSwordFile.hpp"
#include "SkywardSwordQuest.hpp"

namespace zelda
{
namespace io
{

SkywardSwordFileWriter::SkywardSwordFileWriter(Uint8 *data, Uint64 len)
    : base(data, len)
{
    base::setEndianess(BigEndian);
}

SkywardSwordFileWriter::SkywardSwordFileWriter(const std::string &filename)
    : base(filename)
{
    base::setEndianess(BigEndian);
}

void SkywardSwordFileWriter::write(SkywardSwordFile *file)
{
    Uint32 magic = (file->region() == NTSCURegion ? SkywardSwordFile::USMagic :
                   (file->region() == NTSCJRegion ? SkywardSwordFile::JAMagic : SkywardSwordFile::EUMagic));

    base::writeUInt32(magic);
    base::seek(0x1C, base::Beginning);
    base::writeUInt32(0x1D);

    std::vector<SkywardSwordQuest*> quests = file->questList();
    int i = 0;
    for (SkywardSwordQuest* q : quests)
    {
        // Write the save data
        base::writeUBytes(q->data(), q->length());
        Uint64 pos = base::position();
        // Write the slots skip data
        base::seek(0xFB60 + (i * 0x24), base::Beginning);
        base::writeUBytes(q->skipData(), q->skipLength());
        base::seek(pos, base::Beginning);
        i++;
    }

    // write those padding bytes
    base::seek(0xFBE0, base::Beginning);
    save();
}

} // io
} // zelda
