// -*- C++ -*-
//===------------------------------- simd ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#pragma once

/*
    experimental/simd synopsis

namespace std::experimental {

inline namespace parallelism_v2 {

namespace simd_abi {

struct scalar {};
template <int N> struct fixed_size {};
template <typename T> inline constexpr int max_fixed_size = implementation-defined;
template <typename T> using compatible = implementation-defined;
template <typename T> using native = implementation-defined;

} // simd_abi

struct element_aligned_tag {};
struct vector_aligned_tag {};
template <size_t> struct overaligned_tag {};
inline constexpr element_aligned_tag element_aligned{};
inline constexpr vector_aligned_tag vector_aligned{};
template <size_t N> inline constexpr overaligned_tag<N> overaligned{};

// traits [simd.traits]
template <class T> struct is_abi_tag;
template <class T> inline constexpr bool is_abi_tag_v = is_abi_tag<T>::value;

template <class T> struct is_simd;
template <class T> inline constexpr bool is_simd_v = is_simd<T>::value;

template <class T> struct is_simd_mask;
template <class T> inline constexpr bool is_simd_mask_v = is_simd_mask<T>::value;

template <class T> struct is_simd_flag_type;
template <class T> inline constexpr bool is_simd_flag_type_v = is_simd_flag_type<T>::value;

template <class T, size_t N> struct abi_for_size { using type = see below; };
template <class T, size_t N> using abi_for_size_t = typename abi_for_size<T, N>::type;

template <class T, class Abi = simd_abi::compatible<T>> struct simd_size;
template <class T, class Abi = simd_abi::compatible<T>>
inline constexpr size_t simd_size_v = simd_size<T, Abi>::value;

template <class T, class U = typename T::value_type> struct memory_alignment;
template <class T, class U = typename T::value_type>
inline constexpr size_t memory_alignment_v = memory_alignment<T, U>::value;

// class template simd [simd.class]
template <class T, class Abi = simd_abi::compatible<T>> class simd;
template <class T> using native_simd = simd<T, simd_abi::native<T>>;
template <class T, int N> using fixed_size_simd = simd<T, simd_abi::fixed_size<N>>;

// class template simd_mask [simd.mask.class]
template <class T, class Abi = simd_abi::compatible<T>> class simd_mask;
template <class T> using native_simd_mask = simd_mask<T, simd_abi::native<T>>;
template <class T, int N> using fixed_size_simd_mask = simd_mask<T, simd_abi::fixed_size<N>>;

// casts [simd.casts]
template <class T, class U, class Abi> see below simd_cast(const simd<U, Abi>&);
template <class T, class U, class Abi> see below static_simd_cast(const simd<U, Abi>&);

template <class T, class Abi>
fixed_size_simd<T, simd_size_v<T, Abi>> to_fixed_size(const simd<T, Abi>&) noexcept;
template <class T, class Abi>
fixed_size_simd_mask<T, simd_size_v<T, Abi>> to_fixed_size(const simd_mask<T, Abi>&) noexcept;
template <class T, size_t N> native_simd<T> to_native(const fixed_size_simd<T, N>&) noexcept;
template <class T, size_t N>
native_simd_mask<T> to_native(const fixed_size_simd_mask<T, N>> &) noexcept;
template <class T, size_t N> simd<T> to_compatible(const fixed_size_simd<T, N>&) noexcept;
template <class T, size_t N> simd_mask<T> to_compatible(const fixed_size_simd_mask<T, N>&) noexcept;

template <size_t... Sizes, class T, class Abi>
tuple<simd<T, abi_for_size_t<Sizes>>...> split(const simd<T, Abi>&);
template <size_t... Sizes, class T, class Abi>
tuple<simd_mask<T, abi_for_size_t<Sizes>>...> split(const simd_mask<T, Abi>&);
template <class V, class Abi>
array<V, simd_size_v<typename V::value_type, Abi> / V::size()> split(
const simd<typename V::value_type, Abi>&);
template <class V, class Abi>
array<V, simd_size_v<typename V::value_type, Abi> / V::size()> split(
const simd_mask<typename V::value_type, Abi>&);

template <class T, class... Abis>
simd<T, abi_for_size_t<T, (simd_size_v<T, Abis> + ...)>> concat(const simd<T, Abis>&...);
template <class T, class... Abis>
simd_mask<T, abi_for_size_t<T, (simd_size_v<T, Abis> + ...)>> concat(const simd_mask<T, Abis>&...);

// reductions [simd.mask.reductions]
template <class T, class Abi> bool all_of(const simd_mask<T, Abi>&) noexcept;
template <class T, class Abi> bool any_of(const simd_mask<T, Abi>&) noexcept;
template <class T, class Abi> bool none_of(const simd_mask<T, Abi>&) noexcept;
template <class T, class Abi> bool some_of(const simd_mask<T, Abi>&) noexcept;
template <class T, class Abi> int popcount(const simd_mask<T, Abi>&) noexcept;
template <class T, class Abi> int find_first_set(const simd_mask<T, Abi>&);
template <class T, class Abi> int find_last_set(const simd_mask<T, Abi>&);

bool all_of(see below) noexcept;
bool any_of(see below) noexcept;
bool none_of(see below) noexcept;
bool some_of(see below) noexcept;
int popcount(see below) noexcept;
int find_first_set(see below) noexcept;
int find_last_set(see below) noexcept;

// masked assignment [simd.whereexpr]
template <class M, class T> class const_where_expression;
template <class M, class T> class where_expression;

// masked assignment [simd.mask.where]
template <class T> struct nodeduce { using type = T; }; // exposition only

template <class T> using nodeduce_t = typename nodeduce<T>::type; // exposition only

template <class T, class Abi>
where_expression<simd_mask<T, Abi>, simd<T, Abi>>
where(const typename simd<T, Abi>::mask_type&, simd<T, Abi>&) noexcept;

template <class T, class Abi>
const_where_expression<simd_mask<T, Abi>, const simd<T, Abi>>
where(const typename simd<T, Abi>::mask_type&, const simd<T, Abi>&) noexcept;

template <class T, class Abi>
where_expression<simd_mask<T, Abi>, simd_mask<T, Abi>>
where(const nodeduce_t<simd_mask<T, Abi>>&, simd_mask<T, Abi>&) noexcept;

template <class T, class Abi>
const_where_expression<simd_mask<T, Abi>, const simd_mask<T, Abi>>
where(const nodeduce_t<simd_mask<T, Abi>>&, const simd_mask<T, Abi>&) noexcept;

template <class T> where_expression<bool, T> where(see below k, T& d) noexcept;

template <class T>
const_where_expression<bool, const T> where(see below k, const T& d) noexcept;

// reductions [simd.reductions]
template <class T, class Abi, class BinaryOperation = std::plus<>>
T reduce(const simd<T, Abi>&, BinaryOperation = BinaryOperation());

template <class M, class V, class BinaryOperation>
typename V::value_type reduce(const const_where_expression<M, V>& x,
typename V::value_type neutral_element, BinaryOperation binary_op);

template <class M, class V>
typename V::value_type reduce(const const_where_expression<M, V>& x, plus<> binary_op = plus<>());

template <class M, class V>
typename V::value_type reduce(const const_where_expression<M, V>& x, multiplies<> binary_op);

template <class M, class V>
typename V::value_type reduce(const const_where_expression<M, V>& x, bit_and<> binary_op);

template <class M, class V>
typename V::value_type reduce(const const_where_expression<M, V>& x, bit_or<> binary_op);

template <class M, class V>
typename V::value_type reduce(const const_where_expression<M, V>& x, bit_xor<> binary_op);

template <class T, class Abi> T hmin(const simd<T, Abi>&);
template <class M, class V> T hmin(const const_where_expression<M, V>&);
template <class T, class Abi> T hmax(const simd<T, Abi>&);
template <class M, class V> T hmax(const const_where_expression<M, V>&);

// algorithms [simd.alg]
template <class T, class Abi> simd<T, Abi> min(const simd<T, Abi>&, const simd<T, Abi>&) noexcept;

template <class T, class Abi> simd<T, Abi> max(const simd<T, Abi>&, const simd<T, Abi>&) noexcept;

template <class T, class Abi>
std::pair<simd<T, Abi>, simd<T, Abi>> minmax(const simd<T, Abi>&, const simd<T, Abi>&) noexcept;

template <class T, class Abi>
simd<T, Abi> clamp(const simd<T, Abi>& v, const simd<T, Abi>& lo, const simd<T, Abi>& hi);

// [simd.whereexpr]
template <class M, class T>
class const_where_expression {
  const M& mask; // exposition only
  T& data; // exposition only
public:
  const_where_expression(const const_where_expression&) = delete;
  const_where_expression& operator=(const const_where_expression&) = delete;
  remove_const_t<T> operator-() const &&;
  template <class U, class Flags> void copy_to(U* mem, Flags f) const &&;
};

template <class M, class T>
class where_expression : public const_where_expression<M, T> {
public:
  where_expression(const where_expression&) = delete;
  where_expression& operator=(const where_expression&) = delete;
  template <class U> void operator=(U&& x);
  template <class U> void operator+=(U&& x);
  template <class U> void operator-=(U&& x);
  template <class U> void operator*=(U&& x);
  template <class U> void operator/=(U&& x);
  template <class U> void operator%=(U&& x);
  template <class U> void operator&=(U&& x);
  template <class U> void operator|=(U&& x);
  template <class U> void operator^=(U&& x);
  template <class U> void operator<<=(U&& x);
  template <class U> void operator>>=(U&& x);
  void operator++();
  void operator++(int);
  void operator--();
  void operator--(int);
  template <class U, class Flags> void copy_from(const U* mem, Flags);
};

// [simd.class]
template <class T, class Abi> class simd {
public:
  using value_type = T;
  using reference = see below;
  using mask_type = simd_mask<T, Abi>;

  using abi_type = Abi;
  static constexpr size_t size() noexcept;
  simd() = default;

  // implicit type conversion constructor
  template <class U> simd(const simd<U, simd_abi::fixed_size<size()>>&);

  // implicit broadcast constructor (see below for constraints)
  template <class U> simd(U&& value);

  // generator constructor (see below for constraints)
  template <class G> explicit simd(G&& gen);

  // load constructor
  template <class U, class Flags> simd(const U* mem, Flags f);

  // loads [simd.load]
  template <class U, class Flags> void copy_from(const U* mem, Flags f);

  // stores [simd.store]
  template <class U, class Flags> void copy_to(U* mem, Flags f) const;

  // scalar access [simd.subscr]
  reference operator[](size_t);
  value_type operator[](size_t) const;

  // unary operators [simd.unary]
  simd& operator++();
  simd operator++(int);
  simd& operator--();
  simd operator--(int);
  mask_type operator!() const;
  simd operator~() const; // see below
  simd operator+() const;
  simd operator-() const;

  // binary operators [simd.binary]
  friend simd operator+ (const simd&, const simd&);
  friend simd operator- (const simd&, const simd&);
  friend simd operator* (const simd&, const simd&);
  friend simd operator/ (const simd&, const simd&);
  friend simd operator% (const simd&, const simd&);
  friend simd operator& (const simd&, const simd&);
  friend simd operator| (const simd&, const simd&);
  friend simd operator^ (const simd&, const simd&);
  friend simd operator<<(const simd&, const simd&);
  friend simd operator>>(const simd&, const simd&);
  friend simd operator<<(const simd&, int);
  friend simd operator>>(const simd&, int);

  // compound assignment [simd.cassign]
  friend simd& operator+= (simd&, const simd&);
  friend simd& operator-= (simd&, const simd&);
  friend simd& operator*= (simd&, const simd&);
  friend simd& operator/= (simd&, const simd&);
  friend simd& operator%= (simd&, const simd&);

  friend simd& operator&= (simd&, const simd&);
  friend simd& operator|= (simd&, const simd&);
  friend simd& operator^= (simd&, const simd&);
  friend simd& operator<<=(simd&, const simd&);
  friend simd& operator>>=(simd&, const simd&);
  friend simd& operator<<=(simd&, int);
  friend simd& operator>>=(simd&, int);

  // compares [simd.comparison]
  friend mask_type operator==(const simd&, const simd&);
  friend mask_type operator!=(const simd&, const simd&);
  friend mask_type operator>=(const simd&, const simd&);
  friend mask_type operator<=(const simd&, const simd&);
  friend mask_type operator> (const simd&, const simd&);
  friend mask_type operator< (const simd&, const simd&);
};

// [simd.math]
template <class Abi> using scharv = simd<signed char, Abi>; // exposition only
template <class Abi> using shortv = simd<short, Abi>; // exposition only
template <class Abi> using intv = simd<int, Abi>; // exposition only
template <class Abi> using longv = simd<long int, Abi>; // exposition only
template <class Abi> using llongv = simd<long long int, Abi>; // exposition only
template <class Abi> using floatv = simd<float, Abi>; // exposition only
template <class Abi> using doublev = simd<double, Abi>; // exposition only
template <class Abi> using ldoublev = simd<long double, Abi>; // exposition only
template <class T, class V> using samesize = fixed_size_simd<T, V::size()>; // exposition only

template <class Abi> floatv<Abi> acos(floatv<Abi> x);
template <class Abi> doublev<Abi> acos(doublev<Abi> x);
template <class Abi> ldoublev<Abi> acos(ldoublev<Abi> x);

template <class Abi> floatv<Abi> asin(floatv<Abi> x);
template <class Abi> doublev<Abi> asin(doublev<Abi> x);
template <class Abi> ldoublev<Abi> asin(ldoublev<Abi> x);

template <class Abi> floatv<Abi> atan(floatv<Abi> x);
template <class Abi> doublev<Abi> atan(doublev<Abi> x);
template <class Abi> ldoublev<Abi> atan(ldoublev<Abi> x);

template <class Abi> floatv<Abi> atan2(floatv<Abi> y, floatv<Abi> x);
template <class Abi> doublev<Abi> atan2(doublev<Abi> y, doublev<Abi> x);
template <class Abi> ldoublev<Abi> atan2(ldoublev<Abi> y, ldoublev<Abi> x);

template <class Abi> floatv<Abi> cos(floatv<Abi> x);
template <class Abi> doublev<Abi> cos(doublev<Abi> x);
template <class Abi> ldoublev<Abi> cos(ldoublev<Abi> x);

template <class Abi> floatv<Abi> sin(floatv<Abi> x);
template <class Abi> doublev<Abi> sin(doublev<Abi> x);
template <class Abi> ldoublev<Abi> sin(ldoublev<Abi> x);

template <class Abi> floatv<Abi> tan(floatv<Abi> x);
template <class Abi> doublev<Abi> tan(doublev<Abi> x);
template <class Abi> ldoublev<Abi> tan(ldoublev<Abi> x);

template <class Abi> floatv<Abi> acosh(floatv<Abi> x);
template <class Abi> doublev<Abi> acosh(doublev<Abi> x);
template <class Abi> ldoublev<Abi> acosh(ldoublev<Abi> x);

template <class Abi> floatv<Abi> asinh(floatv<Abi> x);
template <class Abi> doublev<Abi> asinh(doublev<Abi> x);
template <class Abi> ldoublev<Abi> asinh(ldoublev<Abi> x);

template <class Abi> floatv<Abi> atanh(floatv<Abi> x);
template <class Abi> doublev<Abi> atanh(doublev<Abi> x);
template <class Abi> ldoublev<Abi> atanh(ldoublev<Abi> x);

template <class Abi> floatv<Abi> cosh(floatv<Abi> x);
template <class Abi> doublev<Abi> cosh(doublev<Abi> x);
template <class Abi> ldoublev<Abi> cosh(ldoublev<Abi> x);

template <class Abi> floatv<Abi> sinh(floatv<Abi> x);
template <class Abi> doublev<Abi> sinh(doublev<Abi> x);
template <class Abi> ldoublev<Abi> sinh(ldoublev<Abi> x);

template <class Abi> floatv<Abi> tanh(floatv<Abi> x);
template <class Abi> doublev<Abi> tanh(doublev<Abi> x);
template <class Abi> ldoublev<Abi> tanh(ldoublev<Abi> x);

template <class Abi> floatv<Abi> exp(floatv<Abi> x);
template <class Abi> doublev<Abi> exp(doublev<Abi> x);
template <class Abi> ldoublev<Abi> exp(ldoublev<Abi> x);

template <class Abi> floatv<Abi> exp2(floatv<Abi> x);
template <class Abi> doublev<Abi> exp2(doublev<Abi> x);
template <class Abi> ldoublev<Abi> exp2(ldoublev<Abi> x);

template <class Abi> floatv<Abi> expm1(floatv<Abi> x);
template <class Abi> doublev<Abi> expm1(doublev<Abi> x);
template <class Abi> ldoublev<Abi> expm1(ldoublev<Abi> x);

template <class Abi> floatv<Abi> frexp(floatv<Abi> value, samesize<int, floatv<Abi>>* exp);
template <class Abi> doublev<Abi> frexp(doublev<Abi> value, samesize<int, doublev<Abi>>* exp);
template <class Abi> ldoublev<Abi> frexp(ldoublev<Abi> value, samesize<int, ldoublev<Abi>>* exp);

template <class Abi> samesize<int, floatv<Abi>> ilogb(floatv<Abi> x);
template <class Abi> samesize<int, doublev<Abi>> ilogb(doublev<Abi> x);
template <class Abi> samesize<int, ldoublev<Abi>> ilogb(ldoublev<Abi> x);

template <class Abi> floatv<Abi> ldexp(floatv<Abi> x, samesize<int, floatv<Abi>> exp);
template <class Abi> doublev<Abi> ldexp(doublev<Abi> x, samesize<int, doublev<Abi>> exp);
template <class Abi> ldoublev<Abi> ldexp(ldoublev<Abi> x, samesize<int, ldoublev<Abi>> exp);

template <class Abi> floatv<Abi> log(floatv<Abi> x);
template <class Abi> doublev<Abi> log(doublev<Abi> x);
template <class Abi> ldoublev<Abi> log(ldoublev<Abi> x);

template <class Abi> floatv<Abi> log10(floatv<Abi> x);
template <class Abi> doublev<Abi> log10(doublev<Abi> x);
template <class Abi> ldoublev<Abi> log10(ldoublev<Abi> x);

template <class Abi> floatv<Abi> log1p(floatv<Abi> x);
template <class Abi> doublev<Abi> log1p(doublev<Abi> x);
template <class Abi> ldoublev<Abi> log1p(ldoublev<Abi> x);

template <class Abi> floatv<Abi> log2(floatv<Abi> x);
template <class Abi> doublev<Abi> log2(doublev<Abi> x);
template <class Abi> ldoublev<Abi> log2(ldoublev<Abi> x);

template <class Abi> floatv<Abi> logb(floatv<Abi> x);
template <class Abi> doublev<Abi> logb(doublev<Abi> x);
template <class Abi> ldoublev<Abi> logb(ldoublev<Abi> x);

template <class Abi> floatv<Abi> modf(floatv<Abi> value, floatv<Abi>* iptr);
template <class Abi> doublev<Abi> modf(doublev<Abi> value, doublev<Abi>* iptr);
template <class Abi> ldoublev<Abi> modf(ldoublev<Abi> value, ldoublev<Abi>* iptr);

template <class Abi> floatv<Abi> scalbn(floatv<Abi> x, samesize<int, floatv<Abi>> n);
template <class Abi> doublev<Abi> scalbn(doublev<Abi> x, samesize<int, doublev<Abi>> n);
template <class Abi> ldoublev<Abi> scalbn(ldoublev<Abi> x, samesize<int, ldoublev<Abi>> n);
template <class Abi> floatv<Abi> scalbln(floatv<Abi> x, samesize<long int, floatv<Abi>> n);
template <class Abi> doublev<Abi> scalbln(doublev<Abi> x, samesize<long int, doublev<Abi>> n);
template <class Abi> ldoublev<Abi> scalbln(ldoublev<Abi> x, samesize<long int, ldoublev<Abi>> n);

template <class Abi> floatv<Abi> cbrt(floatv<Abi> x);
template <class Abi> doublev<Abi> cbrt(doublev<Abi> x);
template <class Abi> ldoublev<Abi> cbrt(ldoublev<Abi> x);

template <class Abi> scharv<Abi> abs(scharv<Abi> j);
template <class Abi> shortv<Abi> abs(shortv<Abi> j);
template <class Abi> intv<Abi> abs(intv<Abi> j);
template <class Abi> longv<Abi> abs(longv<Abi> j);
template <class Abi> llongv<Abi> abs(llongv<Abi> j);
template <class Abi> floatv<Abi> abs(floatv<Abi> j);
template <class Abi> doublev<Abi> abs(doublev<Abi> j);
template <class Abi> ldoublev<Abi> abs(ldoublev<Abi> j);

template <class Abi> floatv<Abi> hypot(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> hypot(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> hypot(doublev<Abi> x, doublev<Abi> y);
template <class Abi> floatv<Abi> hypot(floatv<Abi> x, floatv<Abi> y, floatv<Abi> z);
template <class Abi> doublev<Abi> hypot(doublev<Abi> x, doublev<Abi> y, doublev<Abi> z);
template <class Abi> ldoublev<Abi> hypot(ldoublev<Abi> x, ldoublev<Abi> y, ldoublev<Abi> z);

template <class Abi> floatv<Abi> pow(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> pow(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> pow(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> sqrt(floatv<Abi> x);
template <class Abi> doublev<Abi> sqrt(doublev<Abi> x);
template <class Abi> ldoublev<Abi> sqrt(ldoublev<Abi> x);

template <class Abi> floatv<Abi> erf(floatv<Abi> x);
template <class Abi> doublev<Abi> erf(doublev<Abi> x);
template <class Abi> ldoublev<Abi> erf(ldoublev<Abi> x);
template <class Abi> floatv<Abi> erfc(floatv<Abi> x);
template <class Abi> doublev<Abi> erfc(doublev<Abi> x);
template <class Abi> ldoublev<Abi> erfc(ldoublev<Abi> x);

template <class Abi> floatv<Abi> lgamma(floatv<Abi> x);
template <class Abi> doublev<Abi> lgamma(doublev<Abi> x);
template <class Abi> ldoublev<Abi> lgamma(ldoublev<Abi> x);

template <class Abi> floatv<Abi> tgamma(floatv<Abi> x);
template <class Abi> doublev<Abi> tgamma(doublev<Abi> x);
template <class Abi> ldoublev<Abi> tgamma(ldoublev<Abi> x);

template <class Abi> floatv<Abi> ceil(floatv<Abi> x);
template <class Abi> doublev<Abi> ceil(doublev<Abi> x);
template <class Abi> ldoublev<Abi> ceil(ldoublev<Abi> x);

template <class Abi> floatv<Abi> floor(floatv<Abi> x);
template <class Abi> doublev<Abi> floor(doublev<Abi> x);
template <class Abi> ldoublev<Abi> floor(ldoublev<Abi> x);

template <class Abi> floatv<Abi> nearbyint(floatv<Abi> x);
template <class Abi> doublev<Abi> nearbyint(doublev<Abi> x);
template <class Abi> ldoublev<Abi> nearbyint(ldoublev<Abi> x);

template <class Abi> floatv<Abi> rint(floatv<Abi> x);
template <class Abi> doublev<Abi> rint(doublev<Abi> x);
template <class Abi> ldoublev<Abi> rint(ldoublev<Abi> x);

template <class Abi> samesize<long int, floatv<Abi>> lrint(floatv<Abi> x);
template <class Abi> samesize<long int, doublev<Abi>> lrint(doublev<Abi> x);
template <class Abi> samesize<long int, ldoublev<Abi>> lrint(ldoublev<Abi> x);
template <class Abi> samesize<long long int, floatv<Abi>> llrint(floatv<Abi> x);
template <class Abi> samesize<long long int, doublev<Abi>> llrint(doublev<Abi> x);
template <class Abi> samesize<long long int, ldoublev<Abi>> llrint(ldoublev<Abi> x);

template <class Abi> floatv<Abi> round(floatv<Abi> x);
template <class Abi> doublev<Abi> round(doublev<Abi> x);
template <class Abi> ldoublev<Abi> round(ldoublev<Abi> x);
template <class Abi> samesize<long int, floatv<Abi>> lround(floatv<Abi> x);
template <class Abi> samesize<long int, doublev<Abi>> lround(doublev<Abi> x);
template <class Abi> samesize<long int, ldoublev<Abi>> lround(ldoublev<Abi> x);
template <class Abi> samesize<long long int, floatv<Abi>> llround(floatv<Abi> x);
template <class Abi> samesize<long long int, doublev<Abi>> llround(doublev<Abi> x);
template <class Abi> samesize<long long int, ldoublev<Abi>> llround(ldoublev<Abi> x);

template <class Abi> floatv<Abi> trunc(floatv<Abi> x);
template <class Abi> doublev<Abi> trunc(doublev<Abi> x);
template <class Abi> ldoublev<Abi> trunc(ldoublev<Abi> x);

template <class Abi> floatv<Abi> fmod(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> fmod(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> fmod(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> remainder(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> remainder(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> remainder(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> remquo(floatv<Abi> x, floatv<Abi> y, samesize<int, floatv<Abi>>* quo);
template <class Abi> doublev<Abi> remquo(doublev<Abi> x, doublev<Abi> y, samesize<int, doublev<Abi>>* quo);
template <class Abi> ldoublev<Abi> remquo(ldoublev<Abi> x, ldoublev<Abi> y, samesize<int, ldoublev<Abi>>* quo);

template <class Abi> floatv<Abi> copysign(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> copysign(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> copysign(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> doublev<Abi> nan(const char* tagp);
template <class Abi> floatv<Abi> nanf(const char* tagp);
template <class Abi> ldoublev<Abi> nanl(const char* tagp);

template <class Abi> floatv<Abi> nextafter(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> nextafter(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> nextafter(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> nexttoward(floatv<Abi> x, ldoublev<Abi> y);
template <class Abi> doublev<Abi> nexttoward(doublev<Abi> x, ldoublev<Abi> y);
template <class Abi> ldoublev<Abi> nexttoward(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> fdim(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> fdim(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> fdim(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> fmax(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> fmax(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> fmax(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> fmin(floatv<Abi> x, floatv<Abi> y);
template <class Abi> doublev<Abi> fmin(doublev<Abi> x, doublev<Abi> y);
template <class Abi> ldoublev<Abi> fmin(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> floatv<Abi> fma(floatv<Abi> x, floatv<Abi> y, floatv<Abi> z);
template <class Abi> doublev<Abi> fma(doublev<Abi> x, doublev<Abi> y, doublev<Abi> z);
template <class Abi> ldoublev<Abi> fma(ldoublev<Abi> x, ldoublev<Abi> y, ldoublev<Abi> z);

template <class Abi> samesize<int, floatv<Abi>> fpclassify(floatv<Abi> x);
template <class Abi> samesize<int, doublev<Abi>> fpclassify(doublev<Abi> x);
template <class Abi> samesize<int, ldoublev<Abi>> fpclassify(ldoublev<Abi> x);

template <class Abi> simd_mask<float, Abi> isfinite(floatv<Abi> x);
template <class Abi> simd_mask<double, Abi> isfinite(doublev<Abi> x);
template <class Abi> simd_mask<long double, Abi> isfinite(ldoublev<Abi> x);

template <class Abi> simd_mask<float, Abi> isinf(floatv<Abi> x);
template <class Abi> simd_mask<double, Abi> isinf(doublev<Abi> x);
template <class Abi> simd_mask<long double, Abi> isinf(ldoublev<Abi> x);

template <class Abi> simd_mask<float, Abi> isnan(floatv<Abi> x);
template <class Abi> simd_mask<double, Abi> isnan(doublev<Abi> x);
template <class Abi> simd_mask<long double, Abi> isnan(ldoublev<Abi> x);

template <class Abi> simd_mask<float, Abi> isnormal(floatv<Abi> x);
template <class Abi> simd_mask<double, Abi> isnormal(doublev<Abi> x);
template <class Abi> simd_mask<long double, Abi> isnormal(ldoublev<Abi> x);

template <class Abi> simd_mask<float, Abi> signbit(floatv<Abi> x);
template <class Abi> simd_mask<double, Abi> signbit(doublev<Abi> x);
template <class Abi> simd_mask<long double, Abi> signbit(ldoublev<Abi> x);

template <class Abi> simd_mask<float, Abi> isgreater(floatv<Abi> x, floatv<Abi> y);
template <class Abi> simd_mask<double, Abi> isgreater(doublev<Abi> x, doublev<Abi> y);
template <class Abi> simd_mask<long double, Abi> isgreater(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> simd_mask<float, Abi> isgreaterequal(floatv<Abi> x, floatv<Abi> y);
template <class Abi> simd_mask<double, Abi> isgreaterequal(doublev<Abi> x, doublev<Abi> y);
template <class Abi> simd_mask<long double, Abi> isgreaterequal(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> simd_mask<float, Abi> isless(floatv<Abi> x, floatv<Abi> y);
template <class Abi> simd_mask<double, Abi> isless(doublev<Abi> x, doublev<Abi> y);
template <class Abi> simd_mask<long double, Abi> isless(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> simd_mask<float, Abi> islessequal(floatv<Abi> x, floatv<Abi> y);
template <class Abi> simd_mask<double, Abi> islessequal(doublev<Abi> x, doublev<Abi> y);
template <class Abi> simd_mask<long double, Abi> islessequal(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> simd_mask<float, Abi> islessgreater(floatv<Abi> x, floatv<Abi> y);
template <class Abi> simd_mask<double, Abi> islessgreater(doublev<Abi> x, doublev<Abi> y);
template <class Abi> simd_mask<long double, Abi> islessgreater(ldoublev<Abi> x, ldoublev<Abi> y);

template <class Abi> simd_mask<float, Abi> isunordered(floatv<Abi> x, floatv<Abi> y);
template <class Abi> simd_mask<double, Abi> isunordered(doublev<Abi> x, doublev<Abi> y);
template <class Abi> simd_mask<long double, Abi> isunordered(ldoublev<Abi> x, ldoublev<Abi> y);

template <class V> struct simd_div_t { V quot, rem; };
template <class Abi> simd_div_t<scharv<Abi>> div(scharv<Abi> numer, scharv<Abi> denom);
template <class Abi> simd_div_t<shortv<Abi>> div(shortv<Abi> numer, shortv<Abi> denom);
template <class Abi> simd_div_t<intv<Abi>> div(intv<Abi> numer, intv<Abi> denom);
template <class Abi> simd_div_t<longv<Abi>> div(longv<Abi> numer, longv<Abi> denom);
template <class Abi> simd_div_t<llongv<Abi>> div(llongv<Abi> numer, llongv<Abi> denom);

// [simd.mask.class]
template <class T, class Abi>
class simd_mask {
public:
  using value_type = bool;
  using reference = see below;
  using simd_type = simd<T, Abi>;
  using abi_type = Abi;
  static constexpr size_t size() noexcept;
  simd_mask() = default;

  // broadcast constructor
  explicit simd_mask(value_type) noexcept;

  // implicit type conversion constructor
  template <class U> simd_mask(const simd_mask<U, simd_abi::fixed_size<size()>>&) noexcept;

  // load constructor
  template <class Flags> simd_mask(const value_type* mem, Flags);

  // loads [simd.mask.copy]
  template <class Flags> void copy_from(const value_type* mem, Flags);
  template <class Flags> void copy_to(value_type* mem, Flags) const;

  // scalar access [simd.mask.subscr]
  reference operator[](size_t);
  value_type operator[](size_t) const;

  // unary operators [simd.mask.unary]
  simd_mask operator!() const noexcept;

  // simd_mask binary operators [simd.mask.binary]
  friend simd_mask operator&&(const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator||(const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator& (const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator| (const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator^ (const simd_mask&, const simd_mask&) noexcept;

  // simd_mask compound assignment [simd.mask.cassign]
  friend simd_mask& operator&=(simd_mask&, const simd_mask&) noexcept;
  friend simd_mask& operator|=(simd_mask&, const simd_mask&) noexcept;
  friend simd_mask& operator^=(simd_mask&, const simd_mask&) noexcept;

  // simd_mask compares [simd.mask.comparison]
  friend simd_mask operator==(const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator!=(const simd_mask&, const simd_mask&) noexcept;
};

} // parallelism_v2
} // std::experimental

*/

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <bitset>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

