#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP
namespace Lib3 {



template <typename T>
INLINE LinkedList<T>::LinkedList() {                                                       // 默认创建含有 List_OriginalBufferLength 个元素存放空间的 Buffer
    Init();
}
template <typename T>
INLINE LinkedList<T>::LinkedList(UInt32 const &len) {
    assert ( sizeof(Byte*) + sizeof(T*) * len + sizeof(T) * len <= 0xFFFFFFFFu );     // x64下最多允许一次 new 出 4g 的内存空间
    if (len <= List_OriginalBufferLength)
        Init();
    else
        Init(len);
}
template <typename T>
LinkedList<T>::LinkedList(LinkedList const &o) {
    var len = o._length;
    if (len <= List_OriginalBufferLength)
        Init();
    else
        Init(len);
    for (UInt32 i = 0; i < len; ++i)
        new (_indexs[i]) T(*o._indexs[i]);
    _length = len;
}
template <typename T>
LinkedList<T>::~LinkedList() {
    for (UInt32 i = 0; i < _length; ++i)                                 // 依次回收已使用的节点( 调析构 )
        _indexs[i]->~T();

    var p = _buffer;                                                     // 依次删除所有 Buffer
Lab1:
    if (p != _originalBuffer) {
        var next = *(Byte**)p;                                          // 取下一个内存块地址
        delete [] p;
        p = next;
        goto Lab1;
    }
}
template <typename T>
void LinkedList<T>::Init() {
    _buffer              = _originalBuffer;
    _indexs              = (T**)_originalBuffer;
    _indexsLength        = List_OriginalBufferLength;
    _length              = 0;
    var items           = (T*)(_buffer + sizeof(T*) * List_OriginalBufferLength);
    for (UInt32 i = 0; i < List_OriginalBufferLength; ++i)
        _indexs[i] = items + i;
}
template <typename T>
INLINE void LinkedList<T>::Init(UInt32 const &len) {
    Init();
    Resize(len);
}
template <typename T>
void LinkedList<T>::Resize(UInt32 const &len) {
    UInt32 len_ = len;
    var incLen = len_ - _indexsLength;                                    // 计算 resize 的元素个数
    var idxAreaItemsLen = (UInt32)(sizeof(T*) * _indexsLength / sizeof(T));    // 计算原 Buffer 存放 Indexs 的区域能存多少个元素
    if (idxAreaItemsLen > incLen) {                                     // 如果 Indexs区域 可以创建元素则创建 (修正 resize 元素个数, 请求内存，复制，再创建)
        len_ += idxAreaItemsLen - incLen;                                // 避免浪费, 令请求的 len 变长
        incLen = 0;
    } else
        incLen -= idxAreaItemsLen;
    var buf = new Byte[ sizeof(Byte*) + sizeof(T*) * len_ + sizeof(T) * incLen ]; // 请求新内存 buf
    *(Byte**)buf = _buffer;                                              // buf 头 链接到老 Buffer
    _buffer = buf;                                                       // Buffer 指向更新
    var idxs = (T**)(buf + sizeof(Byte*));                              // 令 idxs 指向 buf 中的 Indexs 区
    memcpy(idxs, _indexs, sizeof(T*) * _indexsLength);                    // 复制 Buffer 中的 Indexs 区到 buf 中的 Indexs 区
    idxs += _indexsLength;                                               // 将 idxs 的位置指向未使用的 Indexs
    if (idxAreaItemsLen) {
        var items = (T*)_indexs;                                         // 令 items 指向 Buffer 中的 Indexs 区
        for (UInt32 i = 0; i < idxAreaItemsLen; ++i)                    // 将 Indexs 区中的元素指针写入 idxs
            idxs[i] = items + i;
        idxs += idxAreaItemsLen;                                        // 将 idxs 的位置指向未使用的 Indexs
    }
    _indexs = (T**)(buf + sizeof(Byte*));                                // Indexs 指向更新
    _indexsLength = len_;                                                 // IndexsLength 更新
    if (incLen) {
        var items = (T*)(_indexs + len_);                                 // 指到 buf 中的元素数据区
        for (UInt32 i = 0; i < incLen; ++i)                             // 将 Indexs 区中的元素指针写入 idxs
            idxs[i] = items + i;
    }
}
template <typename T>
INLINE LinkedList<T> & LinkedList<T>::Add(T const &v) {
    if (_length == _indexsLength)
        Resize(_length * 2);
    new (_indexs[_length++]) T(v);
    return *this;
}
template <typename T>
INLINE LinkedList<T> & LinkedList<T>::Add(T &&v) {
    if (_length == _indexsLength)
        Resize(_length * 2);
    new (_indexs[_length++]) T((T&&)v);
    return *this;
}
template <typename T>
LinkedList<T> & LinkedList<T>::RemoveAt(UInt32 const &idx) {
    if (idx >= _length)
        return *this;
    if (_length == 0)
        return *this;
    --_length;
    _indexs[idx]->~T();
    if (idx == _length)
        return *this;
    var tmp = _indexs[idx];
    memmove(&_indexs[idx], &_indexs[idx + 1], sizeof(T*) * (_length - idx));
    _indexs[_length] = tmp;
    return *this;
}
template <typename T>
LinkedList<T> & LinkedList<T>::Remove(UInt32 const &offset, UInt32 const &len) {
    if (len == 0)
        return Remove(offset);
    if (offset >= _length)
        return *this;
    else {
        UInt32 len_ = len;
        if (len_ > _length - offset)
            len_ = _length - offset;
        _length -= len_;
        for (var i = offset; i < offset + len_; ++i)
            _indexs[i]->~T();
        memmove(&_indexs[offset], &_indexs[offset + len_], sizeof(T*) * (_length - offset));
        return *this;
    }
}
template <typename T>
LinkedList<T> & LinkedList<T>::Remove(UInt32 const &offset) {
    for (var i = offset; i < _length; ++i)
        _indexs[i]->~T();
    _length = offset;
    return *this;
}
template <typename T>
INLINE LinkedList<T> & LinkedList<T>::Clear() {
    for (UInt32 i = 0; i < _length; ++i)                                 // 依次回收已使用的节点( 调析构 )
        _indexs[i]->~T();
    _length = 0;
    return *this;
}
template <typename T>
LinkedList<T> & LinkedList<T>::Insert(UInt32 const &idx, T const &v) {
    if (idx >= _length)
        Add(v);
    else {
        if (_length == _indexsLength)
            Resize(_length * 2);
        var tmp = _indexs[_length];
        memmove(&_indexs[idx + 1], &_indexs[idx], sizeof(T*) * (_length - idx));
        _indexs[idx] = tmp;
        new (tmp) T(v);
        ++_length;
    }
    return *this;
}
template <typename T>
LinkedList<T> & LinkedList<T>::Insert(UInt32 const &idx, T &&v) {
    if (idx >= _length)
        Add((T&&)v);
    else {
        if (_length == _indexsLength)
            Resize(_length * 2);
        var tmp = _indexs[_length];
        memmove(&_indexs[idx + 1], &_indexs[idx], sizeof(T*) * (_length - idx));
        _indexs[idx] = tmp;
        new (tmp) T((T&&)v);
        ++_length;
    }
    return *this;
}
template <typename T>
Boolean LinkedList<T>::Contains(T const &t) const {
    for (UInt32 i = 0; i < _length; ++i)
        if (*_indexs[i] == t)
            return true;
    return false;
}

template <typename T>
Boolean LinkedList<T>::operator==(LinkedList const &o) const {
    if (_length == 0 && o.Length == 0)
        return true;
    if (_length != o.Length)
        return false;
    for (UInt32 i = 0; i < _length; ++i) {
        if (*_indexs[i] == *o.Indexs[i])
            continue;
        else
            return false;
    }
    return true;
}
template <typename T>
INLINE T & LinkedList<T>::operator[](UInt32 const &idx) {
    return *_indexs[idx];
}
template <typename T>
INLINE T const & LinkedList<T>::operator[](UInt32 const &idx) const {
    return *_indexs[idx];
}
template <typename T>
INLINE UInt32 & LinkedList<T>::Length() {
    return _length;
}
template <typename T>
INLINE UInt32 const & LinkedList<T>::Length() const {
    return _length;
}

template <typename T>
INLINE void LinkedList<T>::EnsureLength(UInt32 const &len) {
    if (len > _indexsLength)
        Resize(len);
    _length = len;
}

template <typename T>
INLINE void LinkedList<T>::EnsureSpace(UInt32 const &len) {
    if (_length + len > _indexsLength)
        Resize((_length + len) * 2);
}



}
#endif