﻿#include "athena/Utility.hpp"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <stdarg.h>
#include <iterator>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <random>
#include "utf8proc.h"

#ifdef _MSC_VER
#include <functional>
#include <locale>
#define stat64 __stat64
#endif

namespace athena
{
namespace utility
{

void fillRandom(atUint8* rndArea, atUint64 count)
{
    atUint8* buf = rndArea;
    for (atUint64 i = 0; i < count / 4; i++)
    {
        *(atUint32*)(buf) = rand();
        buf += 4;
    }

    atUint64 rem = count % 4;
    if (rem)
    {
        for (atUint64 j = 0; j < rem; j++)
        {
            *buf = rand();
            buf++;
        }
    }
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
    int nsize = ::vsprintf(buffer, fmt, list);

    while (size <= nsize)
    {
        //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize + 1]; //+1 for /0
        nsize = ::vsprintf(buffer, fmt, list);
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
    tolower(val);

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
    stat64_t st;
    stat64(filename.c_str(), &st);
    return st.st_size;
}

#ifdef _MSC_VER
atUint64 fileSize(const std::wstring& filename)
{
    stat64_t st;
    _wstati64(filename.c_str(), &st);
    return st.st_size;
}
#endif

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

atUint64 rand64()
{
    // Combine 4 parts of low 16-bit of each rand()
    atUint64 r0 = (atUint64)rand() << 48;
    atUint64 r1 = (atUint64)rand() << 48 >> 16;
    atUint64 r2 = (atUint64)rand() << 48 >> 32;
    atUint64 r3 = (atUint64)rand() << 48 >> 48;
    return r0 | r1 | r2 | r3;
}

std::string wideToUtf8(const std::wstring& src)
{
    std::string retval;
    retval.reserve(src.length());
    for (wchar_t ch : src)
    {
        utf8proc_uint8_t mb[4];
        utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(ch), mb);
        if (c < 0)
        {
            atWarning("invalid UTF-8 character while encoding");
            return retval;
        }
        retval.append(reinterpret_cast<char*>(mb), c);
    }
    return retval;
}

std::wstring utf8ToWide(const std::string& src)
{
    std::wstring retval;
    retval.reserve(src.length());
    const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(src.c_str());
    while (*buf)
    {
        utf8proc_int32_t wc;
        utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
        if (len < 0)
        {
            atWarning("invalid UTF-8 character while decoding");
            return retval;
        }
        buf += len;
        retval += wchar_t(wc);
    }
    return retval;
}

} // utility
} // Athena