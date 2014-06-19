#ifndef STRINGS_H
#define STRINGS_H
namespace Lib3 {
namespace Strings {



// 创建循环使用的 SByte[], Char[] 容器, Convert 填充后返回指针, 无需回收 (不传入 buff 的 ToString 非线程安全)
// todo: 加 volatile , interlock 以确保多线程下运行正确

// 建立 Buffer 容器池和当前索引  以便于在多个参数中嵌套执行 ToString 时能正确的返回不同的容器指针
// 
#define ToString_BuferCount 32

// Buffer Length 为 64 是一个理论安全值, 只要不使用超过 64 字长的 字串类来做 ToString 就不会超过限制
//
#define ToString_BuferLength 64

// 指向当前 Buffer 的 Index
extern UInt32 ToString_CharBuferIndex;
extern UInt32 ToString_WCharBuferIndex;
// 字串临时 Buffer
extern ALIGN8( SByte ToString_CharBuffer[ToString_BuferCount][ToString_BuferLength] );
extern ALIGN8( Char ToString_WCharBuffer[ToString_BuferCount][ToString_BuferLength] );




// 将 o 转为 字串 并填充到某个 SByte Buffer 容器 并返回 容器指针
// 注意: o 的类型通常不可以是 SByte, Char 这些 (超出 ToString_BuferLength 字长将有缓冲区溢出风险)
// 注意2: 非线程安全
template<typename T>
SByte const * ToString(T const &o);

// 将 o 转为 字串 并填充到某个 Char Buffer 容器 并返回 容器指针
// 注意: o 的类型通常不可以是 SByte, Char 这些 (超出 ToString_BuferLength 字长将有缓冲区溢出风险)
// 注意2: 非线程安全
template<typename T>
Char const * ToWString(T const &o);

// 上面两个 ToString 的 Hex 版
template<typename T>
SByte const * ToString_Hex(T const &o);

template<typename T>
Char const * ToWString_Hex(T const &o);


// 将 o 转为 字串 并填充到传入的 SByte Buffer 容器 并返回 容器指针
// 注意: o 的类型通常不可以是 SByte, Char 这些 (缓冲区溢出风险)
// 注意2: 传入的 buf 长度至少为 32
template<typename T>
SByte const * ToString(T const &o, SByte * const &buf);

// 将 o 转为 字串 并填充到某个 Char Buffer 容器 并返回 容器指针
// 注意: o 的类型通常不可以是 SByte, Char 这些 (超出 ToString_BuferLength 字长将有缓冲区溢出风险)
// 注意2: 传入的 buf 长度至少为 32
template<typename T>
Char const * ToWString(T const &o, Char * const &buf);

// 上面两个 ToString 的 Hex 版
template<typename T>
SByte const * ToString_Hex(T const &o, SByte * const &buf);

template<typename T>
Char const * ToWString_Hex(T const &o, Char * const &buf);


// todo: 追加更多的 ToString 各种格式化


// 下面这段代码用于返回某 简单数据类型 转为字串后的最大长度

template<typename T>
UInt32 ToString_GetMaxLength();

template<>
UInt32 ToString_GetMaxLength<Single>();

template<>
UInt32 ToString_GetMaxLength<Double>();

// Char 返回 len * 3, char 返回 len
template<typename T, UInt32 len>
UInt32 ToString_GetMaxLength();

// 类似 Console Write, Binary Write 的字串格式化写入 (SByte 版, 内容将被格式化为 utf8 编码)

// 往 buf 写入 1 个字符
void WriteChar(SByte * const &buf, UInt32 &offset, UInt32 const &c);

template<typename T>
void Write(SByte * const &buf, UInt32 &offset, T const &v);
template<typename T>
void WriteHex(SByte * const &buf, UInt32 &offset, T const &v);



// 往 buf 写入 以 16 进制 Byte[] 内容
void WriteHex(SByte * const &buf, UInt32 &offset, Byte const * const &s, UInt32 const &len);

// 往 buf 写入 Utf8 编码格式的字串
void Write(SByte * const &buf, UInt32 &offset, SByte const * const &s, UInt32 const &len);
void Write(SByte * const &buf, UInt32 &offset, SByte const * const &s);

// 往 buf 写入 Unicode 编码格式的字串
void Write(SByte * const &buf, UInt32 &offset, Char const * const &s, UInt32 const &len);
void Write(SByte * const &buf, UInt32 &offset, Char const * const &s);

template<typename T>
void WriteNull(T * const &buf, UInt32 &offset);



// 按 10 进制 往 buf 写入 可空 值类型
template <typename T>
void Write(SByte * const &buf, UInt32 &offset, Nullable<T> const &nv);

// 按 16 进制 往 buf 写入 可空 值类型 (2, 4, 8, 16 定长)
template <typename T>
void WriteHex(SByte * const &buf, UInt32 &offset, Nullable<T> const &nv);


// 类似 Console Write, Binary Write 的字串格式化写入 (Char 版, 内容将被格式化为 Unicode 编码, 当前只支持到 UCS2)

// 往 buf 写入 1 个字符
void WriteChar(Char * const &buf, UInt32 &offset, UInt32 const &c);

// 用来替代上面的代码的泛型
template<typename T>
void Write(Char * const &buf, UInt32 &offset, T const &v);
template<typename T>
void WriteHex(Char * const &buf, UInt32 &offset, T const &v);


// 往 buf 写入 以 16 进制 Byte[] 内容
void WriteHex(Char * const &buf, UInt32 &offset, Byte const * const &s, UInt32 const &len);

// 往 buf 写入 Utf8 编码格式的字串
void Write(Char * const &buf, UInt32 &offset, SByte const * const &s, UInt32  const &len);
void Write(Char * const &buf, UInt32 &offset, SByte const * const &s);

// 往 buf 写入 Unicode 编码格式的字串
void Write(Char * const &buf, UInt32 &offset, Char const * const &s, UInt32  const &len);
void Write(Char * const &buf, UInt32 &offset, Char const * const &s);


// 按 10 进制 往 buf 写入 可空 值类型
template <typename T>
void Write(Char * const &buf, UInt32 &offset, Nullable<T> const &nv);

// 按 16 进制 往 buf 写入 可空 值类型 (2, 4, 8, 16 定长)
template <typename T>
void WriteHex(Char * const &buf, UInt32 &offset, Nullable<T> const &nv);



template<UInt32 len, class T>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len]);
template<UInt32 len, class T, typename T0>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0);
template<UInt32 len, class T, typename T0, typename T1>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1);
template<UInt32 len, class T, typename T0, typename T1, typename T2>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);