namespace athena::_simd {

enum class _StorageKind {
  _Scalar,
  _Array,
};

template <_StorageKind __kind, int _Np>
struct __simd_abi {};

template <class _Tp, class _Abi>
class __simd_storage {};

template <class _Tp, class _Abi>
class __simd_mask_storage {};

template <class _Tp>
class __simd_storage<_Tp, __simd_abi<_StorageKind::_Scalar, 1>> {
  _Tp __storage_;

  template <class, class>
  friend class simd;

  template <class, class>
  friend class simd_mask;

public:
  _Tp __get(size_t __index) const noexcept { return (&__storage_)[__index]; };
  void __set(size_t __index, _Tp __val) noexcept { (&__storage_)[__index] = __val; }
};

template <class _Vp, class _Tp, class _Abi>
class __simd_reference {
  static_assert(std::is_same<_Vp, _Tp>::value, "");

  template <class, class>
  friend class simd;

  template <class, class>
  friend class simd_mask;

  __simd_storage<_Tp, _Abi>* __ptr_;
  size_t __index_;

  __simd_reference(__simd_storage<_Tp, _Abi>* __ptr, size_t __index) : __ptr_(__ptr), __index_(__index) {}

  __simd_reference(const __simd_reference&) = default;

public:
  __simd_reference() = delete;
  __simd_reference& operator=(const __simd_reference& other) = delete;

