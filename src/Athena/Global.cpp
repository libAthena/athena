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

#include "Athena/Global.hpp"

std::ostream& operator<<(std::ostream& os, const Athena::SeekOrigin& origin)
{
    switch(origin)
    {
        case Athena::SeekOrigin::Begin:
            os << "Begin";
            break;
        case Athena::SeekOrigin::Current:
            os << "Current";
            break;
        case Athena::SeekOrigin::End:
            os << "End";
            break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const Athena::Endian& endian)
{
    switch(endian)
    {
        case Athena::Endian::LittleEndian:
            os << "LittleEndian";
            break;
        case Athena::Endian::BigEndian:
            os << "BigEndian";
            break;
    }
    return os;
}


static atEXCEPTION_HANDLER g_atExceptionHandler = nullptr;

atEXCEPTION_HANDLER atGetExceptionHandler()
{
    return g_atExceptionHandler;
}


void atSetExceptionHandler(atEXCEPTION_HANDLER func)
{
    g_atExceptionHandler = func;
}
