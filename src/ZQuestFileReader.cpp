#include "ZQuestFileReader.hpp"
#include "ZQuest.hpp"
#include "IOException.hpp"
#include "Compression.hpp"

namespace zelda
{

ZQuestFileReader::ZQuestFileReader(Uint8 *data, Uint64 length)
    : base(data, length)
{
}

ZQuestFileReader::ZQuestFileReader(const std::string &filename)
    : base(filename)
{
}

ZQuest *ZQuestFileReader::read()
{
    Uint32 magic, version, compressedLen, uncompressedLen;
    ZQuest::Game game;
    Uint16 BOM;
    Uint8* data;

    magic = base::readUInt32();

    if (magic != ZQuest::Magic)
        throw IOException("ZQuestFileReader::read -> Not a valid ZQuest file");

    version = base::readUInt32();

    if (version != ZQuest::Version)
        throw IOException("ZQuestFileReader::read -> Unsupported ZQuest version");

    compressedLen = base::readUInt32();
    uncompressedLen = base::readUInt32();
    game = (ZQuest::Game)base::readUInt32();
    BOM = base::readUInt16();
    base::seek(0x0A);
    data = (Uint8*)base::readBytes(compressedLen); // compressedLen is always the total file size

    if (compressedLen != uncompressedLen)
    {
        Uint8* dst = new Uint8[uncompressedLen];
        Uint32 dstLen = io::Compression::decompressZlib(data, compressedLen, dst, uncompressedLen);

        if (dstLen != uncompressedLen)
        {
            delete[] dst;
            delete[] data;
            throw IOException("ZQuestFileReader::read -> Error decompressing data");
        }

        delete[] data;
        data = dst;
    }

    return new ZQuest(game, BOM == 0xFEFF ? BigEndian : LittleEndian, data, uncompressedLen);
}

} // zelda