  operator _Vp() const { return __ptr_->__get(__index_); }

  __simd_reference operator=(_Vp __value) && {
    __ptr_->__set(__index_, __value);
    return *this;
  }

  __simd_reference operator++() && { return std::move(*this) = __ptr_->__get(__index_) + 1; }

  _Vp operator++(int) && {
    auto __val = __ptr_->__get(__index_);
    __ptr_->__set(__index_, __val + 1);
    return __val;
  }

  __simd_reference operator--() && { return std::move(*this) = __ptr_->__get(__index_) - 1; }

  _Vp operator--(int) && {
    auto __val = __ptr_->__get(__index_);
    __ptr_->__set(__index_, __val - 1);
    return __val;
  }

  __simd_reference operator+=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) + __value; }

  __simd_reference operator-=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) - __value; }

  __simd_reference operator*=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) * __value; }

  __simd_reference operator/=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) / __value; }

  __simd_reference operator%=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) % __value; }

  __simd_reference operator>>=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) >> __value; }

  __simd_reference operator<<=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) << __value; }

  __simd_reference operator&=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) & __value; }

  __simd_reference operator|=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) | __value; }

  __simd_reference operator^=(_Vp __value) && { return std::move(*this) = __ptr_->__get(__index_) ^ __value; }

  bool operator<(_Vp __value) const { return __ptr_->__get(__index_) < __value; }

  bool operator<=(_Vp __value) const { return __ptr_->__get(__index_) <= __value; }

  bool operator>(_Vp __value) const { return __ptr_->__get(__index_) > __value; }

  bool operator>=(_Vp __value) const { return __ptr_->__get(__index_) >= __value; }

  bool operator==(_Vp __value) const { return __ptr_->__get(__index_) == __value; }

  bool operator!=(_Vp __value) const { return __ptr_->__get(__index_) != __value; }
};

