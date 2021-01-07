#pragma once
#include <arm_neon.h>
#ifndef _ATHENA_SIMD_INCLUDED
#error simd_neon.hpp must not be included directly. Include simd.hpp instead.
#endif
namespace athena::_simd {
// __m128 ABI
using m128_abi = __simd_abi<_StorageKind(int(_StorageKind::_Array) + 1), 4>;
// __m128d ABI
using m128d_abi = __simd_abi<_StorageKind(int(_StorageKind::_Array) + 2), 4>;

template <>
class __simd_storage<double, m128d_abi>;

// __m128 storage for NEON
template <>
class __simd_storage<float, m128_abi> {
public:
  using storage_type = float32x4_t;
  storage_type __storage_{};
  [[nodiscard]] float __get(size_t __index) const noexcept { return __storage_[__index]; }
  inline void __set(size_t __index, float __val) noexcept { __storage_[__index] = __val; }
  constexpr __simd_storage(float a, float b, float c, float d) : __storage_{a, b, c, d} {}
  constexpr void __set4(float a, float b, float c, float d) noexcept { __storage_ = storage_type{a, b, c, d}; }
  constexpr explicit __simd_storage(float rv) : __simd_storage(rv, rv, rv, rv) {}
  inline void __broadcast(float __val) noexcept { __storage_ = vdupq_n_f32(__val); }
  [[nodiscard]] inline float __dot2(const __simd_storage<float, m128_abi>& other) const noexcept {
    return vaddv_f32(vget_low_f32(vmulq_f32(__storage_, other.__storage_)));
  }
  [[nodiscard]] inline float __dot3(const __simd_storage<float, m128_abi>& other) const noexcept {
    return vaddvq_f32(vsetq_lane_f32(0.f, vmulq_f32(__storage_, other.__storage_), 3));
  }
  [[nodiscard]] inline float __dot4(const __simd_storage<float, m128_abi>& other) const noexcept {
    return vaddvq_f32(vmulq_f32(__storage_, other.__storage_));
  }
  template <int x, int y, int z, int w>
  [[nodiscard]] inline __simd_storage __shuffle() const noexcept {
    storage_type ret;
    ret = vmovq_n_f32(vgetq_lane_f32(__storage_, x));
    ret = vsetq_lane_f32(vgetq_lane_f32(__storage_, y), ret, 1);
    ret = vsetq_lane_f32(vgetq_lane_f32(__storage_, z), ret, 2);
    ret = vsetq_lane_f32(vgetq_lane_f32(__storage_, w), ret, 3);
    return __simd_storage(ret);
  }

  inline void __copy_from(const simd_data<simd<float, m128_abi>>& __buffer) noexcept {
    __storage_ = vld1q_f32(__buffer.data());
  }

  inline void __copy_to(simd_data<simd<float, m128_abi>>& __buffer) const noexcept { vst1q_f32(__buffer.data(), __storage_); }

  constexpr __simd_storage() = default;
  explicit __simd_storage(const __simd_storage<double, m128d_abi>& other);

  constexpr explicit __simd_storage(const storage_type& s) : __storage_(s) {}
  [[nodiscard]] constexpr const storage_type& __native() const { return __storage_; }
};
// __m128 mask storage for NEON
template <>
class __simd_mask_storage<float, m128_abi> : public __simd_storage<float, m128_abi> {
public:
  inline bool __get(size_t __index) const noexcept {
    return vreinterpretq_u32_f32(__storage_)[__index] != 0;
  }
  inline void __set(size_t __index, bool __val) noexcept {
    uint32x4_t data = vreinterpretq_u32_f32(__storage_);
    data[__index] = __val ? UINT32_MAX : 0;
    __storage_ = vreinterpretq_f32_u32(data);
  }
};

template <>
inline simd<float, m128_abi> simd<float, m128_abi>::operator-() const {
  return vreinterpretq_f32_s32(
      veorq_s32(vreinterpretq_s32_f32(__s_.__storage_), vreinterpretq_s32_f32(vdupq_n_f32(-0.f))));
}

inline simd<float, m128_abi> operator+(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return vaddq_f32(a.__s_.__storage_, b.__s_.__storage_);
}

inline simd<float, m128_abi> operator-(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return vsubq_f32(a.__s_.__storage_, b.__s_.__storage_);
}

inline simd<float, m128_abi> operator*(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return vmulq_f32(a.__s_.__storage_, b.__s_.__storage_);
}

inline simd<float, m128_abi> operator/(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return vdivq_f32(a.__s_.__storage_, b.__s_.__storage_);
}

inline simd<float, m128_abi>& operator+=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ += b.__s_.__storage_;
  return a;
}

