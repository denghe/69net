#ifndef CONSOLE_H
#define CONSOLE_H
namespace Lib3 {
namespace Console {


void SetLocale();                                                       // 快速初始化 Locale
void Pause();                                                           // 输出按任意键继续并暂停
void WriteNull();                                                       // 用于输出一个表达 空值 的字串
void WriteChar(Int32 const &c);                                         // 立即输出 1 个字符到控制台/终端



void Write(Boolean   const &in);                                        // 输出 true, false
void Write(SByte     const &in);                                        // 输出单个单字节字符
void Write(Char      const &in);                                        // 输出单个 unicode 字符
void Write(Byte      const &in);                                        // 输出 10 进制值
void Write(Int16     const &in);                                        // 输出 10 进制值
void Write(UInt16    const &in);                                        // 输出 10 进制值
void Write(Int32     const &in);                                        // 输出 10 进制值
void Write(UInt32    const &in);                                        // 输出 10 进制值
void Write(Int64     const &in);                                        // 输出 10 进制值
void Write(UInt64    const &in);                                        // 输出 10 进制值
void Write(Single    const &in);                                        // 输出 单精浮点 值
void Write(Double    const &in);                                        // 输出 双精浮点 值
void Write(DateTime  const &in);                                        // 输出 yyyy-MM-dd hh:mm:ss
void Write(Decimal   const &in);                                        // 输出 定点数 值
void Write(Guid      const &in);                                        // 输出 UUID 格式
void Write(BitArray  const &in);                                        // 输出 1(true)0(false) 序列
void Write(DbTable   const &in);                                        // 输出一个表
void Write(DbSet     const &in);                                        // 输出一个表集
void Write(SString   const &in);                                        // 输出 Utf8 编码格式的字串      前后会加上引号
void Write(String    const &in);                                        // 输出 Unicode 编码格式的字串   前后会加上引号
void Write(SByte const * const &in);                                    // 以 0 为结尾的 utf8 串         前后会加上引号
void Write(Char const * const &in);                                     // 以 0 为结尾的 unicode 串      前后会加上引号
void Write(Binary    const &a);                                         // 输出 16 进制
template<typename T> void Write(Nullable<T> const &nv);                 // 空则输出 (Null), 否则输出值
template<typename T> void Write(Array<T> const &a);                     // 逗号 split 输出
template<typename T> void Write(List<T> const &a);                      // 逗号 split 输出
template<typename T> void Write(LinkedList<T> const &a);                // 逗号 split 输出
template<typename TK, typename TV> void Write(Dictionary<TK, TV> const &a); // 逗号 split 输出
template<UInt32 len> void Write(SByte const (&in)[len]);                // 以 0 为结尾的 限长 utf8 串    前后会加上引号
template<UInt32 len> void Write(Char const (&in)[len]);                 // 以 0 为结尾的 限长 unicode 串 前后会加上引号
template<UInt32 len> void Write(Byte const (&s)[len]);                  // 输出 16 进制
template<typename T> void Write(T const &in);                           // 输出支持 void Dump() 函数的对象实例
template<typename T, UInt32 len> void Write(T const (&in)[len]);        // 输出支持 void Dump() 函数的对象实例数组
// 指针 + 长度
void Write(SByte const * const &in, UInt32 const &len);                 // utf8 串 (不判定 0)             前后会加上引号
void Write(Char const * const &in, UInt32 const &len);                  // unicode 串 (不判定 0)          前后会加上引号
void Write(Byte const * const &in, UInt32 const &len);                  // 输出 16 进制
template<typename T> void Write(T const * const &in, UInt32 const &len);// 调上面的函数，逗号 split 输出


/*
WriteHex 相关输出长像：
Len = 9               0  1  2  3  | 4  5  6  7  | 8  9  A  B  | C  D  E  F
00000000              FF FF FF FF   FF FF FF FF   FF .......
........              ..........
*/
void WriteHex(Byte     const &in);                                      // 输出 16 进制值
void WriteHex(UInt16   const &in);                                      // 输出 16 进制值
void WriteHex(UInt32   const &in);                                      // 输出 16 进制值
void WriteHex(UInt64   const &in);                                      // 输出 16 进制值
void WriteHex(Byte const * const &in, UInt32 const &len);               // 以 16 进制方式输出一个 Byte[] 的内容
template<typename T> void WriteHex(T const &in);                        // T 需要支持 DumpHex()



// DbSet 相关
typedef void (* WriteColFunc_t)(void * const &list, UInt32 const &idx);
extern WriteColFunc_t const WriteColFuncs[];
template<Byte ctid> void WriteColFunc(void * const &list, UInt32 const &idx);



template<UInt32 len>
void WriteFormat(Char const (&s)[len]);
template<UInt32 len, typename T0>
void WriteFormat(Char const (&s)[len], T0 const &p0);
template<UInt32 len, typename T0, typename T1>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1);
template<UInt32 len, typename T0, typename T1, typename T2>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);


template<UInt32 len>
void WriteFormat(SByte const (&s)[len]);
template<UInt32 len, typename T0>
void WriteFormat(SByte const (&s)[len], T0 const &p0);
template<UInt32 len, typename T0, typename T1>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1);
template<UInt32 len, typename T0, typename T1, typename T2>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);


template<typename T>
void WriteFormat(T const &t);
template<typename T0>
void WriteFormat(SString const &str, T0 const &p0);
template<typename T0, typename T1>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1);
template<typename T0, typename T1, typename T2>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2);
template<typename T0, typename T1, typename T2, typename T3>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
template<typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);


template<typename T0>
void WriteFormat(String const &str, T0 const &p0);
template<typename T0, typename T1>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1);
template<typename T0, typename T1, typename T2>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2);
template<typename T0, typename T1, typename T2, typename T3>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
template<typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);















}}
#endif