template <class _Tp, class _Abi>
class __simd_mask_reference {
  template <class, class>
  friend class simd;

  template <class, class>
  friend class simd_mask;

  __simd_mask_storage<_Tp, _Abi>* __ptr_;
  size_t __index_;

  __simd_mask_reference(__simd_mask_storage<_Tp, _Abi>* __ptr, size_t __index) : __ptr_(__ptr), __index_(__index) {}

  __simd_mask_reference(const __simd_mask_reference&) = default;

public:
  __simd_mask_reference() = delete;
  __simd_mask_reference& operator=(const __simd_mask_reference&) = delete;

  operator bool() const { return __ptr_->__get(__index_); }

  __simd_mask_reference operator=(bool __value) && {
    __ptr_->__set(__index_, __value);
    return *this;
  }
};

template <class _To, class _From>
constexpr decltype(_To{std::declval<_From>()}, true) __is_non_narrowing_convertible_impl(_From) {
  return true;
}

template <class _To>
constexpr bool __is_non_narrowing_convertible_impl(...) {
  return false;
}

template <class _From, class _To>
constexpr typename std::enable_if<std::is_arithmetic<_To>::value && std::is_arithmetic<_From>::value, bool>::type
__is_non_narrowing_arithmetic_convertible() {
  return __is_non_narrowing_convertible_impl<_To>(_From{});
}

