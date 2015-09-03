#include "Athena/ZQuestFileReader.hpp"
#include "Athena/ZQuestFile.hpp"
#include "Athena/Compression.hpp"
#include "Athena/Checksums.hpp"
#include "Athena/Utility.hpp"

#include <iostream>
#include <iomanip>

namespace Athena
{
namespace io
{

ZQuestFileReader::ZQuestFileReader(atUint8* data, atUint64 length)
    : base(data, length)
{
}

ZQuestFileReader::ZQuestFileReader(const std::string& filename)
    : base(filename)
{
}

ZQuestFile* ZQuestFileReader::read()
{
    atUint32 magic, version, compressedLen, uncompressedLen;
    ZQuestFile::Game game = ZQuestFile::NoGame;
    std::string gameString;
    atUint16 BOM;
    atUint32 checksum = 0;

    magic = base::readUint32();

    if ((magic & 0x00FFFFFF) != (ZQuestFile::Magic & 0x00FFFFFF))
    {
        atError("Not a valid ZQuest file");
        return nullptr;
    }

    version = base::readUint32();

    if (version > ZQuestFile::Version)
    {
        atError("Unsupported ZQuest version");
        return nullptr;
    }

    compressedLen = base::readUint32();
    uncompressedLen = base::readUint32();

    if (version >= ZQUEST_VERSION_CHECK(2, 0, 0))
    {
        gameString = std::string((const char*)base::readBytes(0x0A).get(), 0x0A);

        for (size_t i = 0; i <  ZQuestFile::gameStringList().size(); i++)
        {
            if (!ZQuestFile::gameStringList().at(i).substr(0, 0x0A).compare(gameString))
            {
                gameString = ZQuestFile::gameStringList().at(i);
                game = (ZQuestFile::Game)i;
                break;
            }
        }

        BOM = base::readUint16();
        checksum = base::readUint32();
    }
    else
    {
        game = (ZQuestFile::Game)base::readUint32();
        BOM = base::readUint16();
        base::seek(0x0A);
    }

    std::unique_ptr<atUint8[]> data = base::readUBytes(compressedLen); // compressedLen is always the total file size

    if (version >= ZQUEST_VERSION_CHECK(2, 0, 0))
    {
        if (checksum != Athena::Checksums::crc32(data.get(), compressedLen))
        {
            atError("Checksum mismatch, data corrupt");
            return nullptr;
        }
    }
    else
    {
        std::clog << "ZQuest version 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << Athena::utility::swapU32(version);
        std::clog << " has no checksum field" << std::endl;
    }

    if (compressedLen != uncompressedLen)
    {
        atUint8* dst = new atUint8[uncompressedLen];
        atUint32 dstLen = io::Compression::decompressZlib(data.get(), compressedLen, dst, uncompressedLen);

        if (dstLen != uncompressedLen)
        {
            delete[] dst;
            atError("Error decompressing data");
            return nullptr;
        }

        data.reset(dst);
    }

    return new ZQuestFile(game, BOM == 0xFEFF ? Endian::BigEndian : Endian::LittleEndian, std::move(data), uncompressedLen, gameString);
}

} // io
} // zelda
