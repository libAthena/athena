// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#include "utility.hpp"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <cstdarg>
#include <iterator>

namespace zelda
{
namespace utility
{

bool isEmpty(Int8* buf, Uint32 size)
{
    return buf[0] == 0 && !memcmp(buf, buf + 1, size - 1);
}

Uint16 swapU16(Uint16 val )
{
    return (Uint16)swap16(val);
}

Int16 swap16(Int16 val )
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

Uint32 swapU32(Uint32 val)
{
    return (Uint32)swap32(val);
}

int swap32(Int32 val )
{
    val = (val & 0x0000FFFF) << 16 | (val & 0xFFFF0000) >> 16;
    val = (val & 0x00FF00FF) << 8 | (val & 0xFF00FF00) >> 8;
    return val;
}

Uint64 swapU64(Uint64 val)
{
    return (Uint64)swap64(val);
}

Int64 swap64(Int64 val)
{
    return ((Int64)((((Int64)(val) & 0xFF00000000000000ULL) >> 56) |
                        (((Int64)(val) & 0x00FF000000000000ULL) >> 40) |
                        (((Int64)(val) & 0x0000FF0000000000ULL) >> 24) |
                        (((Int64)(val) & 0x000000FF00000000ULL) >>  8) |
                        (((Int64)(val) & 0x00000000FF000000ULL) <<  8) |
                        (((Int64)(val) & 0x0000000000FF0000ULL) << 24) |
                        (((Int64)(val) & 0x000000000000FF00ULL) << 40) |
                        (((Int64)(val) & 0x00000000000000FFULL) << 56)));
}

bool isSystemBigEndian()
{
    Uint8* test = (Uint8*)"\xFE\xFF";
    return (*(Uint16*)test == 0xFEFF);
}

void fillRandom(Uint8 * rndArea, Uint8 count)
{
    for(Uint16 i = 0; i < count; i++)
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

    return (double)((Uint64)retVal);
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

std::string sprintf(const char* fmt, ...)
{
    int size = 512;
    char* buffer = 0;
    buffer = new char[size];
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    if(size<=nsize)
    { //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize+1]; //+1 for /0
        nsize = vsnprintf(buffer, size, fmt, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete[] buffer;
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


} // utility
} // zelda
