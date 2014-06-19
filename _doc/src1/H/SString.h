#ifndef SSTRING_H
#define SSTRING_H
namespace Lib3 {


// 一些前置声明
template<typename T>
struct List;
struct String;

// 变长 SByte 数组 容器 (带 String_OriginalBufferLength 初始 Buff)
// 本容器以 需求len * 2 方式扩容. 根据研究成果, new 无析构的非8对齐 type 时, 将直接得到 8 倍内存对齐的地址. 故此部分不需要再优化
struct SString {
    enum Settings {
        String_OriginalBufferLength     = 32
    };
    static SString const Empty;                                         // 内容为 ""
private:
    SByte           _originalBuffer[String_OriginalBufferLength];       // 初始 Buff
    SByte *         _buffer;                                            // 容器指针 指向实际存储数据的内存起始
    UInt32          _length;                                            // 数据有效长度
    UInt32          _bufferLength;                                      // 容器尺寸
    BufferStates    _bufferState;                                       // Buffer 状态标识

    // 初始化的公用代码
protected:
    void            Init();                                             // 默认构造
    void            Init(UInt32 const &len);                            // 根据长度来初始化看是 New 还是 Original
    void            Init(SByte const * const &s, UInt32 const &len);    // 按传入的数据来初始化, 复制字串内容, BufferState_New

public:
    SString();                                                          // 默认情况下, Buffer 指向 OriginalBuffer, BufferLength 为 OriginalBuffer 大小, Length = 0, BufferState = BufferState_Original
    explicit SString(UInt32 const &bufLen);                             // 初始化容器长. 如果 bufLen 超过 OriginalBuffer 的长度, Buffer 将存储 new SByte[bufLen] 指针, BufferState = BufferState_New, Length = 0, 否则按默认情况初始化. 
    explicit SString(Int32 const &bufLen);
    SString(SByte const * const &s, UInt32 const &len);                 // 按传入的数据来初始化, 复制字串内容, BufferState_New
    SString(SByte const * const &s);
    template<UInt32 len>
    SString(SByte const (&s)[len]);
    SString(UInt32 const &inBufLen, SByte * const &inBuf);              // 存引用, BufferState_Custom
    SString(SString const &s);                                          // 深度复制
    SString(SString &&s);                                               // 复制或继承 Buffer
    template<typename T>
    SString(T const &v);                                                // 通过简单类型来初始化
    ~SString();                                                         // 当 BufferState == BufferState_New 时 释放 Buffer
    SString &       EnsureSpace(UInt32 const &len);                     // 检查 Buffer 剩余空间大小，根据情况 Resize. if len > BufferLength - Length, 则扩容为 (Length + len) * 2
    SString &       EnsureLength();                                     // 检查 BufferLength, if Length > BufferLength, 则扩容为 Length  (常用于复制数据进来之前先改 Length 再调这个函数确保空间足够)
    SString &       AppendZero();                                       // 在数据区后面填充1个0但不计入长度 (实现拿 Buffer() 当 char* 0结尾字串用的效果)
    SString &       Clear();                                            // 清除 New 出来的 Buffer 或 Custom 引用, 将 SString 置为初创建状态 (Length = 0, BufferLength & Buffer & State = Original)
    SByte * &       Buffer();                                           // 引用到 field
    UInt32 &        BufferLength();                                     // 引用到 field
    BufferStates &  BufferState();                                      // 引用到 field
    UInt32 &        Length();                                           // 引用到 field
    SByte * const & Buffer() const;                                     // 引用到 field
    UInt32 const &  BufferLength() const;                               // 引用到 field
    BufferStates const &   BufferState() const;                         // 引用到 field
    UInt32 const &  Length() const;                                     // 引用到 field
protected:
    SString         TrimCore(SByte const * const &s, UInt32 const &len) const;          // 返回一个新的前后去掉空格/排版符后的 SString 对象
    SString         TrimStartCore(SByte const * const &s, UInt32 const &len) const;     // 返回一个新的去掉前空格/排版符后的 SString 对象
    SString         TrimEndCore(SByte const * const &s, UInt32 const &len) const;       // 返回一个新的去掉后空格/排版符后的 SString 对象
    SString         TrimAllCore(SByte const * const &s, UInt32 const &len) const;       // 返回一个新的去掉所有位置空格/排版符后的 SString 对象
    Boolean         ContainsCore(SByte const * const &s, UInt32 const &len) const;      // 判断是否含有某串
    Int32           IndexOfCore(SByte const * const &s, UInt32 const &len) const;       // 返回查找到的索引. 未找到返回 -1
    Int32           IndexOfCore(UInt32 const &offset, SByte const * const &s, UInt32 const &len) const;    // 从偏移处查找　找到返回索引. 未找到返回 -1
    Boolean         StartsWithCore(SByte const * const &s, UInt32 const &len) const;    // 判断字串开始部分是不是等于某串/值
    Boolean         EndsWithCore(SByte const * const &s, UInt32 const &len) const;      // 判断字串结尾部分是不是等于某串/值
    SString &       InsertCore(UInt32 const &offset, SByte const * const &s, UInt32 const &len);   // 在指定偏移处插入 xxx. 如果 offset 大于长度, 当前长度到 offset 这之间将补空格
    SString &       AppendCore(SByte const * const &s, UInt32 const &len);              // 在当前内容最后面追加 xxx.
    SString &       AppendCore(UInt32 const &fixLen, SByte const * const &s, UInt32 const &len);   // 在当前内容最后面追加 xxx. 如果 fixLen 小于追加物长度, 将在后面补 空格
    SString         ReplaceCore(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len) const;  // 返回 查找&替换 所有 xxx1 为 xxx2 后的新串
    List<SString>   SplitCore(SByte const * const &s, UInt32 const &sLen);       // 按切割符来将字串分成几个小段以数组返回
    SString &       ReplaceSelfCore(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len);  // 返回 查找&替换 所有 xxx1 为 xxx2 后的新串
public:
	SString         Trim() const;
    SString         Trim(SString const &s) const;
    SString         Trim(SByte const * const &s, UInt32 const &len) const;
    SString         Trim(SByte const * const &s) const;

