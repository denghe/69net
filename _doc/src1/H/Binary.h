#ifndef BINARY_H
#define BINARY_H
namespace Lib3 {


struct BinaryFormatter_Normal;


// 变长 Byte[] 容器 (带 4k 初始Buff, 主用于网络[反]序列化)
// 自定义类须实现 SaveTo(Binary<FT> bin) 以及 LoadFrom(Binary<FT> bin),  Dispose()
// FT 对应 BinaryFormatter_Simple, 以及 BinaryFormatter_Normal, 如果有自写 FT, 也得写相应的 SaveTo, LoadFrom
// Dispose() 用于 LoadFrom 执行失败的回滚操作，也用于调用 LoadFrom 之前的数据清理操作 (常见于反复使用同一容器的情况, 需要用户手工调)
// LoadFrom 相当于 new placement 调用构造函数的效果，故无法知道先前是否已经初始化过要不要回收，故总是无脑覆盖
// 本类不提供表达式相关操作 故不写右值相关

// template<typename FT = BinaryFormatter_Normal>                               // 为了简化　注释掉 Formatter 可切换部分，Formatter 可切换代码在 Bak 里有备份
struct Binary {
    typedef BinaryFormatter_Normal FT;                                          // 为了简化　直接 typedef Formatter
    enum Settings {
        Binary_OriginalBufferLength            = 4096
    };
    Binary();                                                                   // Buffer = OriginalBuffer, BufferLength = OriginalLength, Length = 0, BufferState = BufferState_Original
    ~Binary();                                                                  // 当 BufferState == BufferState_New 时 释放 Buffer
    explicit Binary(UInt32 const &len);                                         // 按传入容器长初始化（原始 Buffer 长度不足将新建）
    Binary(UInt32 const &len, Byte * const &s);                                 // 按传入的数据来初始化（引用：BufferState_Custom）
    Binary(Byte const * const &s, UInt32 const &len);                           // 按传入的数据来初始化（复制）
    template<UInt32 len>
    Binary(Byte const (&s)[len]);                                               // 按传入的数据来初始化 (定长数组版)
    template<Int32 len>
    Binary(SByte const (&s)[len]);                                               // 可以用 16 进制资源字串来初始化
    Binary(Binary const &o);                                                    // 复制
    Binary &                        Set(Binary const &o);                       // 只复制数据, 尽量利用已经创建的空间
    Binary &                        Set(UInt32 const &len, Byte * const &s);    // 将当前内容清除并设置新值（引用）
    Binary &                        Set(Byte const * const &s, UInt32 const &len);   // 重置长度/保障，复制
    Binary &                        operator=(Binary const &o);                 // 同 Set
    Int32                           Compare(Binary const &o) const;             // 相同返回 0, 小于 o 返回 -1, 大于 o 返回 1. 判定规则同 SString (左对齐)
	Boolean                         operator==(Binary const &o) const;          // 比较 Length 和 内容是否相等
	Boolean                         operator!=(Binary const &o) const;
	Boolean                         operator< (Binary const &o) const;
	Boolean                         operator<=(Binary const &o) const;
	Boolean                         operator> (Binary const &o) const;
	Boolean                         operator>=(Binary const &o) const;
    Byte * &                        Buffer();                                   // _buffer 的引用封装
    UInt32 &                        Length();                                   // _length 的引用封装
    UInt32 &                        Offset();                                   // _offset 的引用封装
    Byte * const &                  Buffer() const;                             // _buffer 的引用封装
    UInt32 const &                  Length() const;                             // _length 的引用封装
    UInt32 const &                  Offset() const;                             // _offset 的引用封装
    void                            Clear();                                    // 将 Length 和 Offset 清 0, 其他不变
    void                            EnsureSpace(UInt32 const &len);             // 检查 Buffer 剩余空间大小，根据情况 Resize. if len > BufferLength - Length, 则扩容为 (Length + len) * 2
    void                            EnsureLength();                             // 检查 BufferLength, if Length > BufferLength, 则扩容为 Length  (常用于复制数据进来之前先改 Length 再调这个函数确保空间足够)
    //void                            EnsureLength(UInt32 const &len);            // 同上. 先令 Length = len
    template<typename T>
    Binary &                        Write(T const &in);                         // 写数据。EnsureSpace, Length += sizeof(in)
    template<typename T>
    Binary &                        Write(T const * const &in, UInt32 const &inLen); // 写 长度 + 数组数据。EnsureSpace, Length += sizeof(in * inLen)
    template<typename T, UInt32 len>
    Binary &                        Write(T const (&in)[len]);                  // 定长数组版 (不写长度)

    template<typename T>
    ConvertStates                   Read(T &out);                               // 读操作。将递增 offset, 尝试将值写入 out, 返回操作结果
    template<typename T>
    ConvertStates                   Read(T * const &out, UInt32 const &len);    // 读数组操作。尝试读出 outLen 个 T 写入 out 指针处, 返回操作结果
    template<typename T, UInt32 len>
    ConvertStates                   Read(T const (&out)[len]);                  // 定长数组版 (不读长度, 直接读 len 个元素)
    template<typename T>
    Binary &                        operator<<(T const &in);                    // 同 Write
    template<typename T, UInt32 len>
    Binary &                        operator<<(T const (&in)[len]);             // 同 Write
private:
    ALIGN8( Byte                    _originalBuffer[Binary_OriginalBufferLength] ); // 初始 Buff
    Byte *                          _buffer;                                    // 容器指针 指向实际存储数据的内存起始
    UInt32                          _bufferLength;                              // 容器尺寸
    UInt32                          _length;                                    // 数据有效长度
    BufferStates                    _bufferState;                               // Buffer 状态标识
    mutable UInt32                  _offset;                                    // 读操作用到的偏移量 (读的时候会修改该值)
};




}
#endif