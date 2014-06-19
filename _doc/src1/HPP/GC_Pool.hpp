#ifndef GC_POOL_HPP
#define GC_POOL_HPP
namespace Lib3 {
namespace GC {




INLINE Pool::Pool(UInt32 const &size, UInt32 const &len) {
    assert ( size * len + (sizeof(size_t)) <= 0xFFFFFFFFu );            // x64下最多允许一次 new 出 4g 的内存空间
    assert ( size % (sizeof(size_t)) == 0 && size >= (sizeof(size_t)) );
    _buffer             = (Byte*)new size_t[ (((sizeof(Byte*) + (size + sizeof(void*)) * len) + (sizeof(size_t))) + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1) ];
    _pointers           = (void**)(_buffer + sizeof(Byte*));
    _count              = len;
    _length             = len;
    _size               = size;
    *(Byte**)_buffer    = Null;                                         // 令第1块内存链指向 0
    var data = (Byte*)(_pointers + len);                                // 算数据区起点
    for (UInt32 i = 0; i < len; ++i) {
        _pointers[i] = (void*)data;                                     // 填充块指针
        data += size;
    }
}
INLINE Pool::~Pool() {
    Byte *p = _buffer, *next;                                           // 依次删除所有 Buffer
    do {
        next = *(Byte**)p;                                              // 取下一个内存块地址
        delete [] p;
    }
    while ((p = next));                                                 // 直到遇到 0
}
INLINE void Pool::Resize() {
    var newLen = _length * 2;                                           // 新请求内存时倍增块数
    var incLen = _length;                                               // 计算要新增的块数 ( len - _length )
    var paBlocks = (UInt32)(sizeof(void*) * _length / _size);            // 计算 void** 内存区域能存多少个块
    if (paBlocks > incLen) {                                            // 如果 void** 区域 可以创建比预期更多的块
        newLen += paBlocks - incLen;                                    // 避免浪费, 令请求的 len 变长
        incLen = 0;
    } else
        incLen -= paBlocks;                                             // 除开 void ** 区能满足的块数，得到剩下还有新增多大的块数
    var buf = (Byte*)new size_t[ (((sizeof(Byte*) + sizeof(void*) * newLen + _size * incLen) + (sizeof(size_t))) + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1) ];
    *(Byte**)buf = _buffer;                                             // buf 头 链接到老 Buffer
    _buffer = buf;                                                      // _buffer 指向更新
    var pointers = (void**)(buf + sizeof(Byte*));                       // 令 pointers 指向 buf 中的 void** 区
    if (paBlocks) {                                                     // 如果 void** 内存区 可以创建块
        var items = (Byte*)_pointers;                                   // 令 items 指向 原Buffer 中的 void** 区, 算块起始指针，填充到 新 buffer 中的 pointers 区
        for (UInt32 i = 0; i < paBlocks; ++i) {
            pointers[i] = (void*)items;
            items += _size;
        }
        pointers += paBlocks;                                           // 将 pointers 的位置指向未使用的 void**
    }
    _pointers = (void**)(buf + sizeof(Byte*));                          // 指向新buffer的void**
    if (incLen) {
        var items = (Byte*)(_pointers + newLen);                        // 指到 buf 中的块数据区
        for (UInt32 i = 0; i < incLen; ++i) {                           // 将 void** 区中的块指针写入 pointers
            pointers[i] = items;
            items += _size;
        }
    }
    _count = paBlocks + incLen;
    _length = newLen;
}

#ifdef __DEBUG
INLINE void * Pool::Allocate(UInt32 const &len, SByte const * const &fn, Int32 const &ln) {
    assert ( len <= _size );
    if (--_count == 0)
        Resize();
    return _pointers[_count];
}
INLINE void Pool::Free(void * const &p, SByte const * const &fn, Int32 const &ln) {
    assert ( _count <= _length );
    _pointers[_count++] = p;
}
#else
INLINE void * Pool::Allocate(UInt32 const &len) {
    if (--_count == 0)
        Resize();
    return _pointers[_count];
}
INLINE void Pool::Free(void * const &p) {
    _pointers[_count++] = p;
}
#endif


INLINE UInt32 & Pool::Length() {
    return _length;
}
INLINE UInt32 const & Pool::Length() const {
    return _length;
}
INLINE UInt32 & Pool::Count() {
    return _count;
}
INLINE UInt32 const & Pool::Count() const {
    return _count;
}





}}
#endif