	UInt32          TrimSelf();                                         // 当前文本前后去掉空格/排版符 返回共去掉多少个
    UInt32          TrimSelf(SString const &s);
    UInt32          TrimSelf(SByte const * const &s, UInt32 const &len);
    UInt32          TrimSelf(SByte const * const &s);

  	SString         TrimStart() const;
    SString         TrimStart(SString const &s) const;
    SString         TrimStart(SByte const * const &s, UInt32 const &len) const;
    SString         TrimStart(SByte const * const &s) const;

  	UInt32          TrimStartSelf();                                    // 当前文本前面去空格/排版符 返回共去掉多少个
    UInt32          TrimStartSelf(SString const &s);
    UInt32          TrimStartSelf(SByte const * const &s, UInt32 const &len);
    UInt32          TrimStartSelf(SByte const * const &s);

	SString         TrimEnd() const;
    SString         TrimEnd(SString const &s) const;
    SString         TrimEnd(SByte const * const &s, UInt32 const &len) const;
    SString         TrimEnd(SByte const * const &s) const;

	UInt32          TrimEndSelf();                                      // 当前文本后面去空格/排版符 返回共去掉多少个
    UInt32          TrimEndSelf(SString const &s);
    UInt32          TrimEndSelf(SByte const * const &s, UInt32 const &len);
    UInt32          TrimEndSelf(SByte const * const &s);

	SString         TrimAll() const;
    SString         TrimAll(SString const &s) const;
    SString         TrimAll(SByte const * const &s, UInt32 const &len) const;
    SString         TrimAll(SByte const * const &s) const;

