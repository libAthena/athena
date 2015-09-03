#ifndef SAKURAGLOBAL_HPP
#define SAKURAGLOBAL_HPP

#include "Athena/Global.hpp"

namespace Athena
{
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

#endif // SAKURAGLOBAL_HPP
