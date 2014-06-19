#ifndef BINARYFORMATTER_NORMAL_H
#define BINARYFORMATTER_NORMAL_H
namespace Lib3 {


// 压缩掉0的序列化器（实测比不压版只慢少少，所以直接取代），进一步压，可对结果做2次处理（比如调用流式压缩算法）
// todo: more (stack, queue, pool, ....)
struct BinaryFormatter_Normal {

    //
    // 基础去 0 算法相关
    //

    // zeg-zag 算法　将有符号数转为无符号数 (正数 = 正数 * 2, 负数 = 绝对值 * 2 - 1)
    //
    static UInt16 ZegZagEncode( Int16 const &i);                        
    static UInt32 ZegZagEncode( Int32 const &i);                        // 32bit 有符号数： (n << 1) ^ (n >> 31)
    static UInt64 ZegZagEncode( Int64 const &i);                        // 64bit 有符号数： (n << 1) ^ (n >> 63)
	static Byte   ZegZagEncode( SByte const &i);
    static Int16  ZegZagDecode(UInt16 const &i);                        // 将无符号数还原为有符号数
    static Int32  ZegZagDecode(UInt32 const &i);
    static Int64  ZegZagDecode(UInt64 const &i);
	static SByte  ZegZagDecode(Byte   const &i);

    // 64 位考虑用 Mask 方式 （T, Nullable<T> 两个版本)
    // 32 位长度值考虑用 Mask 方式 （T, Nullable<T> 两个版本)
    // 一般 32 位, 16 位 考虑用 7Bit 方式, （T, Nullable<T> 两个版本) Nullable 的情况在首 byte 再多占 1 bit 来表达 ( FF 空值, 01xx xxxx 则表示后面还有 )
    // 8 位考虑用 Mask 方式 （T, Nullable<T> 两个版本) (即 0 ~ 253 是正常值，255 是空值, 254 表示值在第 2 个字节里)

    static void Write7Bit(Binary &bin, UInt64 const &in);    // 将 UInt64 转为每 byte 存 7 bit 的模式 (最高位bit用来存　后面是否还有值. 如果有 9 byte, 最后个 byte 用 8 bit) 写入
    static void Write7Bit(Binary &bin, UInt32 const &in);    // 将 UInt32 转为每 byte 存 7 bit 的模式 (最高位bit用来存　后面是否还有值) 写入
    static void Write7Bit(Binary &bin, UInt16 const &in);    // 将 UInt32 转为每 byte 存 7 bit 的模式 (最高位bit用来存　后面是否还有值) 写入
    static ConvertStates Read7Bit(Binary &bin, UInt64 &out);
    static ConvertStates Read7Bit(Binary &bin, UInt32 &out);
    static ConvertStates Read7Bit(Binary &bin, UInt16 &out);

    static void WriteMask(Binary &bin, UInt64 const &in);    // 00 表示 0, 首字节每 Bit 存放相应 Byte 是否为 0, 后面依次存有值的byte (共1~9 bytes)
    static void WriteMask(Binary &bin, UInt32 const &in);    // 小于 FF - 15(4bit) 直接写 (1 byte)，否则生成 FF - 15 + 1~15 和 值Byte0/Byte1/.../Byte0123 (共1~5 bytes)
    static void WriteMask(Binary &bin, UInt16 const &in);    // 小于 FF - 3 (2bit) 直接写 (1 byte)，否则生成 FF - 3 + 1~3   和 值Byte0/Byte1/Byte01       (共1~3 bytes)
    static void WriteMask(Binary &bin, Byte   const &in);    // 小于 FF - 1 (1bit) 直接写 (1 byte)，否则生成 FF - 1  + 1    和 值Byte0                    (共1~2 bytes)
    static ConvertStates ReadMask(Binary &bin, UInt64 &out);
    static ConvertStates ReadMask(Binary &bin, UInt32 &out);
    static ConvertStates ReadMask(Binary &bin, UInt16 &out);
    static ConvertStates ReadMask(Binary &bin, Byte   &out);

    static void Write7Bit(Binary &bin, Nullable<UInt32> const &in);    // FF 表示 空，将 UInt32 转为每 byte 存 7 bit 的模式 (最高位bit用来存　后面是否还有值) 写入 (首byte多占1位用来表达空)
    static void Write7Bit(Binary &bin, Nullable<UInt16> const &in);    // FF 表示 空，将 UInt32 转为每 byte 存 7 bit 的模式 (最高位bit用来存　后面是否还有值) 写入 (首byte多占1位用来表达空)
    static ConvertStates Read7Bit(Binary &bin, Nullable<UInt32> &out);
    static ConvertStates Read7Bit(Binary &bin, Nullable<UInt16> &out);
                          