	UInt32          TrimAllSelf();                                      // 当前文本所有位置去空格/排版符 返回共去掉多少个
    UInt32          TrimAllSelf(SString const &s);
    UInt32          TrimAllSelf(SByte const * const &s, UInt32 const &len);
    UInt32          TrimAllSelf(SByte const * const &s);

    Boolean         Contains(SByte const &c) const;
    Boolean         Contains(SString const &s) const;
    Boolean         Contains(SByte const * const &s, UInt32 const &len) const;
    Boolean         Contains(SByte const * const &s) const;
    template<typename T>
    Boolean         Contains(T const &v) const;
    
    Int32           IndexOf(SByte const &c) const;
    Int32           IndexOf(SString const &s) const;
    Int32           IndexOf(SByte const * const &s, UInt32 const &len) const;
    Int32           IndexOf(SByte const * const &s) const;
    template<typename T>
    Int32           IndexOf(T const &v) const;

    Int32           IndexOf(UInt32 const &offset, SByte const &c) const;
    Int32           IndexOf(UInt32 const &offset, SString const &s) const;
    Int32           IndexOf(UInt32 const &offset, SByte const * const &s, UInt32 const &len) const;
    Int32           IndexOf(UInt32 const &offset, SByte const * const &s) const;
    template<typename T>
    Int32           IndexOf(UInt32 const &offset, T const &v) const;

    Boolean         StartsWith(SString const &s) const;
    Boolean         StartsWith(SByte const &c) const;
    Boolean         StartsWith(SByte const * const &s, UInt32 const &len) const;
    Boolean         StartsWith(SByte const * const &s) const;
    template<typename T>
    Boolean         StartsWith(T const &v) const;
    
    Boolean         EndsWith(SString const &s) const;
    Boolean         EndsWith(SByte const &c) const;
    Boolean         EndsWith(SByte const * const &s, UInt32 const &len) const;
    Boolean         EndsWith(SByte const * const &s) const;
    template<typename T>
    Boolean         EndsWith(T const &v) const;
    
    SString         Substring(UInt32 const &offset, UInt32 const &len) const;         // 切割一段串并返回
    SString         Substring(UInt32 const &offset) const;

    SString &       SubstringSelf(UInt32 const &offset, UInt32 const &len);           // 内容裁剪
    SString &       SubstringSelf(UInt32 const &offset);
    
    SString &       Insert(UInt32 const &offset, SByte const &c);
    SString &       Insert(UInt32 const &offset, SString const &s);
    SString &       Insert(UInt32 const &offset, SString &&s);
    SString &       Insert(UInt32 const &offset, SByte const * const &s, UInt32 const &len);
    SString &       Insert(UInt32 const &offset, SByte const * const &s);
    template<typename T>
    SString &       Insert(UInt32 const &offset, T const &v);
    
    SString &       Append(SByte const &c);
    SString &       Append(SByte const &c, UInt32 const &num);
    SString &       Append(SString const &s);
    SString &       Append(SString &&s);
    SString &       Append(SByte const * const &s, UInt32 const &len);
    SString &       Append(SByte const * const &s);
    template<typename T>
    SString &       Append(T const &v);

    SString &       Append(UInt32 const &fixLen, SByte const &c);
    SString &       Append(UInt32 const &fixLen, SByte const &c, UInt32 const &num);
    SString &       Append(UInt32 const &fixLen, SString const &s);
    SString &       Append(UInt32 const &fixLen, SByte const * const &s, UInt32 const &len);
    SString &       Append(UInt32 const &fixLen, SByte const * const &s);
    template<typename T>
    SString &       Append(UInt32 const &fixLen, T const &v);

    SString         Remove(UInt32 const &offset, UInt32 const &len) const;            // 返回移除掉一段字符的新串
    SString         Remove(UInt32 const &offset) const;

    SString &       RemoveSelf(UInt32 const &offset, UInt32 const &len);              // 移除掉一段字符
    SString &       RemoveSelf(UInt32 const &offset);

