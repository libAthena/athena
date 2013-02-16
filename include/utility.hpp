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

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string>
#include "Types.hpp"

bool isEmpty(Int8*, size_t);

unsigned short swapU16(unsigned short val );
         short swap16 (short val );
unsigned int   swapU32(unsigned int val);
         int   swap32 (int val );
    long long  swap64 (long long val);

float          swapFloat(float val);
double         swapDouble(double val);

bool isSystemBigEndian();

void fillRandom(Uint8 * rndArea, Uint8 count);


#endif
