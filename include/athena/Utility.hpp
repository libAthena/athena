#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string>
#include <string_view>
#include <vector>
#include <cstdarg>
#include <cstring>
#include "athena/Global.hpp"
#include "athena/Types.hpp"

namespace athena::utility {
inline bool isEmpty(atInt8* buf, atUint32 size) { return !std::memcmp(buf, buf + 1, size - 1); }
#if _WIN32
constexpr bool isSystemBigEndian() { return false; }
#else
constexpr bool isSystemBigEndian() { return __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__; }
#endif
constexpr ::athena::Endian SystemEndian = isSystemBigEndian() ? Big : Little;
constexpr ::athena::Endian NotSystemEndian = isSystemBigEndian() ? Little : Big;

#if _MSC_VER
#define BSWAP_CONSTEXPR inline
#else
#define BSWAP_CONSTEXPR constexpr
#endif

BSWAP_CONSTEXPR atInt16 swap16(atInt16 val) {
#if __GNUC__
  return __builtin_bswap16(val);
#elif _MSC_VER
  return _byteswap_ushort(val);
#else
  return (val = (val << 8) | ((val >> 8) & 0xFF));
#endif
}
BSWAP_CONSTEXPR atUint16 swapU16(atUint16 val) { return (atUint16)swap16(val); }
BSWAP_CONSTEXPR atInt32 swap32(atInt32 val) {
#if __GNUC__
  return __builtin_bswap32(val);
#elif _MSC_VER
  return _byteswap_ulong(val);
#else
  val = (val & 0x0000FFFF) << 16 | (val & 0xFFFF0000) >> 16;
  val = (val & 0x00FF00FF) << 8 | (val & 0xFF00FF00) >> 8;
  return val;
#endif
}
BSWAP_CONSTEXPR atUint32 swapU32(atUint32 val) { return (atUint32)swap32(val); }
BSWAP_CONSTEXPR atInt64 swap64(atInt64 val) {
#if __GNUC__
  return __builtin_bswap64(val);
#elif _MSC_VER
  return _byteswap_uint64(val);
#else
  return (val = ((atInt64)(
              (((atInt64)(val)&0xFF00000000000000ULL) >> 56) | (((atInt64)(val)&0x00FF000000000000ULL) >> 40) |
              (((atInt64)(val)&0x0000FF0000000000ULL) >> 24) | (((atInt64)(val)&0x000000FF00000000ULL) >> 8) |
              (((atInt64)(val)&0x00000000FF000000ULL) << 8) | (((atInt64)(val)&0x0000000000FF0000ULL) << 24) |
              (((atInt64)(val)&0x000000000000FF00ULL) << 40) | (((atInt64)(val)&0x00000000000000FFULL) << 56))));
#endif
}
BSWAP_CONSTEXPR atUint64 swapU64(atUint64 val) { return (atUint64)swap64(val); }
BSWAP_CONSTEXPR float swapFloat(float val) {
  union { float f; atInt32 i; } uval1 = {val};
  union { atInt32 i; float f; } uval2 = {swap32(uval1.i)};
  return uval2.f;
}
BSWAP_CONSTEXPR double swapDouble(double val) {
  union { double f; atInt64 i; } uval1 = {val};
  union { atInt64 i; double f; } uval2 = {swap64(uval1.i)};
  return uval2.f;
}
BSWAP_CONSTEXPR atInt16 LittleInt16(atInt16& val) {
  if constexpr (athena::utility::isSystemBigEndian())
    val = athena::utility::swap16(val);

  return val;
}
BSWAP_CONSTEXPR atUint16 LittleUint16(atUint16& val) {
  atInt16 ret = val;
  LittleInt16(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR atInt16 BigInt16(atInt16& val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    val = athena::utility::swap16(val);

  return val;
}
BSWAP_CONSTEXPR atUint16 BigUint16(atUint16& val) {
  atInt16 ret = val;
  BigInt16(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR atInt32 LittleInt32(atInt32& val) {
  if constexpr (athena::utility::isSystemBigEndian())
    val = athena::utility::swap32(val);

  return val;
}
BSWAP_CONSTEXPR atUint32 LittleUint32(atUint32& val) {
  atInt32 ret = val;
  LittleInt32(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR atInt32 BigInt32(atInt32& val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    val = athena::utility::swap32(val);

  return val;
}
BSWAP_CONSTEXPR atUint32 BigUint32(atUint32& val) {
  atInt32 ret = val;
  BigInt32(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR atInt64 LittleInt64(atInt64& val) {
  if constexpr (athena::utility::isSystemBigEndian())
    val = athena::utility::swap64(val);

  return val;
}
BSWAP_CONSTEXPR atUint64 LittleUint64(atUint64& val) {
  atInt64 ret = val;
  LittleInt64(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR atInt64 BigInt64(atInt64& val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    val = athena::utility::swap64(val);

  return val;
}
BSWAP_CONSTEXPR atUint64 BigUint64(atUint64& val) {
  atInt64 ret = val;
  BigInt64(ret);
  val = ret;

  return val;
}

BSWAP_CONSTEXPR float LittleFloat(float val) {
  if constexpr (athena::utility::isSystemBigEndian())
    return athena::utility::swapFloat(val);

  return val;
}
BSWAP_CONSTEXPR float BigFloat(float val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    return athena::utility::swapFloat(val);

  return val;
}
BSWAP_CONSTEXPR double LittleDouble(double val) {
  if constexpr (athena::utility::isSystemBigEndian())
    return athena::utility::swapDouble(val);

  return val;
}
BSWAP_CONSTEXPR double BigDouble(double val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    return athena::utility::swapDouble(val);

  return val;
}

void fillRandom(atUint8* rndArea, atUint64 count);
std::vector<std::string> split(std::string_view s, char delim);
atUint64 rand64();
std::string join(const std::vector<std::string>& elems, std::string_view delims);
void tolower(std::string& str);
void toupper(std::string& str);
bool parseBool(std::string_view boolean, bool* valid = nullptr);

int countChar(std::string_view str, char chr, int* lastOccur = nullptr);

// trim from start
std::string& ltrim(std::string& s);

// trim from end
std::string& rtrim(std::string& s);

// trim from both ends
std::string& trim(std::string& s);
atUint64 fileSize(std::string_view filename);
#ifdef _WIN32
atUint64 fileSize(std::wstring_view filename);
#endif

std::string wideToUtf8(std::wstring_view src);

std::wstring utf8ToWide(std::string_view src);

} // namespace athena::utility
#endif