    SString         Replace(SByte const &c1, SByte const &c2) const;
    SString         Replace(SString const &s1, SString const &s2) const;
    SString         Replace(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len) const;
    SString         Replace(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2) const;
    SString         Replace(SByte const * const &s1, SByte const * const &s2, UInt32 const &s2Len) const;
    SString         Replace(SByte const * const &s1, SByte const * const &s2) const;
    SString         Replace(SString const &s1, SByte const &c2) const;
    SString         Replace(SByte const &c1, SString const &s2) const;
    SString         Replace(SByte const &c1, SByte const * const &s2, UInt32 const &s2Len) const;
    SString         Replace(SByte const &c1, SByte const * const &s2) const;
    SString         Replace(SByte const * const &s1, UInt32 const &s1Len, SByte const &c2) const;
    SString         Replace(SByte const * const &s1, SByte const &c2) const;
    SString         Replace(SByte const * const &s1, UInt32 const &s1Len, SString const &s2) const;
    SString         Replace(SByte const * const &s1, SString const &s2) const;
    SString         Replace(SString const &s1, SByte const * const &s2, UInt32 const &s2Len) const;
    SString         Replace(SString const &s1, SByte const * const &s2) const;
    template<typename T>
    SString         Replace(T const &v1, T const &v2) const;
    template<typename T>
    SString         Replace(SByte const &c1, T const &v2) const;
    template<typename T>
    SString         Replace(T const &v1, SByte const &c2) const;
    template<typename T>
    SString         Replace(SString const &s1, T const &v2) const;
    template<typename T>
    SString         Replace(T const &v1, SString const &s2) const;
    template<typename T>
    SString         Replace(SByte const * const &s1, UInt32 const &s1Len, T const &v2) const;
    template<typename T>
    SString         Replace(SByte const * const &s1, T const &v2) const;
    template<typename T>
    SString         Replace(T const &v1, SByte const * const &s2, UInt32 const &s2Len) const;
    template<typename T>
    SString         Replace(T const &v1, SByte const * const &s2) const;

    SString &       ReplaceSelf(SByte const &c1, SByte const &c2);
    SString &       ReplaceSelf(SString const &s1, SString const &s2);
    SString &       ReplaceSelf(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len);
    SString &       ReplaceSelf(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2);
    SString &       ReplaceSelf(SByte const * const &s1, SByte const * const &s2, UInt32 const &s2Len);
    SString &       ReplaceSelf(SByte const * const &s1, SByte const * const &s2);
    SString &       ReplaceSelf(SString const &s1, SByte const &c2);
    SString &       ReplaceSelf(SByte const &c1, SString const &s2);
    SString &       ReplaceSelf(SByte const &c1, SByte const * const &s2, UInt32 const &s2Len);
    SString &       ReplaceSelf(SByte const &c1, SByte const * const &s2);
    SString &       ReplaceSelf(SByte const * const &s1, UInt32 const &s1Len, SByte const &c2);
    SString &       ReplaceSelf(SByte const * const &s1, SByte const &c2);
    SString &       ReplaceSelf(SByte const * const &s1, UInt32 const &s1Len, SString const &s2);
    SString &       ReplaceSelf(SByte const * const &s1, SString const &s2);
    SString &       ReplaceSelf(SString const &s1, SByte const * const &s2, UInt32 const &s2Len);
    SString &       ReplaceSelf(SString const &s1, SByte const * const &s2);
    template<typename T>
    SString &       ReplaceSelf(T const &v1, T const &v2);
    template<typename T>
    SString &       ReplaceSelf(SByte const &c1, T const &v2);
    template<typename T>
    SString &       ReplaceSelf(T const &v1, SByte const &c2);
    template<typename T>
    SString &       ReplaceSelf(SString const &s1, T const &v2);
    template<typename T>
    SString &       ReplaceSelf(T const &v1, SString const &s2);
    template<typename T>
    SString &       ReplaceSelf(SByte const * const &s1, UInt32 const &s1Len, T const &v2);
    template<typename T>
    SString &       ReplaceSelf(SByte const * const &s1, T const &v2);
    template<typename T>
    SString &       ReplaceSelf(T const &v1, SByte const * const &s2, UInt32 const &s2Len);
    template<typename T>
    SString &       ReplaceSelf(T const &v1, SByte const * const &s2);

