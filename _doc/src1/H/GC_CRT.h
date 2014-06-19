#ifndef GC_CRT_H
#define GC_CRT_H
namespace Lib3 {
namespace GC {


struct CRT : public Base {
#ifdef __DEBUG
    void *                  Allocate(UInt32 const &len, SByte const * const &fn = Null, Int32 const &ln = 0);
    void                    Free(void * const &p,       SByte const * const &fn = Null, Int32 const &ln = 0);
#else
    void *                  Allocate(UInt32 const &len);                // 直接调用 C++ runtime 的 new size_t[] (但是请求的是 4/8 的倍数内存且对齐)
    void                    Free(void * const &p);                      // 直接调用 C++ runtime 的 delete [] size_t*
#endif
    static CRT const        Instance;
};



}}
#endif