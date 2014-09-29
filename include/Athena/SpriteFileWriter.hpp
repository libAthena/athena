#ifndef ATHENA_NO_SAKURA
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

#ifndef SSPRITEFILEWRITER_HPP
#define SSPRITEFILEWRITER_HPP

#include "Athena/BinaryWriter.hpp"

namespace Athena
{
namespace Sakura
{
class SpriteFile;
} // Sakura

namespace io
{

class SpriteFileWriter : public BinaryWriter
{
    BINARYWRITER_BASE;
public:
    SpriteFileWriter(atUint8* data, atUint64 length);

    SpriteFileWriter(const std::string& filepath);

    void writeFile(Sakura::SpriteFile* file);
};

} // io
} // zelda
#endif // SSPRITEFILEWRITER_HPP
#endif // ATHENA_NO_SAKURA
