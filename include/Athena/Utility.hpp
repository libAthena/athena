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
#include "Types.hpp"

namespace Athena
{
namespace utility
{
bool isEmpty(Int8*, Uint32);

Uint16 swapU16(Uint16 val );
Int16  swap16 (Int16 val );
Uint32 swapU32(Uint32 val);
Int32  swap32 (Int32 val );
Uint64 swapU64(Uint64 val);
Int64  swap64 (Int64 val);

float swapFloat(float val);
double  swapDouble(double val);

bool isSystemBigEndian();

void fillRandom(Uint8 * rndArea, Uint8 count);

std::vector<std::string> split(const std::string &s, char delim);
std::string join(const std::vector<std::string>& elems, const std::string& delims);
void tolower(std::string& str);
void toupper(std::string& str);
std::string stdsprintf(const char* fmt, ...);
bool parseBool(const std::string& boolean, bool* valid = NULL);

int countChar(const std::string& str, const char chr, int* lastOccur = NULL);

Uint64 fileSize(FILE* f);
} // utility
} // Athena
#endif
