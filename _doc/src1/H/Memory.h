#ifndef MEMORY_H
#define MEMORY_H
namespace Lib3 {
namespace Memory {



// new len长内存，并按传入的 alignSize 返回一个对齐后的指针
// 算法实现基于 new 出来的地址至少是 4 的倍数, 且长度用 UInt32 放得下
// 将在返回指针的 前面8~5字节中放偏移量
// 关于crt new 的地址是否对齐的原理, 在于 new 的 type 有没有析构以及成员是否需要8倍对齐
// 即：如果有析构　意味着 new 自己要记录 len, 以方便 delete [] 调
// 这意味着本来 malloc 出来的 8 字节对齐的内存, 被占掉 4 字节, 从而变成 4 的倍数
// 如果 type 成员需要8倍对齐 new 最终会根据这个来调整并返回
template<UInt32 alignSize>
void * AlignAllocate(UInt32 const &len);

// 删除对齐的后的指针 (该指针由 AlignAllocate(len) 版分配)
void AlignFree(void * const &p);

// new len * sizeof(T) 长内存，依次调元素构造函数，最后按传入的 alignSize 返回一个对齐后的指针
// 将在返回指针的 前面4~1字节中放长度 前面8~5字节中放偏移量
template<typename T, UInt32 alignSize>
T * AlignAllocate(UInt32 const &len);

// 依次调析构函数后 删除对齐的后的指针 (该指针由 AlignAllocate<T>(len) 版分配)
template<typename T>
void AlignFree(T * const &p);



}}  // namespace
#endif