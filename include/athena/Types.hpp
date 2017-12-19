#ifndef TYPES_HPP
#define TYPES_HPP
#include <stdint.h>
#include <inttypes.h>

using atInt8 = int8_t;
using atUint8 = uint8_t;
using atInt16 = int16_t;
using atUint16 = uint16_t;
using atInt32 = int32_t;
using atUint32 = uint32_t;
using atInt64 = int64_t;
using atUint64 = uint64_t;

// Vector types
#if __SSE__
#include <immintrin.h>
#ifndef _WIN32
#include <mm_malloc.h>
#endif
#endif

#include <new>
#define AT_ALIGNED_ALLOCATOR \
void* operator new(size_t bytes) noexcept \
{return _mm_malloc(bytes, 16);} \
void* operator new[](size_t bytes) noexcept \
{return _mm_malloc(bytes, 16);} \
void operator delete(void* buf) noexcept \
{_mm_free(buf);} \
void operator delete[](void* buf) noexcept \
{_mm_free(buf);}

#define AT_ALIGNED_ALLOCATOR32 \
void* operator new(size_t bytes) noexcept \
{return _mm_malloc(bytes, 32);} \
void* operator new[](size_t bytes) noexcept \
{return _mm_malloc(bytes, 32);} \
void operator delete(void* buf) noexcept \
{_mm_free(buf);} \
void operator delete[](void* buf) noexcept \
{_mm_free(buf);}

typedef union alignas(16)
{
#if __clang__
    float clangVec __attribute__((__vector_size__(8)));
#endif
#if __SSE__
    __m128 mVec128;
    AT_ALIGNED_ALLOCATOR
#endif
    float vec[2];
} atVec2f;

typedef union alignas(16)
{
#if __clang__
    float clangVec __attribute__((__vector_size__(12)));
#endif
#if __SSE__
    __m128 mVec128;
    AT_ALIGNED_ALLOCATOR
#endif
    float vec[3];
} atVec3f;

typedef union alignas(16)
{
#if __clang__
    float clangVec __attribute__((__vector_size__(16)));
#endif
#if __SSE__
    __m128 mVec128;
    AT_ALIGNED_ALLOCATOR
#endif
    float vec[4];
} atVec4f;

typedef union alignas(16)
{
#if __SSE__
    __m128d mVec128;
    AT_ALIGNED_ALLOCATOR
#endif
    double vec[2];
} atVec2d;

typedef union alignas(32)
{
#if __AVX__
    __m256d mVec256;
    AT_ALIGNED_ALLOCATOR32
#elif __SSE__
    AT_ALIGNED_ALLOCATOR
#endif
#if __SSE__
    __m128d mVec128[2];
#endif
    double vec[3];
} atVec3d;

typedef union alignas(32)
{
#if __AVX__
    __m256d mVec256;
    AT_ALIGNED_ALLOCATOR32
#elif __SSE__
    AT_ALIGNED_ALLOCATOR
#endif
#if __SSE__
    __m128d mVec128[2];
#endif
    double vec[4];
} atVec4d;

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
