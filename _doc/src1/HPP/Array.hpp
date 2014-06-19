#ifndef ARRAY_HPP
#define ARRAY_HPP
namespace Lib3 {


// Array.h 中的模板的具体实现


template <typename T>
void Array<T>::Init() {
    _buffer = (T*)_originalBuffer;
    _bufferLength = Array_OriginalBufferLength;
    _length = 0;
}
template <typename T>
void Array<T>::Init(UInt32 const &len) {
    _buffer = (T*)new Byte[sizeof(T) * len];                             // 开辟出内存空间　但不初始化
    _bufferLength = len;
    _length = 0;
}
template <typename T>
void Array<T>::Init(T const * const &a, UInt32 const &len) {
    if (len > Array_OriginalBufferLength) {
        _buffer = (T*)new Byte[sizeof(T) * len];
        _bufferLength = len;
    }
    else {
        _buffer = (T*)_originalBuffer;
        _bufferLength = Array_OriginalBufferLength;
    }
    _length = len;
    for (UInt32 i = 0; i < len; ++i)                                    // 复制元素
        new (&_buffer[i]) T(a[i]);
}
template <typename T>
INLINE Array<T>::Array() {
    Init();
}
template <typename T>
INLINE Array<T>::Array(UInt32 const &len) {
    Init(len);
}
template <typename T>
INLINE Array<T>::Array(T const * const &a, UInt32 const &len) {
    Init(a, len);
}
template <typename T>
template<UInt32 len>
INLINE Array<T>::Array(T const (&a)[len]) {
    Init(a, len);
}
template <typename T>
INLINE Array<T>::Array(Array<T> const &a) {
    if (a.Length())
        Init(a.Buffer(), a.Length());
    else 
        Init();
}

template <typename T>
Array<T>::~Array() {
    for (UInt32 i = 0; i < _length; ++i)                                 // 析构元素
        _buffer[i].~T();
    if (_buffer != (T*)_originalBuffer)                                   // 如果是 new 的，回收内存
        delete [] (Byte*)_buffer;
}

template <typename T>
INLINE T * & Array<T>::Buffer() {
    return _buffer;
}
template <typename T>
INLINE T * const & Array<T>::Buffer() const {
    return _buffer;
}
template <typename T>
INLINE UInt32 & Array<T>::BufferLength() {
    return _bufferLength;
}
template <typename T>
INLINE UInt32 & Array<T>::Length() {
    return _length;
}
template <typename T>
INLINE UInt32 const & Array<T>::BufferLength() const {
    return _bufferLength;
}
template <typename T>
INLINE UInt32 const & Array<T>::Length() const {
    return _length;
}



template <typename T>
void Array<T>::EnsureSpace(UInt32 const &len) {
    if (_length + len > _bufferLength) {
        var bak_buff    = _buffer;
        _bufferLength    = (_length + len) * 2;
        _buffer          = (T*)new Byte[sizeof(T) * _bufferLength];
        for (UInt32 i = 0; i < _length; ++i)
            new (&_buffer[i]) T((T&&)bak_buff[i]);                      // 复制旧数据
        for (UInt32 i = 0; i < _length; ++i)
            bak_buff[i].~T();                                           // 析构旧数据
        if (bak_buff != (T*)_originalBuffer)
            delete [] (Byte*)bak_buff;                                  // 如果不是原始 Buffer 则回收
    }
}
template <typename T>
void Array<T>::EnsureLength() {
    if (_length > _bufferLength) {
        var bak_buff    = _buffer;
        _bufferLength    = _length;
        _buffer          = (T*)new Byte[sizeof(T) * _length];
        for (UInt32 i = 0; i < _length; ++i)
            new (&_buffer[i]) T((T&&)bak_buff[i]);
        for (UInt32 i = 0; i < _length; ++i)
            bak_buff[i].~T();                    
        if (bak_buff != (T*)_originalBuffer)
            delete [] (Byte*)bak_buff;           
    }
}
template <typename T>
Array<T> & Array<T>::Clear() {
    for (UInt32 i = 0; i < _length; ++i)                                 // 析构元素
        _buffer[i].~T();
    _length = 0;
    return *this;
}

template <typename T>
INLINE Array<T> & Array<T>::operator=(Array const &a) {
    return Clear().Add(a);
}


template <typename T>
Boolean Array<T>::operator==(Array<T> const &a) const {
    if (_length != a.Length())
        return false;
    for (UInt32 i = 0; i < _length; ++i)
        if (!_buffer[i] == a.Buffer[i])
            return false;
    return true;
}

template <typename T>
INLINE Boolean Array<T>::operator!=(Array<T> const &a) const {
    return !operator==(a);
}

template <typename T>
INLINE T & Array<T>::operator[](UInt32 const &idx) {
    return _buffer[idx];
}

template <typename T>
INLINE T const & Array<T>::operator[](UInt32 const &idx) const {
    return _buffer[idx];
}

template <typename T>
INLINE Array<T> & Array<T>::operator<<(T const &t) {
    return Add(t);
}
template <typename T>
INLINE Array<T> & Array<T>::operator<<(T &&t) {
    return Add((T&&)t);
}
template <typename T>
INLINE Array<T> & Array<T>::Add(T const &t) {
    EnsureSpace(1);
    new (&_buffer[_length++]) T(t);                                       // 调复制构造
    return *this;
}
template <typename T>
INLINE Array<T> & Array<T>::Add(T &&t) {
    EnsureSpace(1);
    new (&_buffer[_length++]) T((T &&)t);                                 // 调右值版复制构造
    return *this;
}
template <typename T>
Array<T> & Array<T>::Add(T const * const &a, UInt32 const &len) {
    assert ( a );
    if (len == 0)
        return *this;
    EnsureSpace(len);
    for (UInt32 i = 0; i < len; ++i)
        new (&_buffer[i + _length]) T(a[i]);                              // 调复制构造
    _length += len;
    return *this;
}
template <typename T>
template<UInt32 len>
INLINE Array<T> & Array<T>::Add(T const (&a)[len]) {
    return Add(a, len);
}
template <typename T>
INLINE Array<T> & Array<T>::Add(Array<T> const &a) {
    return Add(a.Buffer(), a.Length());
}

template <typename T>
Boolean Array<T>::Contains(T const &t) const {
    for (UInt32 i = 0; i < _length; ++i)
        if (_buffer[i] == t)
            return true;
    return false;
}

template <typename T>
Int32 Array<T>::IndexOf(T const &t) const {
    for (UInt32 i = 0; i < _length; ++i)
        if (_buffer[i] == t)
            return i;
    return -1;
}
template <typename T>
Int32 Array<T>::IndexOf(UInt32 const &offset, T const &t) const {
    if (offset >= _length)
        return -1;
    for (Int32 i = offset; i < _length; ++i)
        if (_buffer[i] == t)
            return i;
    return -1;
}

template <typename T>
Array<T> & Array<T>::Insert(UInt32 const &offset, T const &t) {
    if (offset >= _length)
        return Add(t);
    else {
        EnsureSpace(1);
        for (var i = _length; i > offset; --i) {                         // 挪数据
            new (&_buffer[i]) T((T&&)_buffer[i - 1]);                     // 姑且认为前一个元素是右值 以提速复制
            _buffer[i - 1].~T();                                         // 析构 (这要求具备右值的类型, 在被右值复制后要有判断，避免无脑析构)
        }
        new (&_buffer[offset]) T(t);
        ++_length;
        return *this;
    }
}
template <typename T>
Array<T> & Array<T>::Insert(UInt32 const &offset, T &&t) {
    if (offset >= _length)
        return Add(t);
    else {
        EnsureSpace(1);
        for (var i = _length; i > offset; --i) {
            new (&_buffer[i]) T((T&&)_buffer[i - 1]);
            _buffer[i - 1].~T();
        }
        new (&_buffer[offset]) T((T&&)t);                                // 和上面的代码区别就在这一行。调用元素的右值复制构造
        ++_length;
        return *this;
    }
}
template <typename T>
Array<T> & Array<T>::Insert(UInt32 const &offset, T const * const &a, UInt32 const &len) {
    if (len == 0)
        return *this;
    if (offset >= _length)
        return Add(a, len);
    else {
        EnsureSpace(len);
        for (var i = _length + len; i > offset + len; --i) {
            new (&_buffer[i]) T((T&&)_buffer[i - len]);
            _buffer[i - len].~T();
        }
        for (UInt32 i = 0; i < len; ++i)
            new (&_buffer[i + offset]) T(a[i]);
        _length += len;
        return *this;
    }
}
template <typename T>
template<UInt32 len>
INLINE Array<T> & Array<T>::Insert(UInt32 const &offset, T const (&a)[len]) {
    return Insert(offset, a, len);
}
template <typename T>
INLINE Array<T> & Array<T>::Insert(UInt32 const &offset, Array<T> const &a) {
    return Insert(offset, a.Buffer(), a.Length());
}

template <typename T>
Array<T> & Array<T>::Remove(UInt32 const &offset) {
    for (var i = offset; i < _length; ++i)
        _buffer[i].~T();
    var num = _length - offset;
    _length = offset;
    return *this;
}

template <typename T>
Array<T> & Array<T>::Remove(UInt32 const &offset, UInt32 const &len) {
    if (len == 0)
        return Remove(offset);
    if (offset >= _length)
        return *this;
    else {
        UInt32 len_ = len;
        if (len_ > _length - offset)
            len_ = _length - offset;
        for (var i = offset; i < offset + len_; ++i)
            _buffer[i].~T();                                         // 析构要移除的那一段
        for (var i = offset; i < _length - len_; ++i) {
            new (&_buffer[i]) T((T&&)_buffer[i + len_]);               // 移动 (复制 + 析构)
            _buffer[i + len_].~T();
        }
        _length -= len_;
        return *this;
    }
}

template <typename T>
Array<T> & Array<T>::RemoveAt(UInt32 const &idx) {
    if (idx >= _length)
        return *this;
    else {
        _buffer[idx].~T();                                               // 析构要移除的那一段
        for (var i = idx; i < _length - 1; ++i) {
            new (&_buffer[i]) T((T&&)_buffer[i + 1]);                     // 移动 (复制 + 析构)
            _buffer[i + 1].~T();
        }
        _length -= 1;
        return *this;
    }
}





}
#endif