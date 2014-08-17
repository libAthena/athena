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
#include "Types.hpp"

namespace Athena
{
namespace utility
{
bool isEmpty(atInt8*, atUint32);

atUint16 swapU16(atUint16 val );
atInt16  swap16 (atInt16 val );
atUint32 swapU32(atUint32 val);
atInt32  swap32 (atInt32 val );
atUint64 swapU64(atUint64 val);
atInt64  swap64 (atInt64 val);

float swapFloat(float val);
double  swapDouble(double val);

bool isSystemBigEndian();

<<<<<<< Updated upstream
void fillRandom(atUint8 * rndArea, atUint64 count);
=======
void fillRandom(atUint8 * rndArea, atUint8 count);
>>>>>>> Stashed changes

std::vector<std::string> split(const std::string &s, char delim);
std::string join(const std::vector<std::string>& elems, const std::string& delims);
void tolower(std::string& str);
void toupper(std::string& str);
std::string vsprintf(const char* fmt, va_list list);
std::string sprintf(const char* fmt, ...);
bool parseBool(const std::string& boolean, bool* valid = NULL);

int countChar(const std::string& str, const char chr, int* lastOccur = NULL);

atUint64 fileSize(FILE* f);
} // utility
} // Athena
#endif