inline simd<float, m128_abi>& operator-=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ -= b.__s_.__storage_;
  return a;
}

inline simd<float, m128_abi>& operator*=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ *= b.__s_.__storage_;
  return a;
}

inline simd<float, m128_abi>& operator/=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a.__s_.__storage_ /= b.__s_.__storage_;
  return a;
}

inline simd<float, m128_abi>::mask_type operator==(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = vreinterpretq_f32_u32(vceqq_f32(a.__s_.__storage_, b.__s_.__storage_));
  return ret;
}

inline simd<float, m128_abi>::mask_type operator!=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = vreinterpretq_f32_u32(vmvnq_u32(vceqq_f32(a.__s_.__storage_, b.__s_.__storage_)));
  return ret;
}

inline simd<float, m128_abi>::mask_type operator>=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = vreinterpretq_f32_u32(vcgeq_f32(a.__s_.__storage_, b.__s_.__storage_));
  return ret;
}

inline simd<float, m128_abi>::mask_type operator<=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = vreinterpretq_f32_u32(vcleq_f32(a.__s_.__storage_, b.__s_.__storage_));
  return ret;
}

inline simd<float, m128_abi>::mask_type operator>(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = vreinterpretq_f32_u32(vcgtq_f32(a.__s_.__storage_, b.__s_.__storage_));
  return ret;
}

inline simd<float, m128_abi>::mask_type operator<(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret.__s_.__storage_ = vreinterpretq_f32_u32(vcltq_f32(a.__s_.__storage_, b.__s_.__storage_));
  return ret;
}

// __m128d storage for NEON
template <>
class __simd_storage<double, m128d_abi> {
public:
  using storage_type = float64x2x2_t;
  using vector_type = float64x2_t;
  storage_type __storage_{};
  [[nodiscard]] inline double __get(size_t __index) const noexcept { return __storage_.val[__index / 2][__index % 2]; }
  inline void __set(size_t __index, double __val) noexcept { __storage_.val[__index / 2][__index % 2] = __val; }
  // Make GCC happy
  static constexpr storage_type __make_array(vector_type a, vector_type b) { return {a, b}; }
  constexpr __simd_storage(double a, double b, double c, double d)
  : __storage_(__make_array(vector_type{a, b}, vector_type{c, d})) {}
  constexpr void __set4(double a, double b, double c, double d) noexcept {
    __storage_.val[0] = vector_type{a, b};
    __storage_.val[1] = vector_type{c, d};
  }
  constexpr __simd_storage(double rv) : __simd_storage(rv, rv, rv, rv) {}
  constexpr void __broadcast(double __val) noexcept { __set4(__val, __val, __val, __val); }
  [[nodiscard]] inline double __dot2(const __simd_storage<double, m128d_abi>& other) const noexcept {
    return vaddvq_f64(vmulq_f64(__storage_.val[0], other.__storage_.val[0]));
  }
  [[nodiscard]] inline double __dot3(const __simd_storage<double, m128d_abi>& other) const noexcept {
    const vector_type mul1 = vmulq_f64(__storage_.val[0], other.__storage_.val[0]);
    const vector_type mul2 = vmulq_f64(__storage_.val[1], other.__storage_.val[1]);
    return vaddvq_f64(vcombine_f64(vcreate_f64(vaddvq_f64(mul1)), vget_low_f64(mul2)));
  }
  [[nodiscard]] inline double __dot4(const __simd_storage<double, m128d_abi>& other) const noexcept {
    const vector_type mul1 = vmulq_f64(__storage_.val[0], other.__storage_.val[0]);
    const vector_type mul2 = vmulq_f64(__storage_.val[1], other.__storage_.val[1]);
    return vaddvq_f64(vaddq_f64(mul1, mul2));
  }

