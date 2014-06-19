#ifndef BITARRAY_HPP
#define BITARRAY_HPP
namespace Lib3 {



template<UInt32 len>
INLINE BitArray::BitArray(SByte const (&s)[len]) {
    _length = 0;
    SString ss(s);
    ss.TrimAllSelf(" \t\r\n", 4);
    for (UInt32 i = 0; i < ss.Length(); ++i)
        Set(i, ss[i] == '1');
}

INLINE BitArray::BitArray() {
    _length = 0;
}

INLINE BitArray::BitArray(BitArray const &ba) {
    _length = ba._length;
    _buffer = ba._buffer;                                                 // todo: 根据 Length >> 3 截取复制
}

INLINE BitArray & BitArray::operator=(BitArray const &ba) {
    _length = ba._length;
    _buffer = ba._buffer;                                                 // todo: 根据 Length >> 3 截取复制
    return *this;
}

INLINE void BitArray::Resize(UInt32 const &idx) {
    var size_t_len = BitArrayBitIdxToSizeTIdx(idx) + 1;                 // 根据 idx 计算 size_t 的保证长度 并保证
    if (_buffer.Length() < size_t_len) {
        _buffer.EnsureSpace(size_t_len - _buffer.Length());
        // 将 BitLength 到长度保证后的 BitLength 这段清 0, 即：跳过 BitLength 所在 SizeTIndex, 将剩下的 SizeT 清 0
        var size_t_idx_start = BitArrayBitIdxToSizeTIdx(_length);
        var len = size_t_len - size_t_idx_start;
        memset(Buffer() + size_t_idx_start, 0, len * sizeof(size_t));
        _buffer.Length() = size_t_len;                                     // 同步 Buffer.Length
    }
    _length = idx + 1;                                                   // 同步 Length
}

INLINE BitArray & BitArray::Add(Boolean const &b) {
    return Set(_length, b);
}

INLINE BitArray & BitArray::SetTrue(UInt32 const &idx) {
    if (idx >= _length)
        Resize(idx);
    var stIdx = BitArrayBitIdxToSizeTIdx(idx);
    var stBitIdx = idx % BitArraySizeTBits;                             // idx - stIdx * SizeTBits
    Buffer()[stIdx] |= ((size_t)1 << stBitIdx);
    return *this;
}

INLINE BitArray & BitArray::SetFalse(UInt32 const &idx) {
    if (idx >= _length)
        Resize(idx);
    var stIdx = BitArrayBitIdxToSizeTIdx(idx);
    var stBitIdx = idx % BitArraySizeTBits;
    Buffer()[stIdx] &= ~((size_t)1 << stBitIdx);
    return *this;
}

INLINE BitArray & BitArray::Set(UInt32 const &idx, Boolean const &b) {
    if (idx >= _length)
        Resize(idx);
    var stIdx = BitArrayBitIdxToSizeTIdx(idx);
    var stBitIdx = idx % BitArraySizeTBits;
    if (b)
        Buffer()[stIdx] |= ((size_t)1 << stBitIdx);
    else
        Buffer()[stIdx] &= ~((size_t)1 << stBitIdx);
    return *this;
}

INLINE Boolean BitArray::Get(UInt32 const &idx) const {
    if (idx >= _length)
        return false;
    var stIdx = BitArrayBitIdxToSizeTIdx(idx);
    var stBitIdx = idx % BitArraySizeTBits;
    return (Buffer()[stIdx] & ((size_t)1 << stBitIdx)) > 0;
}

INLINE Boolean BitArray::operator[](UInt32 const &idx) const {
    if (idx >= _length)
        return false;
    var stIdx = BitArrayBitIdxToSizeTIdx(idx);
    var stBitIdx = idx % BitArraySizeTBits;
    return (Buffer()[stIdx] & ((size_t)1 << stBitIdx)) > 0;
}

INLINE Boolean BitArray::operator==(BitArray const &ba) const {
    if (_length != ba.Length())
        return false;
    for (UInt32 i = 0; i < _length >> 3; ++i)                            // BitLength >> 3 = size_t 数
        if (Buffer()[i] != ba.Buffer()[i])
            return false;
    return true;
}

INLINE Boolean BitArray::operator!=(BitArray const &ba) const {
    return !operator==(ba);
}

INLINE UInt32 & BitArray::Length() {
    return _length;
}
INLINE UInt32 const & BitArray::Length() const {
    return _length;
}
INLINE size_t * & BitArray::Buffer() {
    return _buffer.Buffer();
}
INLINE size_t * const & BitArray::Buffer() const {
    return _buffer.Buffer();
}





}
#endif