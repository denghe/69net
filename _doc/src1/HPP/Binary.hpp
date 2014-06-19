#ifndef BINARY_HPP
#define BINARY_HPP
namespace Lib3 {


INLINE Binary::Binary() {
    _length         = 0;
    _buffer         = _originalBuffer;
    _bufferLength   = Binary_OriginalBufferLength;
    _bufferState    = BufferState_Original;
    _offset         = 0;
}


INLINE Binary::~Binary() {
    if (_bufferState == BufferState_New)
        delete [] _buffer;
}


INLINE Binary::Binary(UInt32 const &len) {
    _length         = 0;
    _offset         = 0;
#ifdef __GNUC__
    _buffer         = NULL;               // 防 GCC 编译报未初始化
    _bufferState    = BufferState_Original;
    _bufferLength   = 0;
#endif
    if (len <= Binary_OriginalBufferLength) {
        _buffer         = _originalBuffer;
        _bufferState    = BufferState_Original;
        _bufferLength   = Binary_OriginalBufferLength;
    } else {
        _buffer         = new Byte[len];
        _bufferLength   = len;
        _bufferState    = BufferState_New;
    }
}


INLINE Binary::Binary(Byte const * const &s, UInt32 const &len) {
    _length         = len;
    _offset         = 0;
#ifdef __GNUC__
    _buffer         = NULL;
    _bufferState    = BufferState_Original;
    _bufferLength   = 0;
#endif
    if (len <= Binary_OriginalBufferLength) {
        _buffer         = _originalBuffer;
        _bufferState    = BufferState_Original;
        _bufferLength   = Binary_OriginalBufferLength;
        memcpy(_originalBuffer, s, len);
    } else {
        _buffer         = new Byte[len];
        _bufferState    = BufferState_New;
        _bufferLength   = len;
        memcpy(_buffer, s, len);
    }
}



template<UInt32 len>
Binary::Binary(Byte const (&s)[len]) {
    _length         = len;
    _offset         = 0;
#ifdef __GNUC__
    _buffer         = NULL;
    _bufferState    = 0;
    _bufferLength   = 0;
#endif
    if (len <= Binary_OriginalBufferLength) {
        _buffer         = _originalBuffer;
        _bufferState    = BufferState_Original;
        _bufferLength   = Binary_OriginalBufferLength;
        memcpy(_originalBuffer, s, len);
    } else {
        _buffer         = new Byte[len];
        _bufferState    = BufferState_New;
        _bufferLength   = len;
        memcpy(_buffer, s, len);
    }
}


template<Int32 len>
Binary::Binary(SByte const (&s)[len]) {
    SString ss(s);
    ss.TrimAllSelf(" \t\n\r", 4);
    assert ( ss.Length() % 2 == 0 );
    _length         = ss.Length() >> 1;
    _offset         = 0;
#ifdef __GNUC__
    _buffer         = NULL;
    _bufferState    = BufferState_Original;
    _bufferLength   = 0;
#endif
    if (_length <= Binary_OriginalBufferLength) {
        _buffer         = _originalBuffer;
        _bufferState    = BufferState_Original;
        _bufferLength   = Binary_OriginalBufferLength;
    } else {
        _buffer         = new Byte[_length];
        _bufferState    = BufferState_New;
        _bufferLength   = _length;
    }
    for (UInt32 i = 0; i < ss.Length(); i += 2) {
        Byte n = 0;
        var c = ss[i];
        if (c >= '0' && c <= '9')
            n = (c - '0') << 4;
        else if (c >= 'A' && c <= 'F')
            n = (c - 'A' + 10) << 4;
        else if (c >= 'a' && c <= 'f')
            n = (c - 'a' + 10) << 4;
        else assert ( false );
        c = ss[i + 1];
        if (c >= '0' && c <= '9')
            n |= (c - '0');
        else if (c >= 'A' && c <= 'F')
            n |= (c - 'A' + 10);
        else if (c >= 'a' && c <= 'f')
            n |= (c - 'a' + 10);
        else assert ( false );
        _buffer[i >> 1] = n;
    }
}


INLINE Binary::Binary(UInt32 const &len, Byte * const &s) {
    assert ( s );
    _length         = len;
    _buffer         = (Byte*)s;
    _bufferState    = BufferState_Custom;
    _bufferLength   = len;
    _offset         = 0;
}


INLINE Binary::Binary(Binary const &s) {
    _length         = s.Length();
    _offset         = 0;
#ifdef __GNUC__
    _buffer         = NULL;
    _bufferState    = BufferState_Original;
    _bufferLength   = 0;
#endif
    if (_length <= Binary_OriginalBufferLength) {
        _buffer         = _originalBuffer;
        _bufferState    = BufferState_Original;
        _bufferLength   = Binary_OriginalBufferLength;
        memcpy(_originalBuffer, s.Buffer(), _length);
    } else {
        _buffer         = new Byte[_length];
        _bufferState    = BufferState_New;
        _bufferLength   = _length;
        memcpy(_buffer, s.Buffer(), _length);
    }
}


INLINE Binary & Binary::Set(Binary const &s) {
    return Set(s.Buffer(), s.Length());
}


INLINE Binary & Binary::Set(UInt32 const &len, Byte * const &s) {
    if (_bufferState == BufferState_New)
        delete [] _buffer;
    _offset         = 0;
    _length         = len;
    _bufferLength   = len;
    _buffer         = s;
    _bufferState    = BufferState_Custom;
    return *this;
}


INLINE Binary & Binary::Set(Byte const * const &s, UInt32 const &len) {
    _length         = len;
    _offset         = 0;
    switch (_bufferState) {
    case BufferState_Original:
        if (len <= Binary_OriginalBufferLength)
            memcpy(_originalBuffer, s, len);
        else {
            _buffer         = new Byte[len];
            _bufferLength   = len;
            _bufferState    = BufferState_New;
        }
        break;
    case BufferState_New:
        if (len <= _bufferLength)
            memcpy(_originalBuffer, s, len);
        else {
            delete [] _buffer;
            _buffer         = new Byte[len];
            _bufferLength   = len;
        }
        break;
    case BufferState_Custom:
        if (len <= Binary_OriginalBufferLength) {
            _buffer         = _originalBuffer;
            _bufferState    = BufferState_New;
            _bufferLength   = Binary_OriginalBufferLength;
            memcpy(_originalBuffer, s, len);
        }
        else {
            _buffer         = new Byte[len];
            _bufferLength   = len;
            _bufferState    = BufferState_New;
        }
        break;
    }
    return *this;
}


INLINE Binary & Binary::operator=(Binary const &s) {
    return Set(s);
}


INLINE Int32 Binary::Compare(Binary const &o) const {
    var len = o.Length();
    if (_length == 0) {
        if (len == 0)
            return 0;
        else
            return -1;
    }
    else {
        if (len == 0)
            return 1;
        else {
            if (_length < len)      // len 存比较小的尺寸
                len = _length;
            UInt32 m = len % sizeof(size_t);
            var p1 = (size_t*)_buffer;
            var p2 = (size_t*)o.Buffer();
            for (UInt32 i = 0; i < (len - m) / sizeof(size_t); ++i) {
                if (p1[i] < p2[i])
                    return -1;
                else if (p1[i] > p2[i])
                    return 1;
            }
            for (UInt32 i = len - m; i < len; ++i) {
                if (p1[i] < p2[i])
                    return -1;
                else if (p1[i] > p2[i])
                    return 1;
            }
            if (_length == o.Length())
                return 0;
            else if (len == _length)
                return -1;
            else return 1;
        }
    }
}


INLINE Boolean Binary::operator==(Binary const &o) const {
    return Compare(o) == 0;
}

INLINE Boolean Binary::operator!=(Binary const &o) const {
    return Compare(o) != 0;
}

INLINE Boolean Binary::operator< (Binary const &o) const {
    return Compare(o) < 0;
}

INLINE Boolean Binary::operator<=(Binary const &o) const {
    return Compare(o) <= 0;
}

INLINE Boolean Binary::operator> (Binary const &o) const {
    return Compare(o) > 0;
}

INLINE Boolean Binary::operator>=(Binary const &o) const {
    return Compare(o) >= 0;
}

INLINE Byte * & Binary::Buffer() {
    return _buffer;
}

INLINE Byte * const & Binary::Buffer() const {
    return _buffer;
}

INLINE UInt32 & Binary::Length() {
    return _length;
}

INLINE UInt32 & Binary::Offset() {
    return _offset;
}

INLINE UInt32 const & Binary::Length() const {
    return _length;
}

INLINE UInt32 const & Binary::Offset() const {
    return _offset;
}

INLINE void Binary::Clear() {
    _length = 0;
    _offset = 0;
}


INLINE void Binary::EnsureSpace(UInt32 const &len) {
    if (len > _bufferLength - _length) {
        var bak_buff    = _buffer;
        var bak_stat    = _bufferState;
        _bufferLength   = (_length + len) * 2;
        _buffer         = new Byte[_bufferLength];
        _bufferState    = BufferState_New;
        memcpy(_buffer, bak_buff, _length);
        if (bak_stat == BufferState_New)
            delete [] bak_buff;
    }
}


INLINE void Binary::EnsureLength() {
    if (_length > _bufferLength) {
        var bak_buff    = _buffer;
        var bak_stat    = _bufferState;
        _bufferLength   = _length;
        _buffer         = new Byte[_bufferLength];
        _bufferState    = BufferState_New;
        memcpy(_buffer, bak_buff, _length);
        if (bak_stat == BufferState_New)
            delete [] bak_buff;
    }
}

//
//INLINE void Binary::EnsureLength(UInt32 const &len) {
//    _length = len;
//    EnsureLength();
//}


template<typename T>
INLINE Binary & Binary::Write(T const &in) {
    FT::Write(*this, in);
    return *this;
}

template<typename T>
INLINE Binary & Binary::Write(T const * const &in, UInt32 const &inLen) {
    FT::Write(*this, in, inLen);
    return *this;
}

template<typename T, UInt32 len>
INLINE Binary & Binary::Write(T const (&in)[len]) {
    FT::Write(*this, in);
    return *this;
}


template<typename T>
INLINE ConvertStates Binary::Read(T &out) {
    return FT::Read(*this, out);
}

template<typename T>
INLINE ConvertStates Binary::Read(T * const &out, UInt32 const &len) {
    return FT::Read(*this, out, len);
}

template<typename T, UInt32 len>
INLINE ConvertStates Binary::Read(T const (&out)[len]) {
    return FT::Read(*this, out, len);
}

template<typename T>
INLINE Binary & Binary::operator<<(T const &in) {
    return Write(in);
}

template<typename T, UInt32 len>
INLINE Binary & Binary::operator<<(T const (&in)[len]) {
    return Write(in);
}




}
#endif


