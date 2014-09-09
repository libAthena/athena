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

#include "Athena/Utility.hpp"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <cstdarg>
#include <iterator>
#include <cstdio>

namespace Athena
{
namespace utility
{
bool isEmpty(atInt8* buf, atUint32 size)
{
    return !memcmp(buf, buf + 1, size - 1);
}

atUint16 swapU16(atUint16 val )
{
    return (atUint16)swap16(val);
}

atInt16 swap16(atInt16 val )
{
    return (val = (val << 8) | ((val >> 8) & 0xFF));
}

atUint32 swapU32(atUint32 val)
{
    return (atUint32)swap32(val);
}

int swap32(atInt32 val )
{
    val = (val & 0x0000FFFF) << 16 | (val & 0xFFFF0000) >> 16;
    val = (val & 0x00FF00FF) << 8 | (val & 0xFF00FF00) >> 8;
    return val;
}

atUint64 swapU64(atUint64 val)
{
    return (atUint64)swap64(val);
}

atInt64 swap64(atInt64 val)
{
    return (val = ((atInt64)((((atInt64)(val) & 0xFF00000000000000ULL) >> 56) |
                    (((atInt64)(val) & 0x00FF000000000000ULL) >> 40) |
                    (((atInt64)(val) & 0x0000FF0000000000ULL) >> 24) |
                    (((atInt64)(val) & 0x000000FF00000000ULL) >>  8) |
                    (((atInt64)(val) & 0x00000000FF000000ULL) <<  8) |
                    (((atInt64)(val) & 0x0000000000FF0000ULL) << 24) |
                    (((atInt64)(val) & 0x000000000000FF00ULL) << 40) |
                    (((atInt64)(val) & 0x00000000000000FFULL) << 56))));
}

bool isSystemBigEndian()
{
    static atUint8* test = (atUint8*)"\xFE\xFF";
    return (*(atUint16*)test == 0xFEFF);
}


atInt16 LittleInt16(atInt16& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap16(val);

    return val;
}

atUint16 LittleUint16(atUint16& val)
{
    atInt16 ret = val;
    LittleInt16(ret);
    val = ret;

    return val;
}

atInt16 BigInt16(atInt16& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap16(val);

    return val;
}

atUint16 BigUint16(atUint16& val)
{
    atInt16 ret = val;
    BigInt16(ret);
    val = ret;

    return val;
}

atInt32 LittleInt32(atInt32& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap32(val);

    return val;
}

atUint32 LittleUint32(atUint32& val)
{
    atInt32 ret = val;
    LittleInt32(ret);
    val = ret;

    return val;
}


atInt32 BigInt32(atInt32& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap32(val);

    return val;
}

atUint32 BigUint32(atUint32& val)
{
    atInt32 ret = val;
    BigInt32(ret);
    val = ret;

    return val;
}

atInt64 LittleInt64(atInt64& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap64(val);

    return val;
}

atUint64 LittleUint64(atUint64& val)
{
    atInt64 ret = val;
    LittleInt64(ret);
    val = ret;

    return val;
}

atInt64 BigInt64(atInt64& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swap64(val);

    return val;
}

atUint64 BigUint64(atUint64& val)
{
    atInt64 ret = val;
    BigInt64(ret);
    val = ret;

    return val;
}

float LittleFloat(float& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapFloat(val);

    return val;
}

float BigFloat(float& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapFloat(val);

    return val;
}

double LittleDouble(double& val)
{
    if (Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapDouble(val);

    return val;
}

double BigDouble(double& val)
{
    if (!Athena::utility::isSystemBigEndian())
        val = Athena::utility::swapDouble(val);

    return val;
}


void fillRandom(atUint8 * rndArea, atUint64 count)
{
    for(atUint64 i = 0; i < count; i++)
        rndArea[i]=rand();
}

float swapFloat(float val)
{
    float retVal;
    char* convFloat = (char*) &val;
    char* retFloat = (char*) &retVal;

    retFloat[0] = convFloat[3];
    retFloat[1] = convFloat[2];
    retFloat[2] = convFloat[1];
    retFloat[3] = convFloat[0];

    return retVal;
}

double swapDouble(double val)
{
    double retVal;
    char* convFloat = (char*) &val;
    char* retFloat = (char*) &retVal;

    retFloat[0] = convFloat[7];
    retFloat[1] = convFloat[6];
    retFloat[2] = convFloat[5];
    retFloat[3] = convFloat[4];
    retFloat[4] = convFloat[3];
    retFloat[5] = convFloat[2];
    retFloat[6] = convFloat[1];
    retFloat[7] = convFloat[0];

    return (double)((atUint64)retVal);
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string join(const std::vector<std::string>& elems, const std::string& delims)
{
    std::ostringstream ret;
    std::copy(elems.begin(), elems.end(), std::ostream_iterator<std::string>(ret, delims.c_str()));

    return ret.str();
}

void tolower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void toupper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

std::string vsprintf(const char* fmt, va_list list)
{
    int size = 512;
    char* buffer = 0;
    buffer = new char[size];
    int nsize = vsnprintf(buffer, size, fmt, list);
    while(size<=nsize)
    { //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize+1]; //+1 for /0
        nsize = vsnprintf(buffer, size, fmt, list);
    }
    std::string ret(buffer);
    delete[] buffer;
    return ret;
}
std::string sprintf(const char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    std::string ret = vsprintf(fmt, vl);
    va_end(vl);
    return ret;
}

bool parseBool(const std::string& boolean, bool* valid)
{
    std::string val = boolean;
    // compare must be case insensitive
    // This is the cleanest solution since I only need to do it once
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);

    // Check for true first
    if (!val.compare("true") || !val.compare("1") || !val.compare("yes") || !val.compare("on"))
    {
        if (valid)
            *valid = true;
        return true;
    }

    // Now false
    if (!val.compare("false") || !val.compare("0") || !val.compare("no") || !val.compare("off"))
    {
        if (valid)
            *valid = true;
        return false;
    }

    // Well that could've gone better

    if (valid)
        *valid = false;

    return false;
}

int countChar(const std::string& str, const char chr, int* lastOccur)
{
    int ret = 0;

    int index = 0;
    for (char c : str)
    {
        if (c == chr)
        {
            if (lastOccur != NULL)
                *lastOccur = index;

            ret++;
        }
        index++;
    }

    return ret;
}

atUint64 fileSize(FILE* f)
{
    atUint64 oldPos = ftello64(f);
    fseeko64(f, 0, SEEK_END);
    atUint64 size = ftello64(f);
    fseeko64(f, oldPos, SEEK_SET);
    return size;
}

} // utility
} // Athena
