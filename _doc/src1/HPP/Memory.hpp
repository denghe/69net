#ifndef MEMORY_HPP
#define MEMORY_HPP
namespace Lib3 {
namespace Memory {


template<UInt32 alignSize>
void * AlignAllocate(UInt32 const &len) {
    assert ( alignSize == 8 || alignSize == 16 || alignSize == 32 || alignSize == 64 );     // 应该够了
	Byte *p, *ap;                                                       // ap: aligned pointer
	p = new Byte[len + alignSize + 8];                                  // 多请求为对齐会被削掉的空间 以及 存放 偏移量, 长度 的空间
	ap = (Byte*)( (((size_t)p + 8) + (alignSize-1)) & ~(alignSize-1) ); // 前进8, 再前进 对齐产生的多余字节
	((UInt32*)ap)[-2] = (UInt32)(ap - p);                               // 将偏移写入 ap[-8 ~ -5 ]
	return (void*)ap;
}

template<typename T, UInt32 alignSize>
T * AlignAllocate(UInt32 const &len) {
    var p = (T*)AlignAllocate<alignSize>(len * sizeof(T));
    ((UInt32*)p)[-1] = len * sizeof(T);                                 // 将长度写入 ap[-4 ~ -1 ]
    for (UInt32 i = 0; i < len; ++i)
        new (&p[i]) T();
    return p;
}

template<typename T>
void AlignFree(T * const &p) {
    var len = ((UInt32*)p)[-1];
    for (UInt32 i = 0; i < len; i += sizeof(T))
        (*(T*)(((Byte*)p) + i)).~T();
    AlignFree((void*)p);
}


INLINE void AlignFree(void * const &p) {
	delete [] (((Byte*)p) - ((UInt32*)p)[-2]);
}




}}  // namespace
#endif