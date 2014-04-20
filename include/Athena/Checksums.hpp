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

#ifndef CHECKSUMS_HPP
#define CHECKSUMS_HPP

#include "Athena/Global.hpp"

namespace Athena
{
namespace Checksums
{
Uint32 crc32(const Uint8* data, Uint64 length, Uint32 seed = 0xFFFFFFFF);
Uint16 crc16CCITT(const Uint8* data, Uint64 length, Uint16 seed = 0xFFFF, Uint16 final = 0);
Uint16 crc16(const Uint8* data, Uint64 length);
}
}
#endif // CHECKSUMS_HPP
