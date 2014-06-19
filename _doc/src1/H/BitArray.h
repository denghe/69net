#ifndef BITARRAY_H
#define BITARRAY_H
namespace Lib3 {

// 压缩的 Boolean 变长容器 (扩容自动填0, 长度为最大 Index + 1) ( 1 字节存储 8 个 Boolean 对象. 存储方式为 从低地址到高地址依次存 )
// todo: SetAll, BitArray 间位操作
struct BitArray {
    //
    // 每 size_t 存储的 bit 数
    //
    #define                         BitArraySizeTBits                           (sizeof(size_t) * 8)
    //
    // 每 size_t 存储的 bit 数　的位移数　用于优化除法
    // >> ((sizeof(size_t) >> 2) + 4) 相当于 / (sizeof(size_t) * 8)
    //
    #define                         BitArrayShiftNum                            ((sizeof(size_t) >> 2) + 4)
    //
    // 换算 bit 索引为 size_t 索引 (用于算 bit idx 位于哪个 size_t idx 内)
    //
    #define                         BitArrayBitIdxToSizeTIdx(bIdx)              (bIdx >> BitArrayShiftNum)
    BitArray();                                                                 // 初始化 Buffer
    template<UInt32 len>
    BitArray(SByte const (&s)[len]);                                             // 用定长字串内容来初始化 (主用于测试。值范围: 1 0)
    BitArray(BitArray const &ba);                                               // 深度复制
    BitArray &                      operator=(BitArray const &ba);              // 深度复制
    void                            Resize(UInt32 const &idx);                  // 扩容到指定 Bit 索引
    BitArray &                      Add(Boolean const &b);                      // 在最后面追加一个 Bit
    BitArray &                      SetTrue(UInt32 const &idx);                 // 设置某 bit 为 1
    BitArray &                      SetFalse(UInt32 const &idx);                // 设置某 bit 为 0
    BitArray &                      Set(UInt32 const &idx, Boolean const &b);   // 设置某 bit 为 true 1 / false 0
    Boolean                         Get(UInt32 const &idx) const;               // 取某 bit 的数据
    Boolean                         operator[](UInt32 const &idx) const;        // 取某 bit 的数据
    Boolean                         operator==(BitArray const &ba) const;       // 深度比较 相等返回 true
    Boolean                         operator!=(BitArray const &ba) const;       // 深度比较 不等返回 true
    size_t * &                      Buffer();                                   // 引用 _buffer.Buffer()
    size_t * const &                Buffer() const;                             // 引用 _buffer.Buffer()
    UInt32 &                        Length();                                   // 引用 _length
    UInt32 const &                  Length() const;                             // 引用 _length
private:
    Array<size_t>                   _buffer;                                    // 数据缓冲区
    UInt32                          _length;                                    // 已存储多少 Bit (如果是要改短，会制造尾部脏数据 需要蒙版抹成 0. 否则对相等判断有影响)
};




}
#endif