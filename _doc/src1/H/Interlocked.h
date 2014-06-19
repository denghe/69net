#ifndef INTERLOCKED_H
#define INTERLOCKED_H
namespace Lib3 {
namespace Interlocked {

    INLINE UInt64 And(volatile UInt64 &dest, UInt64 const &v) {
#ifdef __linux
        return __sync_fetch_and_and((UInt64*)&dest, v);
#else
        return InterlockedAnd(&dest, v);
#endif
    }
    INLINE UInt64 Or (volatile UInt64 &dest, UInt64 const &v) {
#ifdef __linux
        return __sync_fetch_and_or((UInt64*)&dest, v);
#else
        return InterlockedOr(&dest, v);
#endif
    }
    INLINE UInt64 Xor(volatile UInt64 &dest, UInt64 const &v) {
#ifdef __linux
        return __sync_fetch_and_xor((UInt64*)&dest, v);
#else
        return InterlockedXor(&dest, v);
#endif
    }

    INLINE UInt32 Exchange(volatile UInt32 &dest, UInt32 const &v) {
#ifdef __linux
        return __sync_fetch_and_and((UInt32*)&dest, v);
#else
        return InterlockedExchange(&dest, v);
#endif
    }
    INLINE UInt64 Exchange(volatile UInt64 &dest, UInt64 const &v) {
#ifdef __linux
    return __sync_fetch_and_and((UInt64*)&dest, v);
#else
    return InterlockedExchange(&dest, v);
#endif
    }
    INLINE void * Exchange(volatile void * &dest, void * const &v) {
#ifdef __linux
    return __sync_fetch_and_and((void**)&dest, v);
#else
    return InterlockedExchangePointer((volatile PVOID*)&dest, v);
#endif
    }

    INLINE UInt32 CompareExchange(volatile UInt32 &dest, UInt32 const &v, UInt32 const &cv) {
#ifdef __linux
    return __sync_val_compare_and_swap(&dest, cv, v);
#else
    return InterlockedCompareExchange(&dest, v, cv);
#endif
    }
    INLINE UInt64 CompareExchange(volatile UInt64 &dest, UInt64 const &v, UInt64 const &cv) {
#ifdef __linux
    return __sync_val_compare_and_swap(&dest, cv, v);
#else
    return InterlockedCompareExchange64((volatile Int64*)&dest, (Int64)v, (Int64)cv);
#endif
    }
    INLINE void * CompareExchange(volatile void * &dest, void * const &v, void * const &cv) {
#ifdef __linux
    return __sync_val_compare_and_swap((void **)&dest, cv, v);
#else
    return InterlockedCompareExchangePointer((volatile PVOID*)&dest, (PVOID)v, (PVOID)cv);
#endif
    }

    INLINE UInt32 Increment(volatile UInt32 &dest) {
#ifdef __linux
        return __sync_add_and_fetch((UInt32*)&dest, 1);
#else
        return InterlockedIncrement(&dest);
#endif
    }
    INLINE UInt64 Increment(volatile UInt64 &dest) {
#ifdef __linux
    return __sync_add_and_fetch((UInt64*)&dest, 1);
#else
    return InterlockedIncrement(&dest);
#endif
    }

    INLINE UInt32 Decrement(volatile UInt32 &dest) {
#ifdef __linux
    return __sync_add_and_fetch((UInt32*)&dest, -1);
#else
    return InterlockedDecrement(&dest);
#endif
    }
    INLINE UInt64 Decrement(volatile UInt64 &dest) {
#ifdef __linux
    return __sync_add_and_fetch((UInt64*)&dest, -1);
#else
    return InterlockedDecrement(&dest);
#endif
    }

	INLINE Int32  Add(volatile Int32 &dest, Int32 const &v) {
#ifdef __linux
	return __sync_add_and_fetch(&dest, v);
#else
	return InterlockedExchangeAdd((LONG*)&dest, (LONG)v);
#endif
    }

}}
#endif
