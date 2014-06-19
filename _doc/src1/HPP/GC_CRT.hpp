#ifndef GC_CRT_HPP
#define GC_CRT_HPP
namespace Lib3 {
namespace GC {


#ifdef __DEBUG
INLINE void * CRT::Allocate(UInt32 const &len, SByte const * const &fn, Int32 const &ln) {
    // todo: 记录 filename, line number
    return (void *) new size_t[(len + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1)];
}
INLINE void CRT::Free(void * const &p, SByte const * const &fn, Int32 const &ln) {
    // todo: 记录 filename, line number
    delete [] (size_t*)p;
}
#else
INLINE void * CRT::Allocate(UInt32 const &len) {
    return (void *) new size_t[(len + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1)];
}
INLINE void CRT::Free(void * const &p) {
    delete [] (size_t*)p;
}
#endif



}}
#endif