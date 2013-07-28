#include "SSFileWriter.hpp"
#include "SSFile.hpp"
#include "SSQuest.hpp"

namespace zelda
{
namespace io
{

SSFileWriter::SSFileWriter(Uint8 *data, Uint64 len)
    : base(data, len)
{
    base::setEndianess(BigEndian);
}

SSFileWriter::SSFileWriter(const std::string &filename)
    : base(filename)
{
    base::setEndianess(BigEndian);
}

void SSFileWriter::write(SSFile *file)
{
    Uint32 magic = (file->region() == NTSCURegion ? SSFile::USMagic :
                   (file->region() == NTSCJRegion ? SSFile::JAMagic : SSFile::EUMagic));

    base::writeUInt32(magic);
    base::seek(0x1C, base::Beginning);
    base::writeUInt32(0x1D);

    std::vector<SSQuest*> quests = file->questList();
    int i = 0;
    for (SSQuest* q : quests)
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
