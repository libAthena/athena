#pragma once
#ifndef _ATHENA_SIMD_INCLUDED
#error simd_sse.hpp must not be included directly. Include simd.hpp instead.
#endif
#include <xmmintrin.h>
#include <emmintrin.h>
#if __SSE4_1__
#include <smmintrin.h>
#endif
namespace athena::_simd {
// __m128 ABI
using m128_abi = __simd_abi<_StorageKind(int(_StorageKind::_Array) + 1), 4>;
// __m128d ABI
using m128d_abi = __simd_abi<_StorageKind(int(_StorageKind::_Array) + 2), 4>;
#ifdef __AVX__
// __m256d ABI
using m256d_abi = __simd_abi<_StorageKind(int(_StorageKind::_Array) + 3), 4>;
#endif

template <>
class __simd_storage<double, m128d_abi>;
#ifdef __AVX__
template <>
class __simd_storage<double, m256d_abi>;
#endif

// __m128 storage for SSE2+
template <>
class __simd_storage<float, m128_abi> {
public:
  using storage_type = __m128;
  storage_type __storage_;
  float __get(size_t __index) const noexcept {
    alignas(16) std::array<float, 4> sse_data;
    _mm_store_ps(sse_data.data(), __storage_);
    return sse_data[__index];
  }
  void __set(size_t __index, float __val) noexcept {
    alignas(16) std::array<float, 4> sse_data;
    _mm_store_ps(sse_data.data(), __storage_);
    sse_data[__index] = __val;
    __storage_ = _mm_load_ps(sse_data.data());
  }
  void __set4(float a, float b, float c, float d) noexcept { __storage_ = _mm_set_ps(d, c, b, a); }
  void __broadcast(float __val) noexcept { __storage_ = _mm_set1_ps(__val); }
  float __dot2(const __simd_storage<float, m128_abi>& other) const noexcept {
#if __SSE4_1__
    float ret;
    _mm_store_ss(&ret, _mm_dp_ps(__storage_, other.__storage_, 0x3F));
    return ret;
#else
    alignas(16) std::array<float, 4> sse_data;
    _mm_store_ps(sse_data.data(), _mm_mul_ps(__storage_, other.__storage_));
    return sse_data[0] + sse_data[1];
#endif
  }
  float __dot3(const __simd_storage<float, m128_abi>& other) const noexcept {
#if __SSE4_1__
    float ret;
    _mm_store_ss(&ret, _mm_dp_ps(__storage_, other.__storage_, 0x7F));
    return ret;
#else
    alignas(16) std::array<float, 4> sse_data;
    _mm_store_ps(sse_data.data(), _mm_mul_ps(__storage_, other.__storage_));
    return sse_data[0] + sse_data[1] + sse_data[2];
#endif
  }
  float __dot4(const __simd_storage<float, m128_abi>& other) const noexcept {
#if __SSE4_1__
    float ret;
    _mm_store_ss(&ret, _mm_dp_ps(__storage_, other.__storage_, 0xFF));
    return ret;
#else
    alignas(16) std::array<float, 4> sse_data;
    _mm_store_ps(sse_data.data(), _mm_mul_ps(__storage_, other.__storage_));
    return sse_data[0] + sse_data[1] + sse_data[2] + sse_data[3];
#endif
  }
  template <int x, int y, int z, int w>
  __simd_storage __shuffle() const noexcept {
    __simd_storage s;
    s.__storage_ = _mm_shuffle_ps(__storage_, __storage_, _MM_SHUFFLE(w, z, y, x));
    return s;
  }

  void __copy_from(const simd_data<simd<float, m128_abi>>& __buffer) noexcept {
    __storage_ = _mm_load_ps(__buffer.data());
  }

  void __copy_to(simd_data<simd<float, m128_abi>>& __buffer) const noexcept {
    _mm_store_ps(__buffer.data(), __storage_);
  }

  __simd_storage() = default;
  explicit __simd_storage(const __simd_storage<double, m128d_abi>& other);
#ifdef __AVX__
  explicit __simd_storage(const __simd_storage<double, m256d_abi>& other);
#endif