template <class _From, class _To>
constexpr typename std::enable_if<!(std::is_arithmetic<_To>::value && std::is_arithmetic<_From>::value), bool>::type
__is_non_narrowing_arithmetic_convertible() {
  return false;
}

template <class _Tp>
constexpr _Tp __variadic_sum() {
  return _Tp{};
}

template <class _Tp, class _Up, class... _Args>
constexpr _Tp __variadic_sum(_Up __first, _Args... __rest) {
  return static_cast<_Tp>(__first) + __variadic_sum<_Tp>(__rest...);
}

template <class _Tp>
struct __nodeduce {
  using type = _Tp;
};

template <class _Tp>
constexpr bool __vectorizable() {
  return std::is_arithmetic<_Tp>::value && !std::is_const<_Tp>::value && !std::is_volatile<_Tp>::value &&
         !std::is_same<_Tp, bool>::value;
}

} // namespace athena::_simd
namespace athena::_simd::simd_abi {

using scalar = __simd_abi<_StorageKind::_Scalar, 1>;

template <int _Np>
using fixed_size = __simd_abi<_StorageKind::_Array, _Np>;

template <class _Tp>
inline constexpr size_t max_fixed_size = 32;

template <class _Tp>
using compatible = fixed_size<16 / sizeof(_Tp)>;

} // namespace athena::_simd::simd_abi
namespace athena::_simd {

template <class _Tp, class _Abi = simd_abi::compatible<_Tp>>
class simd;
template <class _Tp, class _Abi = simd_abi::compatible<_Tp>>
class simd_mask;

struct element_aligned_tag {};
struct vector_aligned_tag {};
template <size_t>
struct overaligned_tag {};
inline constexpr element_aligned_tag element_aligned{};
inline constexpr vector_aligned_tag vector_aligned{};
template <size_t _Np>
inline constexpr overaligned_tag<_Np> overaligned{};

// traits [simd.traits]
template <class _Tp>
struct is_abi_tag : std::integral_constant<bool, false> {};

template <_StorageKind __kind, int _Np>
struct is_abi_tag<__simd_abi<__kind, _Np>> : std::integral_constant<bool, true> {};

template <class _Tp>
struct is_simd : std::integral_constant<bool, false> {};

template <class _Tp, class _Abi>
struct is_simd<simd<_Tp, _Abi>> : std::integral_constant<bool, true> {};

template <class _Tp>
struct is_simd_mask : std::integral_constant<bool, false> {};

template <class _Tp, class _Abi>
struct is_simd_mask<simd_mask<_Tp, _Abi>> : std::integral_constant<bool, true> {};

template <class _Tp>
struct is_simd_flag_type : std::integral_constant<bool, false> {};

template <>
struct is_simd_flag_type<element_aligned_tag> : std::integral_constant<bool, true> {};

template <>
struct is_simd_flag_type<vector_aligned_tag> : std::integral_constant<bool, true> {};

template <size_t _Align>
struct is_simd_flag_type<overaligned_tag<_Align>> : std::integral_constant<bool, true> {};

template <class _Tp>
inline constexpr bool is_abi_tag_v = is_abi_tag<_Tp>::value;
template <class _Tp>
inline constexpr bool is_simd_v = is_simd<_Tp>::value;
template <class _Tp>
inline constexpr bool is_simd_mask_v = is_simd_mask<_Tp>::value;
template <class _Tp>
inline constexpr bool is_simd_flag_type_v = is_simd_flag_type<_Tp>::value;
template <class _Tp, size_t _Np>
struct abi_for_size {
  using type = simd_abi::fixed_size<_Np>;
};
template <class _Tp, size_t _Np>
using abi_for_size_t = typename abi_for_size<_Tp, _Np>::type;

template <class _Tp, class _Abi = simd_abi::compatible<_Tp>>
struct simd_size;

template <class _Tp, _StorageKind __kind, int _Np>
struct simd_size<_Tp, __simd_abi<__kind, _Np>> : std::integral_constant<size_t, _Np> {
  static_assert(std::is_arithmetic<_Tp>::value && !std::is_same<typename std::remove_const<_Tp>::type, bool>::value,
                "Element type should be vectorizable");
};

// TODO: implement it.
template <class _Tp, class _Up = typename _Tp::value_type>
struct memory_alignment;

template <class _Tp, class _Abi = simd_abi::compatible<_Tp>>
inline constexpr size_t simd_size_v = simd_size<_Tp, _Abi>::value;

template <class _Tp, class _Up = typename _Tp::value_type>
inline constexpr size_t memory_alignment_v = memory_alignment<_Tp, _Up>::value;

// class template simd [simd.class]
template <class _Tp, int _Np>
using fixed_size_simd = simd<_Tp, simd_abi::fixed_size<_Np>>;

template <class _Tp, int _Np>
using fixed_size_simd_mask = simd_mask<_Tp, simd_abi::fixed_size<_Np>>;

// casts [simd.casts]
template <class _Tp>
struct __static_simd_cast_traits {
  template <class _Up, class _Abi>
  static simd<_Tp, _Abi> __apply(const simd<_Up, _Abi>& __v);
};

template <class _Tp, class _NewAbi>
struct __static_simd_cast_traits<simd<_Tp, _NewAbi>> {
  template <class _Up, class _Abi>
  static typename std::enable_if<simd<_Up, _Abi>::size() == simd<_Tp, _NewAbi>::size(), simd<_Tp, _NewAbi>>::type
  __apply(const simd<_Up, _Abi>& __v);
};

template <class _Tp>
struct __simd_cast_traits {
  template <class _Up, class _Abi>
  static typename std::enable_if<__is_non_narrowing_arithmetic_convertible<_Up, _Tp>(), simd<_Tp, _Abi>>::type
  __apply(const simd<_Up, _Abi>& __v);
};

template <class _Tp, class _NewAbi>
struct __simd_cast_traits<simd<_Tp, _NewAbi>> {
  template <class _Up, class _Abi>
  static typename std::enable_if<__is_non_narrowing_arithmetic_convertible<_Up, _Tp>() &&
                                     simd<_Up, _Abi>::size() == simd<_Tp, _NewAbi>::size(),
                                 simd<_Tp, _NewAbi>>::type
  __apply(const simd<_Up, _Abi>& __v);
};

template <class _Tp, class _Up, class _Abi>
auto simd_cast(const simd<_Up, _Abi>& __v) -> decltype(__simd_cast_traits<_Tp>::__apply(__v)) {
  return __simd_cast_traits<_Tp>::__apply(__v);
}

template <class _Tp, class _Up, class _Abi>
auto static_simd_cast(const simd<_Up, _Abi>& __v) -> decltype(__static_simd_cast_traits<_Tp>::__apply(__v)) {
  return __static_simd_cast_traits<_Tp>::__apply(__v);
}

template <class _Tp, class _Abi>
fixed_size_simd<_Tp, simd_size<_Tp, _Abi>::value> to_fixed_size(const simd<_Tp, _Abi>&) noexcept;

template <class _Tp, class _Abi>
fixed_size_simd_mask<_Tp, simd_size<_Tp, _Abi>::value> to_fixed_size(const simd_mask<_Tp, _Abi>&) noexcept;

template <class _Tp, size_t _Np>
simd<_Tp> to_compatible(const fixed_size_simd<_Tp, _Np>&) noexcept;

template <class _Tp, size_t _Np>
simd_mask<_Tp> to_compatible(const fixed_size_simd_mask<_Tp, _Np>&) noexcept;

template <size_t... __sizes, class _Tp, class _Abi>
tuple<simd<_Tp, abi_for_size_t<_Tp, __sizes>>...> split(const simd<_Tp, _Abi>&);

template <size_t... __sizes, class _Tp, class _Abi>
tuple<simd_mask<_Tp, abi_for_size_t<_Tp, __sizes>>...> split(const simd_mask<_Tp, _Abi>&);

template <class _SimdType, class _Abi>
array<_SimdType, simd_size<typename _SimdType::value_type, _Abi>::value / _SimdType::size()>
split(const simd<typename _SimdType::value_type, _Abi>&);

template <class _SimdType, class _Abi>
array<_SimdType, simd_size<typename _SimdType::value_type, _Abi>::value / _SimdType::size()>
split(const simd_mask<typename _SimdType::value_type, _Abi>&);

template <class _Tp, class... _Abis>
simd<_Tp, abi_for_size_t<_Tp, __variadic_sum(simd_size<_Tp, _Abis>::value...)>> concat(const simd<_Tp, _Abis>&...);

template <class _Tp, class... _Abis>
simd_mask<_Tp, abi_for_size_t<_Tp, __variadic_sum(simd_size<_Tp, _Abis>::value...)>>
concat(const simd_mask<_Tp, _Abis>&...);

// reductions [simd.mask.reductions]
template <class _Tp, class _Abi>
bool all_of(const simd_mask<_Tp, _Abi>&) noexcept;
template <class _Tp, class _Abi>
bool any_of(const simd_mask<_Tp, _Abi>&) noexcept;
template <class _Tp, class _Abi>
bool none_of(const simd_mask<_Tp, _Abi>&) noexcept;
template <class _Tp, class _Abi>
bool some_of(const simd_mask<_Tp, _Abi>&) noexcept;
template <class _Tp, class _Abi>
int popcount(const simd_mask<_Tp, _Abi>&) noexcept;
template <class _Tp, class _Abi>
int find_first_set(const simd_mask<_Tp, _Abi>&);
template <class _Tp, class _Abi>
int find_last_set(const simd_mask<_Tp, _Abi>&);
bool all_of(bool) noexcept;
bool any_of(bool) noexcept;
bool none_of(bool) noexcept;
bool some_of(bool) noexcept;
int popcount(bool) noexcept;
int find_first_set(bool) noexcept;
int find_last_set(bool) noexcept;

// masked assignment [simd.whereexpr]
template <class _MaskType, class _Tp>
class const_where_expression;
template <class _MaskType, class _Tp>
class where_expression;

// masked assignment [simd.mask.where]
template <class _Tp, class _Abi>
where_expression<simd_mask<_Tp, _Abi>, simd<_Tp, _Abi>> where(const typename simd<_Tp, _Abi>::mask_type&,
                                                              simd<_Tp, _Abi>&) noexcept;

template <class _Tp, class _Abi>
const_where_expression<simd_mask<_Tp, _Abi>, const simd<_Tp, _Abi>> where(const typename simd<_Tp, _Abi>::mask_type&,
                                                                          const simd<_Tp, _Abi>&) noexcept;

template <class _Tp, class _Abi>
where_expression<simd_mask<_Tp, _Abi>, simd_mask<_Tp, _Abi>>
where(const typename __nodeduce<simd_mask<_Tp, _Abi>>::type&, simd_mask<_Tp, _Abi>&) noexcept;

template <class _Tp, class _Abi>
const_where_expression<simd_mask<_Tp, _Abi>, const simd_mask<_Tp, _Abi>>
where(const typename __nodeduce<simd_mask<_Tp, _Abi>>::type&, const simd_mask<_Tp, _Abi>&) noexcept;

template <class _Tp>
where_expression<bool, _Tp> where(bool, _Tp&) noexcept;

template <class _Tp>
const_where_expression<bool, const _Tp> where(bool, const _Tp&) noexcept;

// reductions [simd.reductions]
template <class _Tp, class _Abi, class _BinaryOp = std::plus<_Tp>>
_Tp reduce(const simd<_Tp, _Abi>&, _BinaryOp = _BinaryOp());

template <class _MaskType, class _SimdType, class _BinaryOp>
typename _SimdType::value_type reduce(const const_where_expression<_MaskType, _SimdType>&,
                                      typename _SimdType::value_type neutral_element, _BinaryOp binary_op);

template <class _MaskType, class _SimdType>
typename _SimdType::value_type reduce(const const_where_expression<_MaskType, _SimdType>&,
                                      plus<typename _SimdType::value_type> binary_op = {});

template <class _MaskType, class _SimdType>
typename _SimdType::value_type reduce(const const_where_expression<_MaskType, _SimdType>&,
                                      multiplies<typename _SimdType::value_type> binary_op);

template <class _MaskType, class _SimdType>
typename _SimdType::value_type reduce(const const_where_expression<_MaskType, _SimdType>&,
                                      bit_and<typename _SimdType::value_type> binary_op);

template <class _MaskType, class _SimdType>
typename _SimdType::value_type reduce(const const_where_expression<_MaskType, _SimdType>&,
                                      bit_or<typename _SimdType::value_type> binary_op);

template <class _MaskType, class _SimdType>
typename _SimdType::value_type reduce(const const_where_expression<_MaskType, _SimdType>&,
                                      bit_xor<typename _SimdType::value_type> binary_op);

template <class _Tp, class _Abi>
_Tp hmin(const simd<_Tp, _Abi>&);
template <class _MaskType, class _SimdType>
typename _SimdType::value_type hmin(const const_where_expression<_MaskType, _SimdType>&);
template <class _Tp, class _Abi>
_Tp hmax(const simd<_Tp, _Abi>&);
template <class _MaskType, class _SimdType>
typename _SimdType::value_type hmax(const const_where_expression<_MaskType, _SimdType>&);

// algorithms [simd.alg]
#if 0
template <class _Tp, class _Abi>
simd<_Tp, _Abi> min(const simd<_Tp, _Abi>&, const simd<_Tp, _Abi>&) noexcept;

template <class _Tp, class _Abi>
simd<_Tp, _Abi> max(const simd<_Tp, _Abi>&, const simd<_Tp, _Abi>&) noexcept;

template <class _Tp, class _Abi>
std::pair<simd<_Tp, _Abi>, simd<_Tp, _Abi>> minmax(const simd<_Tp, _Abi>&, const simd<_Tp, _Abi>&) noexcept;

template <class _Tp, class _Abi>
simd<_Tp, _Abi> clamp(const simd<_Tp, _Abi>&, const simd<_Tp, _Abi>&, const simd<_Tp, _Abi>&);
#endif

// [simd.whereexpr]
// TODO implement where expressions.
template <class _MaskType, class _Tp>
class const_where_expression {
public:
  const_where_expression(const const_where_expression&) = delete;
  const_where_expression& operator=(const const_where_expression&) = delete;
  typename remove_const<_Tp>::type operator-() const&&;
  template <class _Up, class _Flags>
  void copy_to(_Up*, _Flags) const&&;
};

template <class _MaskType, class _Tp>
class where_expression : public const_where_expression<_MaskType, _Tp> {
public:
  where_expression(const where_expression&) = delete;
  where_expression& operator=(const where_expression&) = delete;
  template <class _Up>
  void operator=(_Up&&);
  template <class _Up>
  void operator+=(_Up&&);
  template <class _Up>
  void operator-=(_Up&&);
  template <class _Up>
  void operator*=(_Up&&);
  template <class _Up>
  void operator/=(_Up&&);
  template <class _Up>
  void operator%=(_Up&&);
  template <class _Up>
  void operator&=(_Up&&);
  template <class _Up>
  void operator|=(_Up&&);
  template <class _Up>
  void operator^=(_Up&&);
  template <class _Up>
  void operator<<=(_Up&&);
  template <class _Up>
  void operator>>=(_Up&&);
  void operator++();
  void operator++(int);
  void operator--();
  void operator--(int);
  template <class _Up, class _Flags>
  void copy_from(const _Up*, _Flags);
};

template <class _Simd>
class alignas(_Simd) simd_data {
public:
  using value_type = typename _Simd::value_type;
  simd_data() = default;
  simd_data(const _Simd& s);
  simd_data(value_type v) { std::fill(std::begin(__data_), std::end(__data_), v); }
  template <typename... Ts>
  simd_data(Ts... args) : __data_{args...} {}
  value_type operator[](size_t idx) const noexcept { return __data_[idx]; }
  value_type& operator[](size_t idx) noexcept { return __data_[idx]; }
  const value_type* data() const noexcept { return __data_; }
  value_type* data() noexcept { return __data_; }
  auto begin() { return std::begin(__data_); }
  auto end() { return std::end(__data_); }
  auto begin() const { return std::begin(__data_); }
  auto end() const { return std::end(__data_); }

private:
  value_type __data_[_Simd::size()];
};

// [simd.class]
template <class _Tp, class _Abi>
class simd {
  template <class _Up, class _UAbi>
  friend class simd;

public:
  using value_type = _Tp;
  using reference = __simd_reference<_Tp, _Tp, _Abi>;
  using mask_type = simd_mask<_Tp, _Abi>;
  using abi_type = _Abi;

