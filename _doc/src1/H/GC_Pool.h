#ifndef GC_POOL_H
#define GC_POOL_H
namespace Lib3 {
namespace GC {



// 批量请求内存，块起始地址放到连续的空间以方便请求。分配完毕则再次请求内存。
struct Pool : public Base {

#ifdef __DEBUG
    void *                  Allocate(UInt32 const &len, SByte const * const &fn = Null, Int32 const &ln = 0);
    void                    Free(void * const &p,       SByte const * const &fn = Null, Int32 const &ln = 0);
#else
    void *                  Allocate(UInt32 const &len);                // 直接调用 C++ runtime 的 new size_t[] (但是请求的是 4/8 的倍数内存且对齐)
    void                    Free(void * const &p);                      // 直接调用 C++ runtime 的 delete [] size_t*
#endif

    Pool(UInt32 const &size, UInt32 const &len = 16);                   // 创建 block size * len 的初始大小的池. block size 需要 8 的倍数
    ~Pool();
    UInt32 &                Length();                                   // 引用 _length (总块数)
    UInt32 &                Count();                                    // 引用 _indexsLength (剩余块数)
    UInt32 const &          Length() const;                             // 
    UInt32 const &          Count() const;                              // 

private:
    void                    Resize();                                   // 分配完后再次请求 ( old len * 2 )

    // 存储格式为 [指向上一块Buffer的Byte*][_count 个 void * 指针区][数据区 .........]
    Byte *                  _buffer;                                    // 指向 Buffer 头(当前 Buffer). 头部含指向下一块 Buffer 的指针. resize 创建一倍大的新内存块
    void **                 _pointers;                                  // 始终指向 _buffer + sizeof(void*) 的位置
    UInt32                  _size;                                      // Block Size
    UInt32                  _count;                                     // 当前 Buffer 中的 指针 个数 (同时也是总共的) resize += 原 个数 * 2
    UInt32                  _length;                                    // 块数 初始为 len
};




}}
#endif