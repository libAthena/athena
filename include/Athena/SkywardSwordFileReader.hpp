#ifndef ATHENA_NO_SAVES
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

#ifndef __SSFILEREADER_HPP__
#define __SSFILEREADER_HPP__

#include "BinaryReader.hpp"

namespace Athena
{
class SkywardSwordFile;
namespace io
{
class SkywardSwordFileReader : public BinaryReader
{
    BINARYREADER_BASE();
public:

    SkywardSwordFileReader(atUint8* data, atUint64 length);
    SkywardSwordFileReader(const std::string& filename);

    SkywardSwordFile* read();
};
} // io
} // zelda

#endif // __SSFILEREADER_HPP__
#endif // ATHENA_NO_SAVES