  simd() = default;
  simd(const simd&) = default;
  simd& operator=(const simd&) = default;

  static constexpr size_t size() noexcept { return simd_size<_Tp, _Abi>::value; }

private:
  __simd_storage<_Tp, _Abi> __s_;

  template <class _Up>
  static constexpr bool __can_broadcast() {
    return (std::is_arithmetic<_Up>::value && __is_non_narrowing_arithmetic_convertible<_Up, _Tp>()) ||
           (!std::is_arithmetic<_Up>::value && std::is_convertible<_Up, _Tp>::value) ||
           std::is_same<typename std::remove_const<_Up>::type, int>::value ||
           (std::is_same<typename std::remove_const<_Up>::type, unsigned int>::value && std::is_unsigned<_Tp>::value);
  }

  template <class _Generator, size_t... __indicies>
  static constexpr decltype(
      std::forward_as_tuple(std::declval<_Generator>()(std::integral_constant<size_t, __indicies>())...), bool())
  __can_generate(std::index_sequence<__indicies...>) {
    return !__variadic_sum<bool>(
        !__can_broadcast<decltype(std::declval<_Generator>()(std::integral_constant<size_t, __indicies>()))>()...);
  }

  template <class _Generator>
  static bool __can_generate(...) {
    return false;
  }

