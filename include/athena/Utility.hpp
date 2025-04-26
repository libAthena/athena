#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <cstring>
#include <string>
#include <string_view>
#include <vector>

#include "athena/Global.hpp"
#include "athena/Types.hpp"

namespace athena::utility {
inline bool isEmpty(int8_t* buf, uint32_t size) { return !std::memcmp(buf, buf + 1, size - 1); }
#if _WIN32
constexpr bool isSystemBigEndian() { return false; }
#else
constexpr bool isSystemBigEndian() { return __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__; }
#endif
constexpr ::athena::Endian SystemEndian = isSystemBigEndian() ? ::athena::Endian::Big : ::athena::Endian::Little;
constexpr ::athena::Endian NotSystemEndian = isSystemBigEndian() ? ::athena::Endian::Little : ::athena::Endian::Big;

#if _MSC_VER
#define BSWAP_CONSTEXPR inline
#else
#define BSWAP_CONSTEXPR constexpr
#endif

BSWAP_CONSTEXPR int16_t swap16(int16_t val) {
#if __GNUC__
  return __builtin_bswap16(val);
#elif _MSC_VER
  return _byteswap_ushort(val);
#else
  return (val = (val << 8) | ((val >> 8) & 0xFF));
#endif
}
BSWAP_CONSTEXPR uint16_t swapU16(uint16_t val) { return (uint16_t)swap16(val); }
BSWAP_CONSTEXPR int32_t swap32(int32_t val) {
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
BSWAP_CONSTEXPR uint32_t swapU32(uint32_t val) { return (uint32_t)swap32(val); }
BSWAP_CONSTEXPR int64_t swap64(int64_t val) {
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
BSWAP_CONSTEXPR uint64_t swapU64(uint64_t val) { return (uint64_t)swap64(val); }
BSWAP_CONSTEXPR float swapFloat(float val) {
  union { float f; int32_t i; } uval1 = {val};
  union { int32_t i; float f; } uval2 = {swap32(uval1.i)};
  return uval2.f;
}
BSWAP_CONSTEXPR double swapDouble(double val) {
  union { double f; int64_t i; } uval1 = {val};
  union { int64_t i; double f; } uval2 = {swap64(uval1.i)};
  return uval2.f;
}
BSWAP_CONSTEXPR int16_t LittleInt16(int16_t& val) {
  if constexpr (athena::utility::isSystemBigEndian())
    val = athena::utility::swap16(val);

  return val;
}
BSWAP_CONSTEXPR uint16_t LittleUint16(uint16_t& val) {
  int16_t ret = val;
  LittleInt16(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR int16_t BigInt16(int16_t& val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    val = athena::utility::swap16(val);

  return val;
}
BSWAP_CONSTEXPR uint16_t BigUint16(uint16_t& val) {
  int16_t ret = val;
  BigInt16(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR int32_t LittleInt32(int32_t& val) {
  if constexpr (athena::utility::isSystemBigEndian())
    val = athena::utility::swap32(val);

  return val;
}
BSWAP_CONSTEXPR uint32_t LittleUint32(uint32_t& val) {
  int32_t ret = val;
  LittleInt32(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR int32_t BigInt32(int32_t& val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    val = athena::utility::swap32(val);

  return val;
}
BSWAP_CONSTEXPR uint32_t BigUint32(uint32_t& val) {
  int32_t ret = val;
  BigInt32(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR int64_t LittleInt64(int64_t& val) {
  if constexpr (athena::utility::isSystemBigEndian())
    val = athena::utility::swap64(val);

  return val;
}
BSWAP_CONSTEXPR uint64_t LittleUint64(uint64_t& val) {
  int64_t ret = val;
  LittleInt64(ret);
  val = ret;

  return val;
}
BSWAP_CONSTEXPR int64_t BigInt64(int64_t& val) {
  if constexpr (!athena::utility::isSystemBigEndian())
    val = athena::utility::swap64(val);

  return val;
}
BSWAP_CONSTEXPR uint64_t BigUint64(uint64_t& val) {
  int64_t ret = val;
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

void fillRandom(uint8_t* rndArea, uint64_t count);
std::vector<std::string> split(std::string_view s, char delim);
uint64_t rand64();
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
uint64_t fileSize(std::string_view filename);
#ifdef _WIN32
uint64_t fileSize(std::wstring_view filename);
#endif

std::string wideToUtf8(std::wstring_view src);

std::wstring utf8ToWide(std::string_view src);

} // namespace athena::utility
#endif
