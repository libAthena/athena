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

#ifndef __PRETTY_FUNCTION__
#   ifdef __FUNCSIG__
#       define __PRETTY_FUNCTION__ __FUNCSIG__
#   elif defined(__FUNCTION__)
#       define __PRETTY_FUNCTION__ __FUNCTION__
#   elif defined(__FUNC__)
#       define __PRETTY_FUNCTION__ __FUNCTION__
#   elif defined(__func__)
#       define __PRETTY_FUNCTION__ __FUNCTION__
#   else
#       define __PRETTY_FUNCTION__ "<unkown>"
#   endif
#endif

#ifndef aDebug
#define aDebug() \
    std::cout << __FILE__ << "(" << __LINE__ << ") " << __PRETTY_FUNCTION__ << ": "
#endif
#ifndef aError
#define aError() \
    std::cerr << __FILE__ << "(" << __LINE__ << ") " << __PRETTY_FUNCTION__ << ": "
#endif

#ifndef aPrint
#define aPrint() std::cout
#endif

#define aEnd() '\n'

#ifndef BLOCKSZ
#define BLOCKSZ 512
#endif

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
} // Athena

std::ostream& operator<<(std::ostream& os, const Athena::SeekOrigin& origin);
std::ostream& operator<<(std::ostream& os, const Athena::Endian& endian);
#endif // GLOBAL_HPP