  template <class _Generator, size_t... __indicies>
  void __generator_init(_Generator&& __g, std::index_sequence<__indicies...>) {
    int __not_used[]{((*this)[__indicies] = __g(std::integral_constant<size_t, __indicies>()), 0)...};
    (void)__not_used;
  }

public:
  simd(const typename __simd_storage<_Tp, _Abi>::storage_type& s) : __s_(s) {}

#if 0
// implicit type conversion constructor
  template <class _Up,
            class = typename std::enable_if<
                std::is_same<_Abi, simd_abi::fixed_size<size()>>::value &&
                __is_non_narrowing_arithmetic_convertible<_Up, _Tp>()>::type>
  simd(const simd<_Up, simd_abi::fixed_size<size()>>& __v) {
    for (size_t __i = 0; __i < size(); __i++) {
      (*this)[__i] = static_cast<_Tp>(__v[__i]);
    }
  }
#endif
  // implicit type conversion constructor
  template <class _Up, class _UAbi,
            class = typename std::enable_if<
                std::is_constructible<__simd_storage<_Tp, _Abi>, __simd_storage<_Up, _UAbi>>::value>>
  simd(const simd<_Up, _UAbi>& __v) : __s_(__v.__s_) {}

#if 0
  template <class _Up, class _UAbi,
            class = typename std::enable_if_t<
                std::is_constructible_v<__simd_storage<_Tp, _Abi>,
                                        __simd_storage<_Up, _UAbi>>>>
  simd(const simd<_Up, _UAbi>& __v) : __s_(__v.__s_) {}
#endif
  // implicit broadcast constructor
#if 0
  template <class _Up,
            class = typename std::enable_if<__can_broadcast<_Up>()>::type>
  simd(_Up&& __rv) {
    auto __v = static_cast<_Tp>(__rv);
    for (size_t __i = 0; __i < size(); __i++) {
      (*this)[__i] = __v;
    }
    __s_.__broadcast(v);
  }
#endif
  simd(_Tp __rv) { __s_.__broadcast(__rv); }

  simd(_Tp a, _Tp b, _Tp c = {}, _Tp d = {}) { __s_.__set4(a, b, c, d); }

  // generator constructor
  template <class _Generator,
            int = typename std::enable_if<__can_generate<_Generator>(
              std::make_index_sequence<simd_size<_Tp, _Abi>::value>()), int>::type()>
  explicit simd(_Generator&& __g) {
    __generator_init(std::forward<_Generator>(__g), std::make_index_sequence<simd_size<_Tp, _Abi>::value>());
  }

  // load constructor
  template <class _Up, class _Flags, class = typename std::enable_if<__vectorizable<_Up>()>::type,
            class = typename std::enable_if<is_simd_flag_type<_Flags>::value>::type>
  simd(const _Up* __buffer, _Flags) {
    // TODO: optimize for overaligned flags
    for (size_t __i = 0; __i < size(); __i++) {
      (*this)[__i] = static_cast<_Tp>(__buffer[__i]);
    }
  }

#if 0
  // loads [simd.load]
  template <class _Up, class _Flags>
  typename std::enable_if<__vectorizable<_Up>() &&
                          is_simd_flag_type<_Flags>::value>::type
  copy_from(const _Up* __buffer, _Flags) {
    *this = simd(__buffer, _Flags());
  }
#endif
  // loads [simd.load]
  void copy_from(const simd_data<simd>& __buffer) { __s_.__copy_from(__buffer); }

#if 0
  // stores [simd.store]
  template <class _Up, class _Flags>
  typename std::enable_if<__vectorizable<_Up>() &&
                          is_simd_flag_type<_Flags>::value>::type
  copy_to(_Up* __buffer, _Flags) const {
    // TODO: optimize for overaligned flags
    for (size_t __i = 0; __i < size(); __i++) {
      __buffer[__i] = static_cast<_Up>((*this)[__i]);
    }
  }
#endif
  // stores [simd.store]
  void copy_to(simd_data<simd>& __buffer) const { __s_.__copy_to(__buffer); }

