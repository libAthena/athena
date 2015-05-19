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


#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string>
#include <vector>
#include <stdarg.h>
#include <string.h>
#include "Athena/Global.hpp"
#include "Athena/Types.hpp"

namespace Athena
{
namespace utility
{
inline bool isEmpty(atInt8* buf, atUint32 size) {return !memcmp(buf, buf + 1, size - 1);}
bool isSystemBigEndian();

inline atInt16  swap16(atInt16 val)
{
#if __GNUC__
    return __builtin_bswap16(val);
#elif _WIN32
    return _byteswap_ushort(val);
#else
    return (val = (val << 8) | ((val >> 8) & 0xFF));
#endif
}
inline atUint16 swapU16(atUint16 val) {return (atUint16)swap16(val);}
inline atInt32  swap32(atInt32 val)
{
#if __GNUC__
    return __builtin_bswap32(val);
#elif _WIN32
    return _byteswap_ulong(val);
#else
    val = (val & 0x0000FFFF) << 16 | (val & 0xFFFF0000) >> 16;
    val = (val & 0x00FF00FF) << 8 | (val & 0xFF00FF00) >> 8;
    return val;
#endif
}
inline atUint32 swapU32(atUint32 val) {return (atUint32)swap32(val);}
inline atInt64  swap64(atInt64 val)
{
#if __GNUC__
    return __builtin_bswap64(val);
#elif _WIN32
    return _byteswap_uint64(val);
#else
    return (val = ((atInt64)((((atInt64)(val) & 0xFF00000000000000ULL) >> 56) |
                             (((atInt64)(val) & 0x00FF000000000000ULL) >> 40) |
                             (((atInt64)(val) & 0x0000FF0000000000ULL) >> 24) |
                             (((atInt64)(val) & 0x000000FF00000000ULL) >>  8) |
                             (((atInt64)(val) & 0x00000000FF000000ULL) <<  8) |
                             (((atInt64)(val) & 0x0000000000FF0000ULL) << 24) |
                             (((atInt64)(val) & 0x000000000000FF00ULL) << 40) |
                             (((atInt64)(val) & 0x00000000000000FFULL) << 56))));
#endif
}
inline atUint64 swapU64(atUint64 val) {return (atUint64)swap64(val);}
inline float swapFloat(float val)
{
    atInt32 ival = swap32(*((atInt32*)(&val)));
    return *((float*)(&ival));
}
inline double  swapDouble(double val)
{
    atInt64 ival = swap64(*((atInt64*)(&val)));
    return *((double*)(&ival));
}
inline atInt16 LittleInt16(atInt16& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap16(val);

    return val;
}
inline atUint16 LittleUint16(atUint16& val)
{
    atInt16 ret = val;
    LittleInt16(ret);
    val = ret;

    return val;
}
inline atInt16 BigInt16(atInt16& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap16(val);

    return val;
}
inline atUint16 BigUint16(atUint16& val)
{
    atInt16 ret = val;
    BigInt16(ret);
    val = ret;

    return val;
}
inline atInt32 LittleInt32(atInt32& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap32(val);

    return val;
}
inline atUint32 LittleUint32(atUint32& val)
{
    atInt32 ret = val;
    LittleInt32(ret);
    val = ret;

    return val;
}
inline atInt32 BigInt32(atInt32& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap32(val);

    return val;
}
inline atUint32 BigUint32(atUint32& val)
{
    atInt32 ret = val;
    BigInt32(ret);
    val = ret;

    return val;
}
inline atInt64 LittleInt64(atInt64& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap64(val);

    return val;
}
inline atUint64 LittleUint64(atUint64& val)
{
    atInt64 ret = val;
    LittleInt64(ret);
    val = ret;

    return val;
}
inline atInt64 BigInt64(atInt64& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap64(val);

    return val;
}
inline atUint64 BigUint64(atUint64& val)
{
    atInt64 ret = val;
    BigInt64(ret);
    val = ret;

    return val;
}

inline float LittleFloat(float& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapFloat(val);

    return val;
}
inline float BigFloat(float& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapFloat(val);

    return val;
}
inline double LittleDouble(double& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapDouble(val);

    return val;
}
inline double BigDouble(double& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapDouble(val);

    return val;
}

void fillRandom(atUint8* rndArea, atUint64 count);
std::vector<std::string> split(const std::string& s, char delim);
std::string join(const std::vector<std::string>& elems, const std::string& delims);
void tolower(std::string& str);
void toupper(std::string& str);
std::string vsprintf(const char* fmt, va_list list);
std::string sprintf(const char* fmt, ...);
bool parseBool(const std::string& boolean, bool* valid = NULL);

int countChar(const std::string& str, const char chr, int* lastOccur = NULL);

// trim from start
std::string& ltrim(std::string& s);

// trim from end
std::string& rtrim(std::string& s);

// trim from both ends
std::string& trim(std::string& s);
atUint64 fileSize(const std::string& filename);
} // utility
} // Athena
#endif
