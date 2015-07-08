#if !defined(ATHENA_NO_SAVES) && !defined(ATHENA_NO_ZQUEST)
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

#ifndef SSFILEREADER_HPP
#define SSFILEREADER_HPP

#include "Athena/MemoryReader.hpp"

namespace Athena
{
class SkywardSwordFile;
namespace io
{
class SkywardSwordFileReader : public MemoryCopyReader
{
    MEMORYCOPYREADER_BASE();
public:

    SkywardSwordFileReader(atUint8* data, atUint64 length);
    SkywardSwordFileReader(const std::string& filename);

    SkywardSwordFile* read();
};
} // io
} // zelda

#endif // SSFILEREADER_HPP
#endif // ATHENA_NO_SAVES
