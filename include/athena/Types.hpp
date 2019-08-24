#pragma once

#include <cinttypes>
#include <cstdint>

using atInt8 = std::int8_t;
using atUint8 = std::uint8_t;
using atInt16 = std::int16_t;
using atUint16 = std::uint16_t;
using atInt32 = std::int32_t;
using atUint32 = std::uint32_t;
using atInt64 = std::int64_t;
using atUint64 = std::uint64_t;

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
