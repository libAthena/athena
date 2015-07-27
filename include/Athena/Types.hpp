#ifndef TYPES_HPP
#define TYPES_HPP
#include <limits.h>

// 8 bits integer types
#if UCHAR_MAX == 0xFF
typedef signed   char atInt8;
typedef unsigned char atUint8;
#else
#error No 8 bits integer type for this platform
#endif

// 16 bits integer types
#if USHRT_MAX == 0xFFFF
typedef signed   short atInt16;
typedef unsigned short atUint16;
#elif UINT_MAX == 0xFFFF
typedef signed   int atInt16;
typedef unsigned int atUint16;
#elif ULONG_MAX == 0xFFFF
typedef signed   long atInt16;
typedef unsigned long atUint16;
#else
#error No 16 bits integer type for this platform
#endif

// 32 bits integer types
#if USHRT_MAX == 0xFFFFFFFF
typedef signed   short atInt32;
typedef unsigned short atUint32;
#elif UINT_MAX == 0xFFFFFFFF
typedef signed   int atInt32;
typedef unsigned int atUint32;
#elif ULONG_MAX == 0xFFFFFFFF
typedef signed   long atInt32;
typedef unsigned long atUint32;
#else
#error No 32 bits integer type for this platform
#endif

typedef signed   long long atInt64;
typedef unsigned long long atUint64;

// Vector types
#if __SSE__
#include <xmmintrin.h>
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
{_mm_free(buf);} \

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


#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else // __cplusplus
#define NULL (void*)0
#endif
#endif // NULL

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
