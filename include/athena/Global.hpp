#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <iostream>
#include "athena/Types.hpp"
#include "athena/Utility.hpp"

#ifdef _MSC_VER
#pragma warning(disable : 4996)

#include <sys/stat.h>

#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#if !defined(S_ISLNK)
#define S_ISLNK(m) 0
#endif

#define PRISize "Iu"

#else

#define PRISize "zu"

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
#elif _WIN32
typedef struct _stat64 stat64_t;
#elif __FreeBSD__
typedef struct stat stat64_t;
#define stat64 stat
#define fseeko64 fseeko
#define ftello64 ftello
#else
typedef struct stat64 stat64_t;
#endif

#ifndef BLOCKSZ
#define BLOCKSZ 512
#endif

#define ROUND_UP_32(val) (((val) + 31) & ~31)
#define ROUND_UP_16(val) (((val) + 15) & ~15)
#define ROUND_UP_4(val) (((val) + 3) & ~3)

#ifndef ENABLE_BITWISE_ENUM
#define ENABLE_BITWISE_ENUM(type)\
inline type operator|(type a, type b)\
{\
    using T = std::underlying_type_t<type>;\
    return type(static_cast<T>(a) | static_cast<T>(b));\
}\
inline type operator&(type a, type b)\
{\
    using T = std::underlying_type_t<type>;\
    return type(static_cast<T>(a) & static_cast<T>(b));\
}\
inline type& operator|=(type& a, const type& b)\
{\
    using T = std::underlying_type_t<type>;\
    a = type(static_cast<T>(a) | static_cast<T>(b));\
    return a;\
}\
inline type& operator&=(type& a, const type& b)\
{\
    using T = std::underlying_type_t<type>;\
    a = type(static_cast<T>(a) & static_cast<T>(b));\
    return a;\
}\
inline type operator~(const type& key)\
{\
    using T = std::underlying_type_t<type>;\
    return type(~static_cast<T>(key));\
}
#endif

namespace athena
{
namespace error
{
enum class Level
{
    Message,
    Warning,
    Error,
    Fatal
};
}
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
} // Athena

typedef void (*atEXCEPTION_HANDLER)(athena::error::Level level, const char* file, const char* function, int line, const char* fmt, ...);

atEXCEPTION_HANDLER atGetExceptionHandler();
/**
 * atSetExceptionHandler is only meant to be used a the start and end of an application's lifetime,
 * this function cannot be considered thread-safe, therefore modifying during runtime is not recommended.
 */
void atSetExceptionHandler(atEXCEPTION_HANDLER func);

std::ostream& operator<<(std::ostream& os, const athena::SeekOrigin& origin);
std::ostream& operator<<(std::ostream& os, const athena::Endian& endian);

#ifdef _MSC_VER
#ifndef NDEBUG
#define atDebug(fmt, ...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Message, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)
#else
#define atDebug(fmt, ...)
#endif

#define atMessage(fmt, ...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Message, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

#define atWarning(fmt, ...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Warning, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

#define atError(fmt, ...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Error, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

#define atFatal(fmt, ...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Fatal, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)
#elif defined(__GNUC__)

#ifndef NDEBUG
#define atDebug(fmt...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Message, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt); \
} while(0)
#else // _MSC_VER
#define atDebug(fmt, ...)
#endif // NDEBUG

#define atMessage(fmt...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Message, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt); \
} while(0)

#define atWarning(fmt...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Warning, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt); \
} while(0)

#define atError(fmt...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Error, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt); \
} while(0)

#define atFatal(fmt...) \
    do { atEXCEPTION_HANDLER __handler = atGetExceptionHandler(); \
    if (__handler) \
        __handler(athena::error::Level::Fatal, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt); \
} while(0)
#endif // defined(__GNUC__)

#endif // GLOBAL_HPP