    static void WriteMask(Binary &bin, Nullable<UInt64> const &in);    // FF 表示 空，00 表示 0, Bit0~5 分别对应 Byte0~5, Bit6 对应 Byte6+7, 每 Bit 存放相应 Byte 是否为 0
    static void WriteMask(Binary &bin, Nullable<UInt32> const &in);    // FF 表示 空，小于 FE - 15(4bit) 直接写 (1 byte)，否则生成 FE - 15 + 1~15 和 值Byte0/Byte1/.../Byte0123 (共2~5 bytes)
    static void WriteMask(Binary &bin, Nullable<UInt16> const &in);    // FF 表示 空，小于 FE - 3(2bit) 直接写 (1 byte)，否则生成 FE - 3 + 1~3 和 值Byte0/Byte1/Byte01 (共2~3 bytes)
    static void WriteMask(Binary &bin, Nullable<Byte  > const &in);    // FF 表示 空，小于 FE - 1(1bit) 直接写 (1 byte)，否则生成 FE 和 值 (2 bytes)
    static ConvertStates ReadMask(Binary &bin, Nullable<UInt64> &out);
    static ConvertStates ReadMask(Binary &bin, Nullable<UInt32> &out);
    static ConvertStates ReadMask(Binary &bin, Nullable<UInt16> &out);
    static ConvertStates ReadMask(Binary &bin, Nullable<Byte  > &out);


    //
    // DbSet 读写相关
    //
    struct DbSetHandler {
        typedef void                        (* WriteColumnDataFunc_t)(Binary &bin, void * const &list);
        template<typename T> static void    WriteColumnDataFunc(Binary &bin, void * const &list);
        static const  WriteColumnDataFunc_t WriteColumnDataFuncs[];

        typedef ConvertStates               (* ReadColumnDataFunc_t)(Binary &bin, void * const &list);
        template<typename T> static ConvertStates ReadColumnDataFunc(Binary &bin, void * const &list);
        static const  ReadColumnDataFunc_t  ReadColumnDataFuncs[];

        typedef void                        (* CreateColumnDataFunc_t)(List<void*> &a);
        template<typename T> static void    CreateColumnDataFunc(List<void*> &a);
        static const  CreateColumnDataFunc_t CreateColumnDataFuncs[];

    };


    // 基础函数
    template<typename T>
    static void Write(Binary &bin, T const &in); // 非 Lib 内置类型 需要自己实现 SaveTo(Binary) 和 LoadFrom(Binary) 下面有Lib内置类型的重载
    // 容器重载
    template<typename T> static void Write(Binary &bin, Nullable<T>    const &in); // 先写是否有值的标记位: 无值 FF, 有值 01 再看情况写值。后面有大量重载，有值态直接存值
    template<typename T> static void Write(Binary &bin, Array<T>       const &in); // 写长度, 内容遍历调单个写入函数。后面有基于可 memcpy 的大量重载
    template<typename T> static void Write(Binary &bin, List<T>        const &in); // 写长度, 内容遍历调单个写入函数。后面有基于可 memcpy 的大量重载
    template<typename T> static void Write(Binary &bin, LinkedList<T>  const &in); // 写长度, 内容遍历调单个写入函数
    template<typename TK, typename TV>
    static void Write(Binary &bin, Dictionary<TK, TV> const &in); // 写长度, 内容遍历调单个写入函数（Key, Value, Key, Value, ...)
    // 数组重载
    template<typename T>
    static void Write(Binary &bin, T const * const &in, UInt32 const &len); // 不写长度，遍历写入数据。后面有基于可 memcpy 的大量重载
    template<typename T, UInt32 len>
    static void Write(Binary &bin, T const (&in)[len]);     // 定长数组版 (不写长度)

    
    template<typename T>
    static ConvertStates Read(Binary &bin, T &out);     // 原则是所有值在 Load 之后，其原值将被清除，容器 Buffer 将不会变小，反复利用已有内存，尽量不重新 new

    template<typename T> static ConvertStates Read(Binary &bin, Nullable<T>    &out);
    template<typename T> static ConvertStates Read(Binary &bin, Array<T>       &out);
    template<typename T> static ConvertStates Read(Binary &bin, List<T>        &out);
    template<typename T> static ConvertStates Read(Binary &bin, LinkedList<T>  &out);
    template<typename TK, typename TV>
    static ConvertStates Read(Binary &bin, Dictionary<TK, TV> &out);

