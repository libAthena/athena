#pragma once
#define _ATHENA_SIMD_INCLUDED
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
namespace athena::_simd {
using namespace std;
}
#include "parallelism_v2_simd.hpp"
#if _M_IX86_FP >= 1 || _M_X64
#define __SSE__ 1
#endif
#if __AVX__
#include "simd_avx.hpp"
#elif __SSE__
#include "simd_sse.hpp"
#else
namespace simd_abi {
template <typename T>
struct athena_native {};
template <>
struct athena_native<float> {
  using type = fixed_size<4>;
};
template <>
struct athena_native<double> {
  using type = fixed_size<4>;
};
} // namespace simd_abi
#endif
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
namespace athena {
template <typename T>
using simd = _simd::simd<T, typename _simd::simd_abi::athena_native<T>::type>;
template <typename T>
using simd_values = _simd::simd_data<simd<T>>;
using simd_floats = simd_values<float>;
using simd_doubles = simd_values<double>;
} // namespace athena
