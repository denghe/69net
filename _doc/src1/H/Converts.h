#ifndef CONVERTS_H
#define CONVERTS_H
namespace Lib3 {
namespace Converts {




// 转换用表
// 将 0 ~ 15 映射成相应的 ASCII
ALIGN8 ( SByte const Int2HexTable[] ) = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
// 将 0 ~ 9, a ~ f, A ~ F 的 ASCII 映射成相应的 int 值
Int32 const Hex2IntTable[] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
	2,  3,  4,  5,  6,  7,  8,  9,  0,  0,
	0,  0,  0,  0,  0,  10, 11, 12, 13, 14,
	15, 0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  10, 11, 12,
	13, 14, 15
};


// 返回转为 10 进制后的字长
UInt32 GetLength(UInt32 const &in);
UInt32 GetLength(UInt64 const &in);


// 该系列函数均为不安全操作　不判断长度　不判断越界  只以 \0 作为串结尾


// 将 10 进制数值字串转为相应的值类型 (主要为 mysql 查询结果转为值类型服务)
void Convert(SByte const * const &in, Byte      &out);
void Convert(SByte const * const &in, UInt16    &out);
void Convert(SByte const * const &in, UInt32    &out);
void Convert(SByte const * const &in, UInt64    &out);
void Convert(SByte const * const &in, SByte     &out);
void Convert(SByte const * const &in, Int16     &out);
void Convert(SByte const * const &in, Int32     &out);
void Convert(SByte const * const &in, Int64     &out);
void Convert(SByte const * const &in, Double    &out);
void Convert(SByte const * const &in, Single    &out);
void Convert(SByte const * const &in, Char      &out);
void Convert(SByte const * const &in, Boolean   &out);
void Convert(SByte const * const &in, DateTime  &out);
void Convert(SByte const * const &in, Guid      &out);
void Convert(SByte const * const &in, Decimal   &out);
// 将 Char* 转为值的先不做

// 将值类型转换为字符串 返回转换后的串长 (主要为 Append, Write 等函数服务)
UInt32 Convert(Byte      const &in, SByte * const &out);
UInt32 Convert(UInt16    const &in, SByte * const &out);
UInt32 Convert(UInt32    const &in, SByte * const &out);
UInt32 Convert(UInt64    const &in, SByte * const &out);
UInt32 Convert(SByte     const &in, SByte * const &out);
UInt32 Convert(Int16     const &in, SByte * const &out);
UInt32 Convert(Int32     const &in, SByte * const &out);
UInt32 Convert(Int64     const &in, SByte * const &out);
UInt32 Convert(Double    const &in, SByte * const &out);
UInt32 Convert(Single    const &in, SByte * const &out);
UInt32 Convert(Char      const &in, SByte * const &out);
UInt32 Convert(Boolean   const &in, SByte * const &out);
UInt32 Convert(DateTime  const &in, SByte * const &out);
UInt32 Convert(Guid      const &in, SByte * const &out);
UInt32 Convert(Decimal   const &in, SByte * const &out);

UInt32 Convert(Byte      const &in, Char * const &out);
UInt32 Convert(UInt16    const &in, Char * const &out);
UInt32 Convert(UInt32    const &in, Char * const &out);
UInt32 Convert(UInt64    const &in, Char * const &out);
UInt32 Convert(SByte     const &in, Char * const &out);
UInt32 Convert(Int16     const &in, Char * const &out);
UInt32 Convert(Int32     const &in, Char * const &out);
UInt32 Convert(Int64     const &in, Char * const &out);
UInt32 Convert(Double    const &in, Char * const &out);
UInt32 Convert(Single    const &in, Char * const &out);
UInt32 Convert(Char      const &in, Char * const &out);
UInt32 Convert(Boolean   const &in, Char * const &out);
UInt32 Convert(DateTime  const &in, Char * const &out);
UInt32 Convert(Guid      const &in, Char * const &out);
UInt32 Convert(Decimal   const &in, Char * const &out);

// 将值类型转换为 16进制的字符串 返回转换后的串长 (主要为 dump, console 等 服务)
UInt32 Convert_Hex(Byte      const &in, SByte * const &out);
UInt32 Convert_Hex(UInt16    const &in, SByte * const &out);
UInt32 Convert_Hex(UInt32    const &in, SByte * const &out);
UInt32 Convert_Hex(UInt64    const &in, SByte * const &out);
UInt32 Convert_Hex(SByte     const &in, SByte * const &out);
UInt32 Convert_Hex(Int16     const &in, SByte * const &out);
UInt32 Convert_Hex(Int32     const &in, SByte * const &out);
UInt32 Convert_Hex(Int64     const &in, SByte * const &out);
UInt32 Convert_Hex(Double    const &in, SByte * const &out);
UInt32 Convert_Hex(Single    const &in, SByte * const &out);
UInt32 Convert_Hex(Char      const &in, SByte * const &out);
UInt32 Convert_Hex(Boolean   const &in, SByte * const &out);
UInt32 Convert_Hex(DateTime  const &in, SByte * const &out);
UInt32 Convert_Hex(Guid      const &in, SByte * const &out);
UInt32 Convert_Hex(Decimal   const &in, SByte * const &out);

UInt32 Convert_Hex(Byte      const &in, Char * const &out);
UInt32 Convert_Hex(UInt16    const &in, Char * const &out);
UInt32 Convert_Hex(UInt32    const &in, Char * const &out);
UInt32 Convert_Hex(UInt64    const &in, Char * const &out);
UInt32 Convert_Hex(SByte     const &in, Char * const &out);
UInt32 Convert_Hex(Int16     const &in, Char * const &out);
UInt32 Convert_Hex(Int32     const &in, Char * const &out);
UInt32 Convert_Hex(Int64     const &in, Char * const &out);
UInt32 Convert_Hex(Double    const &in, Char * const &out);
UInt32 Convert_Hex(Single    const &in, Char * const &out);
UInt32 Convert_Hex(Char      const &in, Char * const &out);
UInt32 Convert_Hex(Boolean   const &in, Char * const &out);
UInt32 Convert_Hex(DateTime  const &in, Char * const &out);
UInt32 Convert_Hex(Guid      const &in, Char * const &out);
UInt32 Convert_Hex(Decimal   const &in, Char * const &out);

// 将 Hex 转为值的其他重载先不做
void Convert_Hex(SByte const * const &in, Byte   &out);
void Convert_Hex(SByte const * const &in, UInt16 &out);
void Convert_Hex(SByte const * const &in, UInt32 &out);
void Convert_Hex(SByte const * const &in, UInt64 &out);





// Unicode , Utf8 互转 （当前只实现了 UCS2 的字符集转换，即：2 字节）
// 获取 utf8 转为 unicode 之后的长度
UInt32 Convert_Unicode_GetLength(SByte const * const &in);
UInt32 Convert_Unicode_GetLength(SByte const * const &inStr, UInt32 const &inLen);

// 获取 unicode 转为 utf8 之后的长度
UInt32 Convert_Utf8_GetLength(Char const * const &in);
UInt32 Convert_Utf8_GetLength(Char const * const &inStr, UInt32 const &inLen);

// utf8 转为 unicode(2 ~ 4字节), 返回实际填充长度 (最后不填充 \0)
// 相当于 windows 下的 MultiByteToWideChar(CP_UTF8, 0, in, -1, out, dLen);  这个实现要比 windows 的快 1/4
UInt32 Convert_Unicode(SByte const * const &inStr, Char * const &outWStr);
UInt32 Convert_Unicode(SByte const * const &inStr, UInt32 const &inLen, Char * const &outWStr);

// unicode(2 ~ 4字节, gcc 请指定 -fshort-wchar 参数) 转为 utf8, 返回实际填充长度 (最后不填充 \0)
// 相当于 windows 下的 WideCharToMultiByte(CP_UTF8, 0, in, -1, out, dLen, NULL, NULL ); 这个实现要比 windows 的快 1 倍
UInt32 Convert_Utf8(Char const * const &inWStr, SByte * const &outStr);
UInt32 Convert_Utf8(Char const * const &inWStr, UInt32 const &inLen, SByte * const &outStr);

// 单个 Char 转为 Utf8 返回转换后的长度
UInt32 Convert_Utf8(Char const &in, SByte * const &out);






}}
#endif
