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

#ifndef SSPRITEFILEREADER_HPP
#define SSPRITEFILEREADER_HPP

#include "Athena/BinaryReader.hpp"

namespace Athena
{
namespace Sakura
{
class SpriteFile;
} // Sakura

namespace io
{

class SpriteFileReader : public BinaryReader
{
    BINARYREADER_BASE();
public:
    SpriteFileReader(atUint8* data, atUint64 length);
    SpriteFileReader(const std::string& filepath);

    Sakura::SpriteFile* readFile();
};
} // io
} // zelda


#endif // SSPRITEFILEREADER_HPP
