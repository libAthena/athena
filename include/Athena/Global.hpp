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

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "Athena/Types.hpp"
#include "Athena/Utility.hpp"
#include <iostream>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#ifndef AT_PRETTY_FUNCTION
#   ifdef __PRETTY_FUNCTION__
#       define AT_PRETTY_FUNCTION __PRETTY_FUNCTION__
#   elif defined(__FUNCSIG__)
#       define AT_PRETTY_FUNCTION __FUNCSIG__
#   elif defined(__FUNCTION__)
#       define AT_PRETTY_FUNCTION __FUNCTION__
#   elif defined(__FUNC__)
#       define AT_PRETTY_FUNCTION __FUNC__
#   elif defined(__func__)
#       define AT_PRETTY_FUNCTION __func__
#   else
#       define AT_PRETTY_FUNCTION "<unknown>"
#   endif
#endif

#ifndef aDebug
#define aDebug() \
    std::cout << __FILE__ << "(" << __LINE__ << ") " << AT_PRETTY_FUNCTION << ": "
#endif
#ifndef aError
#define aError() \
    std::cerr << __FILE__ << "(" << __LINE__ << ") " << AT_PRETTY_FUNCTION << ": "
#endif

#ifndef aPrint
#define aPrint() std::cout
#endif

#define aEnd() '\n'

#ifndef BLOCKSZ
#define BLOCKSZ 512
#endif

#define ROUND_UP_32(val) (((val) + 31) & ~31)
#define ROUND_UP_16(val) (((val) + 15) & ~15)

namespace Athena
{
enum class SeekOrigin
{
    Begin,
    Current,
    End
};

enum class Endian
{
    LittleEndian,
    BigEndian
};

#ifndef ATHENA_NO_SAKURA
namespace Sakura
{

template <typename T>
class Vector2D
{
public:
    T x;
    T y;

    Vector2D()
        : x(0),
          y(0)
    {
    }

    Vector2D(T x, T y)
        : x(x),
          y(y)
    {
    }
};

typedef Vector2D<int> Vector2Di;
typedef Vector2D<float> Vector2Df;
} // Sakura
#endif // ATHENA_NO_SAKURA
} // Athena

std::ostream& operator<<(std::ostream& os, const Athena::SeekOrigin& origin);
std::ostream& operator<<(std::ostream& os, const Athena::Endian& endian);
#endif // GLOBAL_HPP