    List<SString>   Split(SByte const &c);                               // 切割字串成一堆小数组返回
    List<SString>   Split(SByte const * const &s, UInt32 const &sLen);
    List<SString>   Split(SByte const * const &s);
    List<SString>   Split(SString const &s);
    
    SString         ToLower() const;                                    // A~Z 转为小写 a~z 并返回新串
    SString         ToUpper() const;                                    // a~z 转为大写 A~Z 并返回新串

    SString &       ToLowerSelf();                                      // 文本中的大写 A~Z 转为小写 a~z. 
    SString &       ToUpperSelf();                                      // 文本中的小写 a~z 转为大写 A~Z. 

    /*
     * todo:
     * LastIndexOf      这个需要倒着扫
     * Like             这个再说
     **/
    

    // 只复制数据, 尽量利用已经创建的空间 (如果是 BufferState_Custom 则根据 s 长度情况来复制或 new)
    SString & operator=(SByte const * const &s);
    SString & operator=(SString const &s);
    SString & operator=(SString &&s);
    // 实现追加效果  (如果是 BufferState_Custom 则根据 s 长度情况来复制或 new, 再追加)
    void operator+=(SString const &s);
    void operator+=(SByte const * const &s);
    // 引用某索引所在字符
    SByte & operator[](UInt32 const &idx);
    SByte const & operator[](UInt32 const &idx) const;

    // 下列函数有用到 AppendCore 非公开函数
    friend SString && operator+(SString &&s1, SByte const * const &s2);
    friend SString && operator+(SString &&s1, SString &&s2);


    // todo: 考虑将 AppendFormat 设计为一次性 EnsureSpace

    // 往 SString 写入 格式化串, T 可以是 SByte 或 Char, 类似 .NET 的 SString.Format 用法. 已支持的 format 参数列表：
    // %%   %的转义
    // 0~9  代指后面的第 0 ~ 9 个参数
    template<UInt32 len>
    SString & AppendFormat(SByte const (&s)[len]);
    template<UInt32 len, typename T0>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0);
    template<UInt32 len, typename T0, typename T1>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1);
    template<UInt32 len, typename T0, typename T1, typename T2>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    SString & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);

    template<UInt32 len>
    SString & AppendFormat(Char const (&s)[len]);
    template<UInt32 len , typename T0>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0);
    template<UInt32 len , typename T0, typename T1>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1);
    template<UInt32 len , typename T0, typename T1, typename T2>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    SString & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);

    template<typename T>
    SString & AppendFormat(T const &t);
    template<typename T0>
    SString & AppendFormat(SString const &str, T0 const &p0);
    template<typename T0, typename T1>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1);
    template<typename T0, typename T1, typename T2>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2);
    template<typename T0, typename T1, typename T2, typename T3>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    SString & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);

    template<typename T0>
    SString & AppendFormat(String const &str, T0 const &p0);
    template<typename T0, typename T1>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1);
    template<typename T0, typename T1, typename T2>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2);
    template<typename T0, typename T1, typename T2, typename T3>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    SString & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);









    // 下面是和 String/Char 的互日区
    //
    SString(Char const * const &s, UInt32 const &len);
    SString(Char const * const &s);
    template<UInt32 len>
    SString(Char const (&s)[len]);
    SString(String const &s);
    SString &            Append(Char const &c);
    SString &            Append(String const &s);
    SString &            Append(Char const * const &s, UInt32 const &len);
    SString &            Append(Char const * const &s);
    SString &            Insert(UInt32 const &offset, Char const &c);
    SString &            Insert(UInt32 const &offset, String const &s);
    SString &            Insert(UInt32 const &offset, Char const * const &s, UInt32 const &len);
    SString &            Insert(UInt32 const &offset, Char const * const &s);
    SString &            operator=(Char const * const &s);
    SString &            operator=(String const &s);
    void                operator+=(String const &s);
    void                operator+=(Char const * const &s);
    friend SString &&    operator+(SString &&s1, Char const * const &s2);
    //
    // 上面是和 String/Char 的互日区
    
    // todo: 理论上讲　查找类的函数　都需要实现互日 (不支持在 SString 里查找单个 Char. 但 String 里查找单个 SByte 应该支持)
};

