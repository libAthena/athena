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
#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <limits.h>

/*! \enum Endian
 *  \brief Allows the user to specify the Endianness of data.<br />
 *         The proper actions are automatically taken depending on platform and
 *         buffer settings
 */
enum Endian
{
    LittleEndian, //!< Specifies that the Stream is Little Endian (LSB)
    BigEndian //!< Specifies that the Stream is Big Endian (MSB)
};

// 8 bits integer types
#if UCHAR_MAX == 0xFF
    typedef signed   char Int8;
    typedef unsigned char Uint8;
#else
    #error No 8 bits integer type for this platform
#endif

// 16 bits integer types
#if USHRT_MAX == 0xFFFF
    typedef signed   short Int16;
    typedef unsigned short Uint16;
#elif UINT_MAX == 0xFFFF
    typedef signed   int Int16;
    typedef unsigned int Uint16;
#elif ULONG_MAX == 0xFFFF
    typedef signed   long Int16;
    typedef unsigned long Uint16;
#else
    #error No 16 bits integer type for this platform
#endif

// 32 bits integer types
#if USHRT_MAX == 0xFFFFFFFF
    typedef signed   short Int32;
    typedef unsigned short Uint32;
#elif UINT_MAX == 0xFFFFFFFF
    typedef signed   int Int32;
    typedef unsigned int Uint32;
#elif ULONG_MAX == 0xFFFFFFFF
    typedef signed   long Int32;
    typedef unsigned long Uint32;
#else
    #error No 32 bits integer type for this platform
#endif

typedef signed   long long Int64;
typedef unsigned long long Uint64;

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else // __cplusplus
#define NULL (void*)0
#endif
#endif // NULL

#endif
