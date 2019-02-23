#pragma once
#ifndef _ATHENA_SIMD_INCLUDED
#error simd_avx.hpp must not be included directly. Include simd.hpp instead.
#endif
#include "simd_sse.hpp"
#include <immintrin.h>
namespace athena::_simd {
// __m256d storage for AVX
template <>
class __simd_storage<double, m256d_abi> {
public:
  using storage_type = __m256d;
  storage_type __storage_;
  double __get(size_t __index) const noexcept {
    alignas(32) std::array<double, 4> sse_data;
    _mm256_store_pd(sse_data.data(), __storage_);
    return sse_data[__index];
  }
  void __set(size_t __index, double __val) noexcept {
    alignas(32) std::array<double, 4> sse_data;
    _mm256_store_pd(sse_data.data(), __storage_);
    sse_data[__index] = __val;
    __storage_ = _mm256_load_pd(sse_data.data());
  }
  constexpr __simd_storage(double a, double b, double c, double d) : __storage_{a, b, c, d} {}
  void __set4(double a, double b, double c, double d) noexcept { __storage_ = _mm256_set_pd(d, c, b, a); }
  constexpr __simd_storage(double rv) : __storage_{rv, rv, rv, rv} {}
  void __broadcast(double __val) noexcept { __storage_ = _mm256_set1_pd(__val); }
  double __dot2(const __simd_storage<double, m256d_abi>& other) const noexcept {
    alignas(32) std::array<double, 4> sse_data;
    _mm256_store_pd(sse_data.data(), _mm256_mul_pd(__storage_, other.__storage_));
    return sse_data[0] + sse_data[1];
  }
  double __dot3(const __simd_storage<double, m256d_abi>& other) const noexcept {
    alignas(32) std::array<double, 4> sse_data;
    _mm256_store_pd(sse_data.data(), _mm256_mul_pd(__storage_, other.__storage_));
    return sse_data[0] + sse_data[1] + sse_data[2];
  }
  double __dot4(const __simd_storage<double, m256d_abi>& other) const noexcept {
    alignas(32) std::array<double, 4> sse_data;
    _mm256_store_pd(sse_data.data(), _mm256_mul_pd(__storage_, other.__storage_));
    return sse_data[0] + sse_data[1] + sse_data[2] + sse_data[3];
  }

  void __copy_from(const simd_data<simd<double, m256d_abi>>& __buffer) noexcept {
    __storage_ = _mm256_load_pd(__buffer.data());
  }

  void __copy_to(simd_data<simd<double, m256d_abi>>& __buffer) const noexcept {
    _mm256_store_pd(__buffer.data(), __storage_);
  }

  __simd_storage() = default;
  explicit __simd_storage(const __simd_storage<float, m128_abi>& other) {
    __storage_ = _mm256_cvtps_pd(other.__storage_);
  }

  explicit __simd_storage(const storage_type& s) : __storage_(s) {}
  const storage_type& __native() const { return __storage_; }
};
// __m256d mask storage for AVX
template <>
class __simd_mask_storage<double, m256d_abi> : public __simd_storage<double, m256d_abi> {
public:
  bool __get(size_t __index) const noexcept {
    alignas(32) uint64_t sse_data[4];
    _mm256_store_pd(reinterpret_cast<double*>(sse_data), __storage_);
    return sse_data[__index] != 0;
  }

  void __set(size_t __index, bool __val) noexcept {
    alignas(32) uint64_t sse_data[4];
    _mm256_store_pd(reinterpret_cast<double*>(sse_data), __storage_);
    sse_data[__index] = __val ? UINT64_MAX : 0;
    __storage_ = _mm256_load_pd(reinterpret_cast<double*>(sse_data));
  }
};

template <>
inline simd<double, m256d_abi> simd<double, m256d_abi>::operator-() const {
  return _mm256_xor_pd(__s_.__storage_, _mm256_set1_pd(-0.0));
}

inline simd<double, m256d_abi> operator+(const simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi> ret;
  ret.__s_.__storage_ = _mm256_add_pd(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<double, m256d_abi> operator-(const simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi> ret;
  ret.__s_.__storage_ = _mm256_sub_pd(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<double, m256d_abi> operator*(const simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi> ret;
  ret.__s_.__storage_ = _mm256_mul_pd(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<double, m256d_abi> operator/(const simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi> ret;
  ret.__s_.__storage_ = _mm256_div_pd(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<double, m256d_abi>& operator+=(simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  a.__s_.__storage_ = _mm256_add_pd(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<double, m256d_abi>& operator-=(simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  a.__s_.__storage_ = _mm256_sub_pd(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<double, m256d_abi>& operator*=(simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  a.__s_.__storage_ = _mm256_mul_pd(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<double, m256d_abi>& operator/=(simd<double, m256d_abi>& a, const simd<double, m256d_abi>& b) {
  a.__s_.__storage_ = _mm256_div_pd(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<double, m256d_abi>::mask_type operator==(const simd<double, m256d_abi>& a,
                                                     const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm256_cmp_pd(a.__s_.__storage_, b.__s_.__storage_, _CMP_EQ_OQ);
  return ret;
}

inline simd<double, m256d_abi>::mask_type operator!=(const simd<double, m256d_abi>& a,
                                                     const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm256_cmp_pd(a.__s_.__storage_, b.__s_.__storage_, _CMP_NEQ_OQ);
  return ret;
}

inline simd<double, m256d_abi>::mask_type operator>=(const simd<double, m256d_abi>& a,
                                                     const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm256_cmp_pd(a.__s_.__storage_, b.__s_.__storage_, _CMP_GE_OQ);
  return ret;
}

inline simd<double, m256d_abi>::mask_type operator<=(const simd<double, m256d_abi>& a,
                                                     const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm256_cmp_pd(a.__s_.__storage_, b.__s_.__storage_, _CMP_LE_OQ);
  return ret;
}

inline simd<double, m256d_abi>::mask_type operator>(const simd<double, m256d_abi>& a,
                                                    const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm256_cmp_pd(a.__s_.__storage_, b.__s_.__storage_, _CMP_GT_OQ);
  return ret;
}

inline simd<double, m256d_abi>::mask_type operator<(const simd<double, m256d_abi>& a,
                                                    const simd<double, m256d_abi>& b) {
  simd<double, m256d_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm256_cmp_pd(a.__s_.__storage_, b.__s_.__storage_, _CMP_LT_OQ);
  return ret;
}

inline __simd_storage<float, m128_abi>::__simd_storage(const __simd_storage<double, m256d_abi>& other) {
  __storage_ = _mm256_cvtpd_ps(other.__storage_);
}

namespace simd_abi {
template <>
struct athena_native<double> {
  using type = m256d_abi;
};
} // namespace simd_abi

} // namespace athena::_simd