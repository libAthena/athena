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
    ZQuestFile::Game game;
    Uint16 BOM;
    Uint8* data;

    magic = base::readUInt32();

    if (magic != ZQuestFile::Magic)
        throw error::InvalidOperationException("ZQuestFileReader::read -> Not a valid ZQuest file");

    version = base::readUInt32();

    if (version != ZQuestFile::Version)
        throw error::InvalidOperationException("ZQuestFileReader::read -> Unsupported ZQuest version");

    compressedLen = base::readUInt32();
    uncompressedLen = base::readUInt32();
    game = (ZQuestFile::Game)base::readUInt32();
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
            // TODO: Make proper exception
            throw error::InvalidOperationException("ZQuestFileReader::read -> Error decompressing data");
        }

        delete[] data;
        data = dst;
    }

    return new ZQuestFile(game, BOM == 0xFEFF ? BigEndian : LittleEndian, data, uncompressedLen);
}

} // io
} // zelda
