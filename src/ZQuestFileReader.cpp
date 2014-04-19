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

#include "ZQuestFileReader.hpp"
#include "ZQuestFile.hpp"
#include "InvalidOperationException.hpp"
#include "Compression.hpp"
#include "InvalidDataException.hpp"
#include "Checksums.hpp"
#include <iostream>
#include <iomanip>
#include <utility.hpp>

namespace zelda
{
namespace io
{

ZQuestFileReader::ZQuestFileReader(Uint8 *data, Uint64 length)
    : base(data, length)
{
}

ZQuestFileReader::ZQuestFileReader(const std::string &filename)
    : base(filename)
{
}

ZQuestFile *ZQuestFileReader::read()
{
    Uint32 magic, version, compressedLen, uncompressedLen;
    ZQuestFile::Game game = ZQuestFile::NoGame;
    std::string gameString;
    Uint16 BOM;
    Uint32 checksum;
    Uint8* data;

    magic = base::readUInt32();

    if ((magic & 0x00FFFFFF) != (ZQuestFile::Magic & 0x00FFFFFF))
        THROW_INVALID_DATA("Not a valid ZQuest file");

    version = base::readUInt32();

    if (version > ZQuestFile::Version)
        THROW_INVALID_DATA("Unsupported ZQuest version");

    compressedLen = base::readUInt32();
    uncompressedLen = base::readUInt32();

    if (version >= ZQUEST_VERSION_CHECK(2, 0, 0))
    {
        gameString = ((const char*)base::readBytes(0x0A), 0x0A);
        for (size_t i = 0; i <  ZQuestFile::gameStringList().size(); i++)
        {
            if (!ZQuestFile::gameStringList().at(i).substr(0, 0x0A).compare(gameString))
            {
                gameString = ZQuestFile::gameStringList().at(i);
                break;
            }

        }
        BOM = base::readUInt16();
        checksum = base::readUInt32();
    }
    else
    {
        game = (ZQuestFile::Game)base::readUInt32();
        BOM = base::readUInt16();
        std::cerr << "Test" << std::endl;
        base::seek(0x0A);
    }

    data = (Uint8*)base::readBytes(compressedLen); // compressedLen is always the total file size

    if (version >= ZQUEST_VERSION_CHECK(2, 0, 0))
    {
        if (checksum != zelda::Checksums::crc32(data, compressedLen))
        {
            delete[] data;
            THROW_INVALID_DATA("Checksum mismatch, data corrupt");
        }
    }
    else
    {
        std::clog << "ZQuest version 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << zelda::utility::swapU32(version);
        std::clog << " has no checksum field" << std::endl;
    }

    if (compressedLen != uncompressedLen)
    {
        Uint8* dst = new Uint8[uncompressedLen];
        Uint32 dstLen = io::Compression::decompressZlib(data, compressedLen, dst, uncompressedLen);

        if (dstLen != uncompressedLen)
        {
            delete[] dst;
            delete[] data;
            // TODO: Make proper exception
            THROW_INVALID_DATA("Error decompressing data");
        }

        delete[] data;
        data = dst;
    }

    return new ZQuestFile(game, BOM == 0xFEFF ? BigEndian : LittleEndian, data, uncompressedLen, gameString);
}

} // io
} // zelda