    template<typename T>
    static ConvertStates Read(Binary &bin, T * const &out, UInt32 const &len);
    template<typename T, UInt32 len>
    static ConvertStates Read(Binary &bin, T (&out)[len]);


    // 内置简单数据类型重载
    //
    static void Write(Binary &bin, Byte      const &in); // 不压
    static void Write(Binary &bin, UInt16    const &in); // Write7Bit
    static void Write(Binary &bin, UInt32    const &in); // Write7Bit
    static void Write(Binary &bin, UInt64    const &in); // WriteMask
    static void Write(Binary &bin, SByte     const &in); // 不压
    static void Write(Binary &bin, Int16     const &in); // Write7Bit(ZegZagEncode
    static void Write(Binary &bin, Int32     const &in); // Write7Bit(ZegZagEncode
    static void Write(Binary &bin, Int64     const &in); // WriteMask(ZegZagEncode
    static void Write(Binary &bin, Double    const &in); // 同 Int64
    static void Write(Binary &bin, Single    const &in); // 同 Int32
    static void Write(Binary &bin, Char      const &in); // 同 UInt16
    static void Write(Binary &bin, Boolean   const &in); // 不压
    static void Write(Binary &bin, DateTime  const &in); // 同 UInt64
    static void Write(Binary &bin, Guid      const &in); // 不压
    static void Write(Binary &bin, Decimal   const &in); // 同 UInt64
    static void Write(Binary &bin, BitArray  const &in); // 写长度，内容不压
    static void Write(Binary &bin, SString   const &in); // 同上
    static void Write(Binary &bin, String    const &in); // 同上
    static void Write(Binary &bin, DbSet     const &in); // 顺序写所有元素
    static void Write(Binary &bin, DbTable   const &in); // 顺序写所有元素
    static void Write(Binary &bin, Binary    const &in); // 写长度，内容 memcpy

    static ConvertStates Read(Binary &bin, Byte      &out);
    static ConvertStates Read(Binary &bin, UInt16    &out);
    static ConvertStates Read(Binary &bin, UInt32    &out);
    static ConvertStates Read(Binary &bin, UInt64    &out);
    static ConvertStates Read(Binary &bin, SByte     &out);
    static ConvertStates Read(Binary &bin, Int16     &out);
    static ConvertStates Read(Binary &bin, Int32     &out);
    static ConvertStates Read(Binary &bin, Int64     &out);
    static ConvertStates Read(Binary &bin, Double    &out);
    static ConvertStates Read(Binary &bin, Single    &out);
    static ConvertStates Read(Binary &bin, Char      &out);
    static ConvertStates Read(Binary &bin, Boolean   &out);
    static ConvertStates Read(Binary &bin, DateTime  &out);
    static ConvertStates Read(Binary &bin, Guid      &out);
    static ConvertStates Read(Binary &bin, Decimal   &out);
    static ConvertStates Read(Binary &bin, BitArray  &out);
    static ConvertStates Read(Binary &bin, SString   &out);
    static ConvertStates Read(Binary &bin, String    &out);
    static ConvertStates Read(Binary &bin, DbSet     &out);
    static ConvertStates Read(Binary &bin, DbTable   &out);
    static ConvertStates Read(Binary &bin, Binary    &out);


    // 可 memcpy 提速的数组类重载
    //
    static void Write(Binary &bin, Byte  const * const &in, UInt32 const &len); // memcpy
    static void Write(Binary &bin, SByte const * const &in, UInt32 const &len); // memcpy
    static void Write(Binary &bin, Char  const * const &in, UInt32 const &len); // 条件 memcpy

    static ConvertStates Read(Binary &bin, Byte  * const &out, UInt32 const &len);
    static ConvertStates Read(Binary &bin, SByte * const &out, UInt32 const &len);
    static ConvertStates Read(Binary &bin, Char  * const &out, UInt32 const &len);

    static void Write(Binary &bin, List<Byte > const &in);
    static void Write(Binary &bin, List<SByte> const &in);
    static void Write(Binary &bin, List<Char > const &in);

    static ConvertStates Read(Binary &bin, List<Byte > &out);
    static ConvertStates Read(Binary &bin, List<SByte> &out);
    static ConvertStates Read(Binary &bin, List<Char > &out);


    //
    // 下面是 Nullable 相关
    //