  inline void __copy_from(const simd_data<simd<double, m128d_abi>>& __buffer) noexcept {
    __storage_ = vld2q_f64(__buffer.data());
  }

  inline void __copy_to(simd_data<simd<double, m128d_abi>>& __buffer) const noexcept {
    vst2q_f64(__buffer.data(), __storage_);
  }

  constexpr __simd_storage() = default;
  explicit inline __simd_storage(const __simd_storage<float, m128_abi>& other) {
    __storage_.val[0] = vcvt_f64_f32(vget_low_f32(other.__storage_));
    __storage_.val[1] = vcvt_f64_f32(vget_high_f32(other.__storage_));
  }

  constexpr explicit __simd_storage(const storage_type& s) : __storage_(s) {}
  [[nodiscard]] constexpr const storage_type& __native() const { return __storage_; }
};
// __m128d mask storage for NEON
template <>
class __simd_mask_storage<double, m128d_abi> : public __simd_storage<double, m128d_abi> {
public:
  inline bool __get(size_t __index) const noexcept {
    return vreinterpretq_u64_f64(__storage_.val[__index / 2])[__index % 2] != 0;
  }
  inline void __set(size_t __index, bool __val) noexcept {
    uint64x2_t vec = vreinterpretq_u64_f64(__storage_.val[__index / 2]);
    vec[__index % 2] = __val ? UINT64_MAX : 0;
    __storage_.val[__index / 2] = vreinterpretq_f64_u64(vec);
  }
};

template <>
inline simd<double, m128d_abi> simd<double, m128d_abi>::operator-() const {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vreinterpretq_f64_s64(
        veorq_s64(vreinterpretq_s64_f64(__s_.__storage_.val[i]), vreinterpretq_s64_f64(vdupq_n_f64(-0.0))));
  return ret;
}

inline simd<double, m128d_abi> operator+(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vaddq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return ret;
}

inline simd<double, m128d_abi> operator-(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vsubq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return ret;
}

inline simd<double, m128d_abi> operator*(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vmulq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return ret;
}

inline simd<double, m128d_abi> operator/(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi> ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vdivq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return ret;
}

inline simd<double, m128d_abi>& operator+=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_.val[i] = vaddq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return a;
}

inline simd<double, m128d_abi>& operator-=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_.val[i] = vsubq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return a;
}

inline simd<double, m128d_abi>& operator*=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_.val[i] = vmulq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return a;
}

inline simd<double, m128d_abi>& operator/=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  for (int i = 0; i < 2; ++i)
    a.__s_.__storage_.val[i] = vdivq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]);
  return a;
}

inline simd<double, m128d_abi>::mask_type operator==(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vreinterpretq_f64_u64(vceqq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]));
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator!=(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vreinterpretq_f64_u64(vreinterpretq_u64_u32(
        vmvnq_u32(vreinterpretq_u32_u64(vceqq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i])))));
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator>=(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vreinterpretq_f64_u64(vcgeq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]));
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator<=(const simd<double, m128d_abi>& a,
                                                     const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vreinterpretq_f64_u64(vcleq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]));
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator>(const simd<double, m128d_abi>& a,
                                                    const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vreinterpretq_f64_u64(vcgtq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]));
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator<(const simd<double, m128d_abi>& a,
                                                    const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  for (int i = 0; i < 2; ++i)
    ret.__s_.__storage_.val[i] = vreinterpretq_f64_u64(vcltq_f64(a.__s_.__storage_.val[i], b.__s_.__storage_.val[i]));
  return ret;
}

inline __simd_storage<float, m128_abi>::__simd_storage(const __simd_storage<double, m128d_abi>& other) {
  __storage_ = vcombine_f32(vcvt_f32_f64(other.__storage_.val[0]), vcvt_f32_f64(other.__storage_.val[1]));
}

namespace simd_abi {
template <typename T>
struct athena_native {};
template <>
struct athena_native<float> {
  using type = m128_abi;
};
template <>
struct athena_native<double> {
  using type = m128d_abi;
};
} // namespace simd_abi

} // namespace athena::_simd
