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

#ifndef __SSFILEWRITER_HPP__
#define __SSFILEWRITER_HPP__

#include "BinaryWriter.hpp"

namespace Athena
{
class SkywardSwordFile;

namespace io
{

class SkywardSwordFileWriter : public BinaryWriter
{
    // Why does this fuck up my formatting in Qt Creator?
    BINARYWRITER_BASE
public:
    SkywardSwordFileWriter(atUint8* data, atUint64 len);
    SkywardSwordFileWriter(const std::string& filename);

    void write(SkywardSwordFile* file);
};
}
}
#endif // __SSFILEWRITER_HPP__
#endif // ATHENA_NO_SAVES
