#ifndef __ALL_H__
#define __ALL_H__

// this is precompile header

/*
index:

__IA            __IA32              __IA64
__X64           __X86 / __X32
__DEBUG         __RELEASE
__BE            __LE
__IOS           __ANDROID           __LINUX             __WIN / __WIN32 / __WINDOWS
__GCC           __GCC_VERSION       __MSVC              __RVCT

INLINE          NOINLINE
LIKELY          UNLIKELY
ALIGN2/4/8/16/32

HAS_FUNC

*/

/* CPU(X86) - i386 / x86 32-bit */
#if defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL)
#define __IA32 1
#endif

/* CPU(X86_64) - AMD64 / Intel64 / x86_64 64-bit */
#if defined(__x86_64__) || defined(_M_X64)
#define __IA64 1
#endif

/* CPU - INTEL / AMD */
#if defined(__IA32) || defined(__IA64)
#define __IA 1
#endif

/* Compile - X86 / X64 */
#if defined(__LP64__) || defined(_WIN64) || defined(_M_X64)
#define __X64 1
#else
#define __X32 1
#define __X86 1
#endif

/* Compile - DEBUG / RELEASE */
#if defined(NDEBUG) && !defined(DEBUG)
#define __RELEASE 1
#else
#define __DEBUG 1
#endif

/* CPU - big endian / little endian */
#if defined(BIGENDIAN) || defined(BIG_ENDIAN)
#define __BE 1
#else
#define __LE 1
#endif

/* OS - windows, ios, android, linux 这里完全不考虑 mac osx */
#ifdef _WIN32
#define __WIN 1
#define __WIN32 1
#define __WINDOWS 1
#elif defined(__APPLE__)
#define __IOS 1
#elif defined(__ANDROID__)
#define __ANDROID 1
#endif
#ifdef __linux
#define __LINUX 1
#endif

/* Compiler - ARM RealView Compilation Tools */
#if defined(__CC_ARM) || defined(__ARMCC__)
#define __RVCT 1
#endif

/* Compiler - GNU Compiler Collection */
/* --gnu option of the RVCT compiler also defines __GNUC__ */
#if defined(__GNUC__) && !defined(__RVCT)
#define __GCC 1
#define __GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

/* Compiler - Microsoft Visual C++ */
#if defined(_MSC_VER)
#define __MSVC 1
#endif


/* Compile - enforce inline */
#ifndef INLINE
#if __GCC && __RELEASE && !defined(__MINGW32__)
#define INLINE inline __attribute__((__always_inline__))
#elif (__MSVC || __GCC) && __RELEASE
#define INLINE __forceinline
#else
#define INLINE inline
#endif
#endif

/* Compile - enforce noinline */
#ifndef NOINLINE
#if __GCC
#define NOINLINE __attribute__((__noinline__))
#elif __MSVC || __RVCT
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE
#endif
#endif

/* CPU - branch prediction: unlikely */
#ifndef UNLIKELY
#if __GCC
#define UNLIKELY(x) __builtin_expect((x), 0)
#else
#define UNLIKELY(x) (x)
#endif
#endif

/* CPU - branch prediction: likely */
#ifndef LIKELY
#if __GCC
#define LIKELY(x) __builtin_expect((x), 1)
#else
#define LIKELY(x) (x)
#endif
#endif

/* Memory address align */  // todo: delete or change to c11 alignof
#if __MSVC
#define ALIGN2( x )		        __declspec(align(2)) x
#define ALIGN4( x )		        __declspec(align(4)) x
#define ALIGN8( x )		        __declspec(align(8)) x
#define ALIGN16( x )		    __declspec(align(16)) x
#define ALIGN32( x )		    __declspec(align(32)) x
#elif __GCC
#define ALIGN2( x )             x __attribute__ ((aligned (2)))
#define ALIGN4( x )             x __attribute__ ((aligned (4)))
#define ALIGN8( x )             x __attribute__ ((aligned (8)))
#define ALIGN16( x )            x __attribute__ ((aligned (16)))
#define ALIGN32( x )            x __attribute__ ((aligned (32)))
#else
#define ALIGN2( x )	 x
#define ALIGN4( x )	 x
#define ALIGN8( x )	 x
#define ALIGN16( x ) x
#define ALIGN32( x ) x
#endif






/*

SFINAE check menber function exists
sample：

HAS_FUNC( FUNC_NAME_checker, T_MEMBER_FUNC_NAME, RETURN_TYPE ( T::* )( T_MEMBER_FUNC_PARMS ) const );

template<typename T>
typename std::enable_if<FUNC_NAME_checker<T>::value, RETURN_TYPE>::type FUNC_NAME_switch( T const& v )
{
    return v.T_MEMBER_FUNC_NAME( T_MEMBER_FUNC_PARMS );
};
template<typename T>
typename std::enable_if<!FUNC_NAME_checker<T>::value, RETURN_TYPE>::type FUNC_NAME_switch( T const& v )
{
    return ................;
};

template<typename T>
RETURN_TYPE FUNC_NAME( T const& v )
{
    return FUNC_NAME_switch( v );
}


*/
#define HAS_FUNC( CN, FN, FT )   \
template<typename T>                                                                \
class CN                                                                            \
{                                                                                   \
    template<typename T, FT> struct FuncMatcher;                                    \
    template<typename T> static char hasFunc( FuncMatcher<T, &T::FN>* );            \
    template<typename T> static int hasFunc( ... );                                 \
public:                                                                             \
    static const bool value = sizeof( hasFunc<T>( nullptr ) ) == sizeof(char);      \
}









#include <cmath>
#include <cstdint>
#include <cassert>

#include <string>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <random>
#include <memory>
#include <new>

#include <type_traits>
#include <array>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <functional>
#include <chrono>
#include <locale>
#include <codecvt>


/* Common types */
typedef int8_t  int8, sbyte;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8, byte;
typedef uint16_t uint16, ushort;
typedef uint32_t uint32, uint;
typedef uint64_t uint64;

typedef unsigned long ulong;    // x64: vc 32bit, gcc 64bit
typedef wchar_t wchar;          // win/ios: 16bit, linux, android 32bit











#include "Stopwatch.h"
#include "List.h"
#include "Pool.h"
#include "String.h"
#include "Dict.h"
#include "CircleBuffer.h"
#include "LRUCache.h"
#include "FlatBuffer.h"
#include "BufferUtils.h"
#include "Utils.h"

// ... more



#include "BufferUtils.hpp"
#include "FlatBuffer.hpp"
#include "List.hpp"
#include "LRUCache.hpp"
#include "String.hpp"
#include "Utils.hpp"
#include "Dict.hpp"

// ... more


#endif
