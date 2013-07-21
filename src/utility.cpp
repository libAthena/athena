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

bool isEmpty(Int8* buf, size_t size)
{
    return buf[0] == 0 && !memcmp(buf, buf + 1, size - 1);
}

unsigned short swapU16(unsigned short val )
{
    return (val << 8) | (val >> 8 );
}

short swap16(short val )
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

unsigned int swapU32(unsigned int val)
{
    val = (val & 0x0000FFFF) << 16 | (val & 0xFFFF0000) >> 16;
    val = (val & 0x00FF00FF) << 8 | (val & 0xFF00FF00) >> 8;
    return (Uint32)val;
}

int swap32( int val )
{
    val = (val & 0x0000FFFF) << 16 | (val & 0xFFFF0000) >> 16;
    val = (val & 0x00FF00FF) << 8 | (val & 0xFF00FF00) >> 8;
    return val;
}

long long swap64(long long val)
{
    return ((long long)((((long long)(val) & 0xFF00000000000000ULL) >> 56) |
          (((long long)(val) & 0x00FF000000000000ULL) >> 40) |
          (((long long)(val) & 0x0000FF0000000000ULL) >> 24) |
          (((long long)(val) & 0x000000FF00000000ULL) >>  8) |
          (((long long)(val) & 0x00000000FF000000ULL) <<  8) |
          (((long long)(val) & 0x0000000000FF0000ULL) << 24) |
          (((long long)(val) & 0x000000000000FF00ULL) << 40) |
          (((long long)(val) & 0x00000000000000FFULL) << 56)));
}

bool isSystemBigEndian()
{
    char* test = (char*)"\xFE\xFF";
    return (*(unsigned short*)test == 0xFEFF);
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

//src points to the yaz0 source data (to the "real" source data, not at the header!)
//dst points to a buffer uncompressedSize bytes large (you get uncompressedSize from
//the second 4 bytes in the Yaz0 header).
void yaz0Decode(Uint8* src, Uint8* dst, Uint32 uncompressedSize)
{
  Uint32 srcPlace = 0, dstPlace = 0; //current read/write positions

  Int32 validBitCount = 0; //number of valid bits left in "code" byte
  Uint8 currCodeByte;
  while(dstPlace < uncompressedSize)
  {
    //read new "code" byte if the current one is used up
    if(validBitCount == 0)
    {
      currCodeByte = src[srcPlace];
      ++srcPlace;
      validBitCount = 8;
    }

    if((currCodeByte & 0x80) != 0)
    {
      //straight copy
      dst[dstPlace] = src[srcPlace];
      dstPlace++;
      srcPlace++;
    }
    else
    {
      //RLE part
      Uint8 byte1 = src[srcPlace];
      Uint8 byte2 = src[srcPlace + 1];
      srcPlace += 2;

      Uint32 dist = ((byte1 & 0xF) << 8) | byte2;
      Uint32 copySource = dstPlace - (dist + 1);

      Uint32 numBytes = byte1 >> 4;
      if(numBytes == 0)
      {
        numBytes = src[srcPlace] + 0x12;
        srcPlace++;
      }
      else
        numBytes += 2;

      //copy run
      for(Uint32 i = 0; i < numBytes; ++i)
      {
        dst[dstPlace] = dst[copySource];
        copySource++;
        dstPlace++;
      }
    }

    //use next bit from "code" byte
    currCodeByte <<= 1;
    validBitCount-=1;
  }
}