// 下面是和 String/Char 的互日区
//
SString & operator<<(SString &o, String const &in);
SString & operator<<(SString &o, Char const *in);
SString && operator<<(SString &&o, String const &in);
SString && operator<<(SString &&o, Char const *in);
SString operator+(SString const &s1, String const &s2);
SString operator+(SString const &s1, Char const * const &s2);
SString operator+(Char const * const &s1, SString const &s2);
SString && operator+(Char const * const &s1, SString &&s2);
SString && operator+(SString &&s1, String const &s2);
SString && operator+(SString &&s1, Char const * const &s2);
Boolean operator==(SString const &s1, String const &s2);
Boolean operator==(SString const &s1, Char const * const &s2);
Boolean operator==(Char const * const &s1, SString const &s2);
Boolean operator!=(SString const &s1, String const &s2);
Boolean operator!=(SString const &s1, Char const * const &s2);
Boolean operator!=(Char const * const &s1, SString const &s2);
//
// 上面是和 String/Char 的互日区


// 利用 << 重载 实现追加写效果
template<typename T>
SString & operator<<(SString &o, T const &in);
// 复杂类型避免复制
SString & operator<<(SString &o, SString const &in);
SString & operator<<(SString &o, SString &&in);
SString & operator<<(SString &o, SByte const * const &in);

// 左参右值版
template<typename T>
SString && operator<<(SString &&o, T const &in);
SString && operator<<(SString &&o, SString const &in);
SString && operator<<(SString &&o, SString &&in);
SString && operator<<(SString &&o, SByte const * const &in);

// 字串连接 (右值版为追加) 符号重载 (带 + 的操作会产生临时对象)
// SString, T 分别出现在 + 号左右侧, SString 必须出现至少 1 次
// 
// 注: 其他复杂类型支持, 需要重载该符号. 
// 
template<typename T>
SString operator+(SString const &s, T const &v);
template<typename T>
SString operator+(T const &v, SString const &s);
// 复杂对象连接时传引用 避免复制
SString operator+(SString const &s1, SString const &s2);
SString operator+(SString const &s1, SByte const * const &s2);
SString operator+(SByte const * const &s1, SString const &s2);
// 右值版
template<typename T>
SString && operator+(SString &&s, T const &v);
SString && operator+(SString &&s1, SString const &s2);
SString && operator+(SString &&s1, SString &&s2);
SString && operator+(SString &&s1, SByte const * const &s2);
SString && operator+(SString const &s1, SString &&s2);
SString && operator+(SByte const * const &s1, SString &&s2);

template<typename T>
Boolean operator==(SString const &s, T const &v);
template<typename T>
Boolean operator==(T const &v, SString const &s);
// 复杂对象连接时传引用 避免复制
Boolean operator==(SString const &s1, SString const &s2);
Boolean operator==(SString const &s1, SByte const * const &s2);
Boolean operator==(SByte const * const &s1, SString const &s2);

template<typename T>
Boolean operator!=(SString const &s, T const &v);
template<typename T>
Boolean operator!=(T const &v, SString const &s);
// 复杂对象连接时传引用 避免复制
Boolean operator!=(SString const &s1, SString const &s2);
Boolean operator!=(SString const &s1, SByte const * const &s2);
Boolean operator!=(SByte const * const &s1, SString const &s2);







}
#endif