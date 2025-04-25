#pragma once

#include <ostream>
#include "athena/Types.hpp"

#include <format>

#ifdef _MSC_VER
#pragma warning(disable : 4996)

#if defined(WINAPI_FAMILY) && WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP
#define WINDOWS_STORE 1
#else
#define WINDOWS_STORE 0
#endif

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
#endif // _MSC_VER

// clang-format off
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
// clang-format on

#if defined(GEKKO) || defined(__SWITCH__) || defined(EMSCRIPTEN)
#include "gekko_support.h"
typedef struct stat atStat64_t;
#define atStat64 stat
#elif _WIN32
typedef struct _stat64 atStat64_t;
#define atStat64 _stat64
#elif __APPLE__ || __FreeBSD__
typedef struct stat atStat64_t;
#define atStat64 stat
#else
typedef struct stat64 atStat64_t;
#define atStat64 stat64
#endif

#ifndef BLOCKSZ
#define BLOCKSZ 512
#endif

#define ROUND_UP_256(val) (((val) + 255) & ~255)
#define ROUND_UP_64(val) (((val) + 63) & ~63)
#define ROUND_UP_32(val) (((val) + 31) & ~31)
#define ROUND_UP_16(val) (((val) + 15) & ~15)
#define ROUND_UP_8(val) (((val) + 7) & ~7)
#define ROUND_UP_4(val) (((val) + 3) & ~3)

#define _XSTR(s) _STR(s)
#define _STR(s) #s

#ifndef ENABLE_BITWISE_ENUM
#define ENABLE_BITWISE_ENUM(type)                                                                                      \
  constexpr type operator|(type a, type b) noexcept {                                                                  \
    using T = std::underlying_type_t<type>;                                                                            \
    return type(static_cast<T>(a) | static_cast<T>(b));                                                                \
  }                                                                                                                    \
  constexpr type operator&(type a, type b) noexcept {                                                                  \
    using T = std::underlying_type_t<type>;                                                                            \
    return type(static_cast<T>(a) & static_cast<T>(b));                                                                \
  }                                                                                                                    \
  constexpr type& operator|=(type& a, type b) noexcept {                                                               \
    using T = std::underlying_type_t<type>;                                                                            \
    a = type(static_cast<T>(a) | static_cast<T>(b));                                                                   \
    return a;                                                                                                          \
  }                                                                                                                    \
  constexpr type& operator&=(type& a, type b) noexcept {                                                               \
    using T = std::underlying_type_t<type>;                                                                            \
    a = type(static_cast<T>(a) & static_cast<T>(b));                                                                   \
    return a;                                                                                                          \
  }                                                                                                                    \
  constexpr type operator~(type key) noexcept {                                                                        \
    using T = std::underlying_type_t<type>;                                                                            \
    return type(~static_cast<T>(key));                                                                                 \
  }                                                                                                                    \
  constexpr bool True(type key) noexcept {                                                                             \
    using T = std::underlying_type_t<type>;                                                                            \
    return static_cast<T>(key) != 0;                                                                                   \
  }                                                                                                                    \
  constexpr bool False(type key) noexcept {                                                                            \
    using T = std::underlying_type_t<type>;                                                                            \
    return static_cast<T>(key) == 0;                                                                                   \
  }
#endif

namespace athena {
namespace error {
enum class Level { Message, Warning, Error, Fatal };
}
enum class SeekOrigin { Begin, Current, End };

enum class Endian { Little, Big };

} // namespace athena

typedef void (*atEXCEPTION_HANDLER)(athena::error::Level level, const char* /*file*/, const char*, int /*line*/,
                                    std::string_view fmt, std::format_args args);

atEXCEPTION_HANDLER atGetExceptionHandler();
/**
 * atSetExceptionHandler is only meant to be used at the start and end of an application's lifetime,
 * this function cannot be considered thread-safe, therefore modifying during runtime is not recommended.
 */
void atSetExceptionHandler(atEXCEPTION_HANDLER func);

std::ostream& operator<<(std::ostream& os, const athena::SeekOrigin& origin);
std::ostream& operator<<(std::ostream& os, const athena::Endian& endian);

#ifndef NDEBUG
#define atDebug(fmt, ...)                                                                                              \
  do {                                                                                                                 \
    atEXCEPTION_HANDLER __handler = atGetExceptionHandler();                                                           \
    if (__handler)                                                                                                     \
      __handler(athena::error::Level::Message, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt,                            \
                std::make_format_args(__VA_ARGS__));                                                                   \
  } while (0)
#else
#define atDebug(...)
#endif

#define atMessage(fmt, ...)                                                                                            \
  do {                                                                                                                 \
    atEXCEPTION_HANDLER __handler = atGetExceptionHandler();                                                           \
    if (__handler)                                                                                                     \
      __handler(athena::error::Level::Message, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt,                            \
                std::make_format_args(__VA_ARGS__));                                                                   \
  } while (0)

#define atWarning(fmt, ...)                                                                                            \
  do {                                                                                                                 \
    atEXCEPTION_HANDLER __handler = atGetExceptionHandler();                                                           \
    if (__handler) {                                                                                                   \
      __handler(athena::error::Level::Warning, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt,                            \
                std::make_format_args(__VA_ARGS__));                                                                   \
    }                                                                                                                  \
  } while (0)

#define atError(fmt, ...)                                                                                              \
  do {                                                                                                                 \
    atEXCEPTION_HANDLER __handler = atGetExceptionHandler();                                                           \
    if (__handler)                                                                                                     \
      __handler(athena::error::Level::Error, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt,                              \
                std::make_format_args(__VA_ARGS__));                                                                   \
  } while (0)

#define atFatal(fmt, ...)                                                                                              \
  do {                                                                                                                 \
    atEXCEPTION_HANDLER __handler = atGetExceptionHandler();                                                           \
    if (__handler)                                                                                                     \
      __handler(athena::error::Level::Fatal, __FILE__, AT_PRETTY_FUNCTION, __LINE__, fmt,                              \
                std::make_format_args(__VA_ARGS__));                                                                   \
  } while (0)
