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
struct atVec2f {
  athena::simd<float> simd;
};
struct atVec3f {
  athena::simd<float> simd;
};
struct atVec4f {
  athena::simd<float> simd;
};
struct atVec2d {
  athena::simd<double> simd;
};
struct atVec3d {
  athena::simd<double> simd;
};
struct atVec4d {
  athena::simd<double> simd;
};