template<UInt32 len, class T>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len]);
template<UInt32 len, class T,  typename T0>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0);
template<UInt32 len, class T,  typename T0, typename T1>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0,  T1 const &p1);
template<UInt32 len, class T,  typename T0, typename T1, typename T2>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);





// 统一 strlen , wcslen 以方便在模板中使用和解决兼容性问题
// windows 下的 strlen , wcslen　将使用自写算法替代, 可获得 4~8 倍性能提升
UInt32 GetLength(SByte const * const &s);
UInt32 GetLength(Char const * const &s);
// 在 len 范围内扫 0, 扫到返回实际长度, 未扫到则返回 len
template<UInt32 len>
UInt32 GetLength(SByte const (&s)[len]);
// 在 len 范围内扫 0, 扫到返回实际长度, 未扫到则返回 len
template<UInt32 len>
UInt32 GetLength(Char const (&s)[len]);


// Trim 系列: 字串首尾 去 "排版符". 
// 可自己指定 排版符表 s. s 为空 则表示 只去默认的 space 字符. 
// 例: s 值为 " \t\r\n", 表示去 space, tab, return, newline  这些字符

// 去首排版符 (计数扫到非排版符出现为止 然后 memmove. 填充剩余长度到 bufLen, 返回 去掉了多少个字符)
template<typename T>
UInt32 TrimStartCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 TrimStart(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 TrimStart(T * const &buf, UInt32 &bufLen, T const * const &s);

// 去尾排版符 (倒着计数扫, 扫到非排版符出现时 写入 \0. 填充剩余长度到 bufLen,  返回 去掉了多少字符)
template<typename T>
UInt32 TrimEndCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 TrimEnd(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 TrimEnd(T * const &buf, UInt32 &bufLen, T const * const &s);


// 去首尾排版符 (依次去 尾 首, 往 bufLen 填充实际长度, 填 \0, 返回 去首排版符个数)
template<typename T>
UInt32 TrimCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 Trim(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 Trim(T * const &buf, UInt32 &bufLen, T const * const &s);


// 方便使用的重载 (因为只去空格, 故复制小改, 重写判断是否为排版符部分的代码)
template<typename T>
UInt32 TrimStartCore(T * const &buf, UInt32 &bufLen);
template<typename T>
UInt32 TrimStart(T * const &buf, UInt32 &bufLen);
template<typename T>
UInt32 TrimEndCore(T * const &buf, UInt32 &bufLen);
template<typename T>
UInt32 TrimEnd(T * const &buf, UInt32 &bufLen);
template<typename T>
UInt32 TrimCore(T * const &buf, UInt32 &bufLen);
template<typename T>
UInt32 Trim(T * const &buf, UInt32 &bufLen);


// 过滤字串所有位置中含有的指定字符集 填充实际长度到 bufLen 最后填 \0 返回过滤掉多少个字符
// 算法简述: 感觉上就是实现了一个 能跳过 过滤字符集 的 memmove
template<typename T>
UInt32 TrimAllCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 TrimAll(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
UInt32 TrimAll(T * const &buf, UInt32 &bufLen, T const * const &s);


// Equals 系列 判断两个字串是否相同 并返回

// 算法简单描述: 
// foreach s1, s2 仔细比较
// 优化措施: 1. 如果长度不等, 可直接视为不相同
// 2. SIMD 思路，如果 s1 s2 所在地址均为 4 或 8 的倍数, 就批量读取判断
// 3. 继续, 如果 s1 s2 所在地址按 4/8 取出的余数相同, 则先判断余数个之后, 转到 SIMD 思路
Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len);
Boolean Equals(SByte const * const &s1, SByte const * const &s2, UInt32 const &s2Len);
Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2);
Boolean Equals(SByte const * const &s1, SByte const * const &s2);
// 这个需要判断 sizeof(Char) 再选择是否优化:
// wchar size = 2 (x86, x64) 每次可读 2 ~ 4 个字符
// wchar size = 4 (x64) 每次可读 2 个字符
Boolean Equals(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len);
Boolean Equals(Char const * const &s1, Char const * const &s2, UInt32 const &s2Len);
Boolean Equals(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2);
Boolean Equals(Char const * const &s1, Char const * const &s2);
// 这两个不可以通过长度不一致来判断是否相等, 需要逐个转换比较
Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len);
Boolean Equals(SByte const * const &s1, Char const * const &s2, UInt32 const &s2Len);
Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, Char const * const &s2);
Boolean Equals(SByte const * const &s1, Char const * const &s2);
// 反转参数调上面的函数
Boolean Equals(Char const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len);
Boolean Equals(Char const * const &s1, SByte const * const &s2, UInt32 const &s2Len);
Boolean Equals(Char const * const &s1, UInt32 const &s1Len, SByte const * const &s2);
Boolean Equals(Char const * const &s1, SByte const * const &s2);
// 无前置检查的内核版
Boolean EqualsCore(SByte const * const &s1, SByte const * const &s2, UInt32 const &s1Len);
Boolean EqualsCore(Char const * const &s1, Char const * const &s2, UInt32 const &s1Len);
Boolean EqualsCore(SByte const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len);