    // 可空类型单值 增加压缩比的重载 (只涉及常用于结构体属性中的数据类型)
    //
    static void Write(Binary &bin, Nullable<Byte    > const &in);
    static void Write(Binary &bin, Nullable<UInt16  > const &in);
    static void Write(Binary &bin, Nullable<UInt32  > const &in);
    static void Write(Binary &bin, Nullable<UInt64  > const &in);
    static void Write(Binary &bin, Nullable<SByte   > const &in);
    static void Write(Binary &bin, Nullable<Int16   > const &in);
    static void Write(Binary &bin, Nullable<Int32   > const &in);
    static void Write(Binary &bin, Nullable<Int64   > const &in);
    static void Write(Binary &bin, Nullable<Double  > const &in);
    static void Write(Binary &bin, Nullable<Single  > const &in);
    static void Write(Binary &bin, Nullable<Char    > const &in);
    static void Write(Binary &bin, Nullable<Boolean > const &in);    // FF: null   00: false   01: true
    static void Write(Binary &bin, Nullable<DateTime> const &in);
    static void Write(Binary &bin, Nullable<Decimal > const &in);
    static void Write(Binary &bin, Nullable<BitArray> const &in);    // 在长度信息中包含空值
    static void Write(Binary &bin, Nullable<SString > const &in);    // 在长度信息中包含空值
    static void Write(Binary &bin, Nullable<String  > const &in);    // 在长度信息中包含空值
    static void Write(Binary &bin, Nullable<Binary  > const &in);    // 在长度信息中包含空值


    static ConvertStates Read(Binary &bin, Nullable<Byte    > &out);
    static ConvertStates Read(Binary &bin, Nullable<UInt16  > &out);
    static ConvertStates Read(Binary &bin, Nullable<UInt32  > &out);
    static ConvertStates Read(Binary &bin, Nullable<UInt64  > &out);
    static ConvertStates Read(Binary &bin, Nullable<SByte   > &out);
    static ConvertStates Read(Binary &bin, Nullable<Int16   > &out);
    static ConvertStates Read(Binary &bin, Nullable<Int32   > &out);
    static ConvertStates Read(Binary &bin, Nullable<Int64   > &out);
    static ConvertStates Read(Binary &bin, Nullable<Double  > &out);
    static ConvertStates Read(Binary &bin, Nullable<Single  > &out);
    static ConvertStates Read(Binary &bin, Nullable<Char    > &out);
    static ConvertStates Read(Binary &bin, Nullable<Boolean > &out);
    static ConvertStates Read(Binary &bin, Nullable<DateTime> &out);
    static ConvertStates Read(Binary &bin, Nullable<Decimal > &out);
    static ConvertStates Read(Binary &bin, Nullable<BitArray> &out);
    static ConvertStates Read(Binary &bin, Nullable<SString > &out);
    static ConvertStates Read(Binary &bin, Nullable<String  > &out);
    static ConvertStates Read(Binary &bin, Nullable<Binary  > &out);


    // 针对 Nullable<T> 元素集合，先按 Bit 写是否为空，再依次写入 非空数据
    //
    template<typename T> static void Write(Binary &bin, Array<Nullable<T>>      const &in);
    template<typename T> static void Write(Binary &bin, List<Nullable<T>>       const &in);
    template<typename T> static void Write(Binary &bin, LinkedList<Nullable<T>> const &in);
    template<typename T> static void Write(Binary &bin, Nullable<T> const * const &in, UInt32 const &len);

    template<typename T> static ConvertStates Read(Binary &bin, Array<Nullable<T>>      &out);
    template<typename T> static ConvertStates Read(Binary &bin, List<Nullable<T>>       &out);
    template<typename T> static ConvertStates Read(Binary &bin, LinkedList<Nullable<T>> &out);
    template<typename T> static ConvertStates Read(Binary &bin, Nullable<T> * const &out, UInt32 const &len);





    // Boolean 相关重载 (令 Boolean 按 Bit 存取)
    //
    static void Write(Binary &bin, Boolean const * const &in, UInt32 const &len);
    static void Write(Binary &bin, List<Boolean> const &in);
    static void Write(Binary &bin, Nullable<Boolean> const * const &in, UInt32 const &len);
    static void Write(Binary &bin, List<Nullable<Boolean>> const &in);

    static ConvertStates Read(Binary &bin, Boolean * const &out, UInt32 const &len);
    static ConvertStates Read(Binary &bin, List<Boolean> &out);
    static ConvertStates Read(Binary &bin, Nullable<Boolean> * const &out, UInt32 const &len);
    static ConvertStates Read(Binary &bin, List<Nullable<Boolean>> &out);


};



}
#endif
