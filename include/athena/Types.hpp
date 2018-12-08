#pragma once
#include <cstdint>
#include <cinttypes>

using atInt8 = int8_t;
using atUint8 = uint8_t;
using atInt16 = int16_t;
using atUint16 = uint16_t;
using atInt32 = int32_t;
using atUint32 = uint32_t;
using atInt64 = int64_t;
using atUint64 = uint64_t;

// Vector types
#include "simd/simd.hpp"
typedef struct { athena::simd<float> simd; } atVec2f;
typedef struct { athena::simd<float> simd; } atVec3f;
typedef struct { athena::simd<float> simd; } atVec4f;
typedef struct { athena::simd<double> simd; } atVec2d;
typedef struct { athena::simd<double> simd; } atVec3d;
typedef struct { athena::simd<double> simd; } atVec4d;

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