// 用于在未知长度的字串 buf 中定位到 c 并返回索引. 未找到则返回 -1
template<typename T>
Int32 Find(T const * const &buf, T const &c);
// 用于在字串 buf 中定位到 c 并返回索引. 未找到则返回 -1
template<typename T>
Int32 FindCore(T const * const &buf, UInt32 const &bufLen, T const &c);
template<typename T>
Int32 Find(T const * const &buf, UInt32 const &bufLen, T const &c);
// 用于在字串 buf 中定位到 s 并返回索引. 未找到则返回 -1  (未对未知长度 buf 做优化)
template<typename T>
Int32 FindCore(T const * const &buf, UInt32 const &bufLen, T const * const &s, UInt32 const &sLen);
template<typename T>
Int32 Find(T const * const &buf, UInt32 const &bufLen, T const * const &s, UInt32 const &sLen);

template<typename T>
Int32 Find(T const * const &buf, UInt32 const &bufLen, T const * const &s);
template<typename T>
Int32 Find(T const * const &buf, T const * const &s, UInt32 const &sLen);
template<typename T>
Int32 Find(T const * const &buf, T const * const &s);


// 变大小写

template<typename T>
void ToLowerCore(T * const &buf, UInt32 const &bufLen);
template<typename T>
void ToLower(T * const &buf, UInt32 const &bufLen);
template<typename T>
void ToLower(T * const &buf);

template<typename T>
void ToUpperCore(T * const &buf, UInt32 const &bufLen);
template<typename T>
void ToUpper(T * const &buf, UInt32 const &bufLen);
template<typename T>
void ToUpper(T * const &buf);


// 填充指定长度字符 (不检查 buf 长度)

void Fill(SByte * const &buf, UInt32 const &len, SByte const &c);
void Fill(Char * const &buf, UInt32 const &len, Char const &c);



// todo: XxxxxCore 系列均为无前置检查版

// todo: 把 SString 的一些实例函数弄进来实现一份


// todo:
// 全角转半角系列
// Compare 系列
// Is 系列 (类型合法性判断)
// 正则系列


}}
#endif