  explicit __simd_storage(const storage_type& s) : __storage_(s) {}
  const storage_type& __native() const { return __storage_; }
};
// __m128 mask storage for SSE2+
template <>
class __simd_mask_storage<float, m128_abi> : public __simd_storage<float, m128_abi> {
public:
  bool __get(size_t __index) const noexcept {
    alignas(16) uint32_t sse_data[4];
    _mm_store_ps(reinterpret_cast<float*>(sse_data), __storage_);
    return sse_data[__index] != 0;
  }
  void __set(size_t __index, bool __val) noexcept {
    alignas(16) uint32_t sse_data[4];
    _mm_store_ps(reinterpret_cast<float*>(sse_data), __storage_);
    sse_data[__index] = __val ? UINT32_MAX : 0;
    __storage_ = _mm_load_ps(reinterpret_cast<float*>(sse_data));
  }
};

template <>
inline simd<float, m128_abi> simd<float, m128_abi>::operator-() const {
  return _mm_xor_ps(__s_.__storage_, _mm_set1_ps(-0.f));
}

inline simd<float, m128_abi> operator+(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi> ret;
  ret.__s_.__storage_ = _mm_add_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi> operator-(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi> ret;
  ret.__s_.__storage_ = _mm_sub_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi> operator*(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi> ret;
  ret.__s_.__storage_ = _mm_mul_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi> operator/(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi> ret;
  ret.__s_.__storage_ = _mm_div_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi>& operator+=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ = _mm_add_ps(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<float, m128_abi>& operator-=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ = _mm_sub_ps(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<float, m128_abi>& operator*=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ = _mm_mul_ps(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<float, m128_abi>& operator/=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ = _mm_div_ps(a.__s_.__storage_, b.__s_.__storage_);
  return a;
}

inline simd<float, m128_abi>::mask_type operator==(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm_cmpeq_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi>::mask_type operator!=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm_cmpneq_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi>::mask_type operator>=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm_cmpge_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi>::mask_type operator<=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm_cmple_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi>::mask_type operator>(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm_cmpgt_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

inline simd<float, m128_abi>::mask_type operator<(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = _mm_cmplt_ps(a.__s_.__storage_, b.__s_.__storage_);
  return ret;
}

// __m128d storage for SSE2+
template <>
class __simd_storage<double, m128d_abi> {
public:
  using storage_type = std::array<__m128d, 2>;
  storage_type __storage_;
  double __get(size_t __index) const noexcept {
    alignas(16) std::array<double, 2> sse_data;
    _mm_store_pd(sse_data.data(), __storage_[__index / 2]);
    return sse_data[__index % 2];
  }
  void __set(size_t __index, double __val) noexcept {
    alignas(16) std::array<double, 2> sse_data;
    _mm_store_pd(sse_data.data(), __storage_[__index / 2]);
    sse_data[__index % 2] = __val;
    __storage_[__index / 2] = _mm_load_pd(sse_data.data());
  }
  void __set4(double a, double b, double c, double d) noexcept {
    __storage_[0] = _mm_set_pd(b, a);
    __storage_[1] = _mm_set_pd(d, c);
  }
  void __broadcast(double __val) noexcept {
    for (int i = 0; i < 2; ++i)
      __storage_[i] = _mm_set1_pd(__val);
  }
  double __dot2(const __simd_storage<double, m128d_abi>& other) const noexcept {
#if __SSE4_1__
    double ret;
    _mm_store_sd(&ret, _mm_dp_pd(__storage_[0], other.__storage_[0], 0x3F));
    return ret;
#else
    alignas(16) std::array<double, 2> sse_data;
    _mm_store_pd(sse_data.data(), _mm_mul_pd(__storage_[0], other.__storage_[0]));
    return sse_data[0] + sse_data[1];
#endif
  }
  double __dot3(const __simd_storage<double, m128d_abi>& other) const noexcept {
#if __SSE4_1__
    double ret;
    _mm_store_sd(&ret, _mm_dp_pd(__storage_[0], other.__storage_[0], 0x3F));
    alignas(16) std::array<double, 2> sse_data2;
    _mm_store_pd(sse_data2.data(), _mm_mul_pd(__storage_[1], other.__storage_[1]));
    return ret + sse_data2[0];
#else
    alignas(16) std::array<double, 2> sse_data;
    _mm_store_pd(sse_data.data(), _mm_mul_pd(__storage_[0], other.__storage_[0]));
    alignas(16) std::array<double, 2> sse_data2;
    _mm_store_pd(sse_data2.data(), _mm_mul_pd(__storage_[1], other.__storage_[1]));
    return sse_data[0] + sse_data[1] + sse_data2[0];
#endif
  }
  double __dot4(const __simd_storage<double, m128d_abi>& other) const noexcept {
#if __SSE4_1__
    double ret;
    _mm_store_sd(&ret, _mm_dp_pd(__storage_[0], other.__storage_[0], 0x3F));
    double ret2;
    _mm_store_sd(&ret2, _mm_dp_pd(__storage_[1], other.__storage_[1], 0x3F));
    return ret + ret2;
#else
    alignas(16) std::array<double, 2> sse_data;
    _mm_store_pd(sse_data.data(), _mm_mul_pd(__storage_[0], other.__storage_[0]));
    alignas(16) std::array<double, 2> sse_data2;
    _mm_store_pd(sse_data2.data(), _mm_mul_pd(__storage_[1], other.__storage_[1]));
    return sse_data[0] + sse_data[1] + sse_data2[0] + sse_data2[1];
#endif
  }

  void __copy_from(const simd_data<simd<double, m128d_abi>>& __buffer) noexcept {
    __storage_[0] = _mm_load_pd(__buffer.data());
    __storage_[1] = _mm_load_pd(__buffer.data() + 2);
  }

  void __copy_to(simd_data<simd<double, m128d_abi>>& __buffer) const noexcept {
    _mm_store_pd(__buffer.data(), __storage_[0]);
    _mm_store_pd(__buffer.data() + 2, __storage_[1]);
  }

  __simd_storage() = default;
  explicit __simd_storage(const __simd_storage<float, m128_abi>& other) {
    __storage_[0] = _mm_cvtps_pd(other.__storage_);
    __storage_[1] = _mm_cvtps_pd(_mm_movehl_ps(other.__storage_, other.__storage_));
  }

  explicit __simd_storage(const storage_type& s) : __storage_(s) {}
  const storage_type& __native() const { return __storage_; }
};
// __m128d mask storage for SSE2+
template <>
class __simd_mask_storage<double, m128d_abi> : public __simd_storage<double, m128d_abi> {
public:
  bool __get(size_t __index) const noexcept {
    alignas(16) uint64_t sse_data[2];
    _mm_store_pd(reinterpret_cast<double*>(sse_data), __storage_[__index / 2]);
    return sse_data[__index] != 0;
  }
  void __set(size_t __index, bool __val) noexcept {
    alignas(16) uint64_t sse_data[2];
    _mm_store_pd(reinterpret_cast<double*>(sse_data), __storage_[__index / 2]);
    sse_data[__index % 2] = __val ? UINT64_MAX : 0;
    __storage_[__index / 2] = _mm_load_pd(reinterpret_cast<double*>(sse_data));
  }
};

template <>
inline simd<double, m128d_abi> simd<double, m128d_abi>::operator-() const {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_xor_pd(__s_.__storage_[i], _mm_set1_pd(-0.0));
  return ret;
}

inline simd<double, m128d_abi> operator+(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_add_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi> operator-(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_sub_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi> operator*(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_mul_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi> operator/(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_div_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi>& operator+=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_[i] = _mm_add_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return a;
}

inline simd<double, m128d_abi>& operator-=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_[i] = _mm_sub_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return a;
}

inline simd<double, m128d_abi>& operator*=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_[i] = _mm_mul_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return a;
}

inline simd<double, m128d_abi>& operator/=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_[i] = _mm_div_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return a;
}

inline simd<double, m128d_abi>::mask_type operator==(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_cmpeq_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator!=(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_cmpneq_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator>=(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_cmpge_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator<=(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_cmple_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator>(const simd<double, m128d_abi>& a,
                                                    const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_cmpgt_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator<(const simd<double, m128d_abi>& a,
                                                    const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_[i] = _mm_cmplt_pd(a.__s_.__storage_[i], b.__s_.__storage_[i]);
  return ret;
}

inline __simd_storage<float, m128_abi>::__simd_storage(const __simd_storage<double, m128d_abi>& other) {
  __storage_ = _mm_movelh_ps(_mm_cvtpd_ps(other.__storage_[0]), _mm_cvtpd_ps(other.__storage_[1]));
}

namespace simd_abi {
template <typename T>
struct athena_native {};
template <>
struct athena_native<float> {
  using type = m128_abi;
};
#ifndef __AVX__
template <>
struct athena_native<double> {
  using type = m128d_abi;
};
#endif
} // namespace simd_abi

} // namespace athena::_simd
