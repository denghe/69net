#ifndef GC_BASE_H
#define GC_BASE_H
namespace Lib3 {
namespace GC {


struct Base {
#ifdef __DEBUG
    virtual void *              Allocate(UInt32 const &len, SByte const * const &fn = Null, Int32 const &ln = 0) = 0;
    virtual void                Free(void * const &p,       SByte const * const &fn = Null, Int32 const &ln = 0) = 0;
#else
    virtual void *              Allocate(UInt32 const &len) = 0;
    virtual void                Free(void * const &p) = 0;
#endif
};


}}
#endif