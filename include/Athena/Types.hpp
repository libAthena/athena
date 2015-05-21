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

#ifndef TYPES_HPP
#define TYPES_HPP
#include <limits.h>

// 8 bits integer types
#if UCHAR_MAX == 0xFF
typedef signed   char atInt8;
typedef unsigned char atUint8;
#else
#error No 8 bits integer type for this platform
#endif

// 16 bits integer types
#if USHRT_MAX == 0xFFFF
typedef signed   short atInt16;
typedef unsigned short atUint16;
#elif UINT_MAX == 0xFFFF
typedef signed   int atInt16;
typedef unsigned int atUint16;
#elif ULONG_MAX == 0xFFFF
typedef signed   long atInt16;
typedef unsigned long atUint16;
#else
#error No 16 bits integer type for this platform
#endif

// 32 bits integer types
#if USHRT_MAX == 0xFFFFFFFF
typedef signed   short atInt32;
typedef unsigned short atUint32;
#elif UINT_MAX == 0xFFFFFFFF
typedef signed   int atInt32;
typedef unsigned int atUint32;
#elif ULONG_MAX == 0xFFFFFFFF
typedef signed   long atInt32;
typedef unsigned long atUint32;
#else
#error No 32 bits integer type for this platform
#endif

typedef signed   long long atInt64;
typedef unsigned long long atUint64;

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else // __cplusplus
#define NULL (void*)0
#endif
#endif // NULL

#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif // UNUSED

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

#endif // TYPES_HPP