  // scalar access [simd.subscr]
  reference operator[](size_t __i) { return reference(&__s_, __i); }

  value_type operator[](size_t __i) const { return __s_.__get(__i); }

  // unary operators [simd.unary]
  simd& operator++();
  simd operator++(int);
  simd& operator--();
  simd operator--(int);
  mask_type operator!() const;
  simd operator~() const;
  simd operator+() const;
  simd operator-() const;

  // binary operators [simd.binary]
  friend simd operator+(const simd&, const simd&);
  friend simd operator-(const simd&, const simd&);
  friend simd operator*(const simd&, const simd&);
  friend simd operator/(const simd&, const simd&);
  friend simd operator%(const simd&, const simd&);
  friend simd operator&(const simd&, const simd&);
  friend simd operator|(const simd&, const simd&);
  friend simd operator^(const simd&, const simd&);
  friend simd operator<<(const simd&, const simd&);
  friend simd operator>>(const simd&, const simd&);
  friend simd operator<<(const simd&, int);
  friend simd operator>>(const simd&, int);

  // compound assignment [simd.cassign]
  friend simd& operator+=(simd&, const simd&);
  friend simd& operator-=(simd&, const simd&);
  friend simd& operator*=(simd&, const simd&);
  friend simd& operator/=(simd&, const simd&);
  friend simd& operator%=(simd&, const simd&);

  friend simd& operator&=(simd&, const simd&);
  friend simd& operator|=(simd&, const simd&);
  friend simd& operator^=(simd&, const simd&);
  friend simd& operator<<=(simd&, const simd&);
  friend simd& operator>>=(simd&, const simd&);
  friend simd& operator<<=(simd&, int);
  friend simd& operator>>=(simd&, int);

  // compares [simd.comparison]
  friend mask_type operator==(const simd&, const simd&);
  friend mask_type operator!=(const simd&, const simd&);
  friend mask_type operator>=(const simd&, const simd&);
  friend mask_type operator<=(const simd&, const simd&);
  friend mask_type operator>(const simd&, const simd&);
  friend mask_type operator<(const simd&, const simd&);

  value_type dot2(const simd& other) const { return __s_.__dot2(other.__s_); }
  value_type dot3(const simd& other) const { return __s_.__dot3(other.__s_); }
  value_type dot4(const simd& other) const { return __s_.__dot4(other.__s_); }

  template <int x, int y, int z, int w>
  simd shuffle() const {
    simd s;
    s.__s_ = __s_.template __shuffle<x, y, z, w>();
    return s;
  }

  const typename __simd_storage<_Tp, _Abi>::storage_type& native() const { return __s_.__native(); }
};

// [simd.mask.class]
template <class _Tp, class _Abi>
class simd_mask {
public:
  using value_type = bool;
  using reference = __simd_mask_reference<_Tp, _Abi>;
  using simd_type = simd<_Tp, _Abi>;
  using abi_type = _Abi;
  static constexpr size_t size() noexcept { return simd_size<_Tp, _Abi>::value; }
  simd_mask() = default;

  // broadcast constructor
  explicit simd_mask(value_type) noexcept;

  // implicit type conversion constructor
  template <class _Up>
  simd_mask(const simd_mask<_Up, simd_abi::fixed_size<simd_size<_Tp, _Abi>::value>>&) noexcept;

  // load constructor
  template <class _Flags>
  simd_mask(const value_type*, _Flags);

private:
  __simd_mask_storage<_Tp, _Abi> __s_;

public:
  // loads [simd.mask.copy]
  template <class _Flags>
  void copy_from(const value_type*, _Flags);
  template <class _Flags>
  void copy_to(value_type*, _Flags) const;

  // scalar access [simd.mask.subscr]
  reference operator[](size_t __i) { return reference(&__s_, __i); }
  value_type operator[](size_t __i) const { return __s_.__get(__i); }

  // unary operators [simd.mask.unary]
  simd_mask operator!() const noexcept;

  // simd_mask binary operators [simd.mask.binary]
  friend simd_mask operator&&(const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator||(const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator&(const simd_mask&, const simd_mask&)noexcept;
  friend simd_mask operator|(const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator^(const simd_mask&, const simd_mask&) noexcept;

  // simd_mask compound assignment [simd.mask.cassign]
  friend simd_mask& operator&=(simd_mask&, const simd_mask&) noexcept;
  friend simd_mask& operator|=(simd_mask&, const simd_mask&) noexcept;
  friend simd_mask& operator^=(simd_mask&, const simd_mask&) noexcept;

  // simd_mask compares [simd.mask.comparison]
  friend simd_mask operator==(const simd_mask&, const simd_mask&) noexcept;
  friend simd_mask operator!=(const simd_mask&, const simd_mask&) noexcept;

  // compares [simd.comparison]
  friend simd_mask operator==(const simd_type&, const simd_type&);
  friend simd_mask operator!=(const simd_type&, const simd_type&);
  friend simd_mask operator>=(const simd_type&, const simd_type&);
  friend simd_mask operator<=(const simd_type&, const simd_type&);
  friend simd_mask operator>(const simd_type&, const simd_type&);
  friend simd_mask operator<(const simd_type&, const simd_type&);
};

template <class _Simd>
inline simd_data<_Simd>::simd_data(const _Simd& s) {
  s.copy_to(*this);
}

template <class _Tp, int __num_element>
class __simd_storage<_Tp, __simd_abi<_StorageKind::_Array, __num_element>> {
public:
  using storage_type = std::array<_Tp, __num_element>;

private:
  storage_type __storage_;

  template <class, class>
  friend class simd;

  template <class, class>
  friend class simd_mask;

public:
  _Tp __get(size_t __index) const noexcept { return __storage_[__index]; };
  void __set(size_t __index, _Tp __val) noexcept { __storage_[__index] = __val; }
  std::enable_if_t<__num_element >= 4> __set4(float a, float b, float c, float d) noexcept {
    __storage_[0] = a;
    __storage_[1] = b;
    __storage_[2] = c;
    __storage_[3] = d;
  }
  void __broadcast(float __val) noexcept { std::fill(__storage_.begin(), __storage_.end(), __val); }
  std::enable_if_t<__num_element >= 2, _Tp> __dot2(const __simd_storage& other) const noexcept {
    return __storage_[0] * other.__storage_[0] + __storage_[1] * other.__storage_[1];
  }
  std::enable_if_t<__num_element >= 3, _Tp> __dot3(const __simd_storage& other) const noexcept {
    return __storage_[0] * other.__storage_[0] + __storage_[1] * other.__storage_[1] +
           __storage_[2] * other.__storage_[2];
  }
  std::enable_if_t<__num_element >= 4, _Tp> __dot4(const __simd_storage& other) const noexcept {
    return __storage_[0] * other.__storage_[0] + __storage_[1] * other.__storage_[1] +
           __storage_[2] * other.__storage_[2] + __storage_[3] * other.__storage_[3];
  }
  template <int x, int y, int z, int w>
  std::enable_if_t<__num_element >= 4, __simd_storage> __shuffle() const noexcept {
    __simd_storage s;
    s.__storage_[0] = __storage_[x];
    s.__storage_[1] = __storage_[y];
    s.__storage_[2] = __storage_[z];
    s.__storage_[3] = __storage_[w];
    return s;
  }

  void __copy_from(const simd_data<simd<_Tp, __simd_abi<_StorageKind::_Array, __num_element>>>& __buffer) noexcept {
    std::copy(__buffer.begin(), __buffer.end(), __storage_.begin());
  }

  void __copy_to(simd_data<simd<_Tp, __simd_abi<_StorageKind::_Array, __num_element>>>& __buffer) const noexcept {
    std::copy(__storage_.begin(), __storage_.end(), __buffer.begin());
  }

  __simd_storage() = default;
  template <class _Up, int __Unum_element>
  explicit __simd_storage(const __simd_storage<_Up, __simd_abi<_StorageKind::_Array, __Unum_element>>& other) {
    std::copy(other.__native().begin(), other.__native().end(), __storage_.begin());
  }
  const storage_type& __native() const { return __storage_; }
};

template <class _Tp, int __num_element>
class __simd_mask_storage<_Tp, __simd_abi<_StorageKind::_Array, __num_element>> {
  std::bitset<__num_element> __storage_;

public:
  bool __get(size_t __index) const noexcept { return __storage_.test(__index); }
  void __set(size_t __index, bool __val) noexcept { __storage_.set(__index, __val); }
};

} // namespace athena::_simd
