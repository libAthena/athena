#pragma once

#include <cinttypes>
#include <cstdint>

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
