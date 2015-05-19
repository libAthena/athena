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
#include <sys/stat.h>

#ifdef _MSC_VER
#include <functional>
#include <locale>
#define stat64 __stat64
#endif

namespace Athena
{
namespace utility
{

bool isSystemBigEndian()
{
    static atUint8* test = (atUint8*)"\xFE\xFF";
    return (*(atUint16*)test == 0xFEFF);
}

void fillRandom(atUint8* rndArea, atUint64 count)
{
    for (atUint64 i = 0; i < count; i++)
        rndArea[i] = rand();
}

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}


std::vector<std::string> split(const std::string& s, char delim)
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
    int nsize = ::vsnprintf(buffer, size, fmt, list);

    while (size <= nsize)
    {
        //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize + 1]; //+1 for /0
        nsize = ::vsnprintf(buffer, size, fmt, list);
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

atUint64 fileSize(const std::string& filename)
{
    struct stat64 st;
    stat64(filename.c_str(), &st);
    return st.st_size;
}

// trim from both ends
std::string& trim(std::string& s)
{
    // Find first non whitespace char in StrToTrim
    std::string::size_type first = s.find_first_not_of(' ');

    // Check whether something went wrong?
    if (first == std::string::npos)
    {
        first = 0;
    }

    // Find last non whitespace char from StrToTrim
    std::string::size_type last = s.find_last_not_of(' ');

    // If something didn't go wrong, Last will be recomputed to get real length of substring
    if (last != std::string::npos)
    {
        last = (last + 1) - first;
    }

    // Copy such a string to TrimmedString
    s = s.substr(first, last);

    return s;
}

} // utility
} // Athena
