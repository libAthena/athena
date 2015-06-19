#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "Athena/Types.hpp"
#include "Athena/Utility.hpp"
#include <iostream>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#ifndef AT_PRETTY_FUNCTION
#   if defined(__PRETTY_FUNCTION__) || defined(__GNUC__)
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


#ifdef GEKKO
#include "gekko_support.h"
typedef struct stat stat64_t;
#define stat64 stat
#else
typedef struct stat64 stat64_t;
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
enum SeekOrigin
{
    Begin,
    Current,
    End
};

enum Endian
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

typedef void (*atEXCEPTION_HANDLER)(const std::string& file, const std::string& function, int line, const std::string&, ...);

atEXCEPTION_HANDLER atGetExceptionHandler();
void atSetExceptionHandler(atEXCEPTION_HANDLER func);

std::ostream& operator<<(std::ostream& os, const Athena::SeekOrigin& origin);
std::ostream& operator<<(std::ostream& os, const Athena::Endian& endian);
#endif // GLOBAL_HPP
