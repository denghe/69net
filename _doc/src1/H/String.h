#ifndef STRING_H
#define STRING_H
namespace Lib3 {


// 变长 Char 数组 容器 (带 WString_OriginalBufferLength 初始 Buff)
// 本容器以 需求len * 2 方式扩容. 根据研究成果, new 无析构的非8对齐 type 时, 将直接得到 8 倍内存对齐的地址. 故此部分不需要再优化
struct String {
    enum Settings {
        WString_OriginalBufferLength     = 64
    };
    static String const Empty;                                         // 内容为 L""
private:
    Char            _originalBuffer[WString_OriginalBufferLength];      // 初始 Buff
    Char *          _buffer;                                            // 容器指针 指向实际存储数据的内存起始
    UInt32          _length;                                            // 数据有效长度
    UInt32          _bufferLength;                                      // 容器尺寸
    BufferStates    _bufferState;                                       // Buffer 状态标识

    // 初始化的公用代码
protected:
    void            Init();                                             // 默认构造
    void            Init(UInt32 const &len);                            // 根据长度来初始化看是 New 还是 Original
    void            Init(Char const * const &s, UInt32 const &len);    // 按传入的数据来初始化, 复制字串内容, BufferState_New
public:
    String();                                                          // 默认情况下, Buffer 指向 OriginalBuffer, BufferLength 为 OriginalBuffer 大小, Length = 0, BufferState = BufferState_Original
    explicit String(UInt32 const &bufLen);                             // 初始化容器长. 如果 bufLen 超过 OriginalBuffer 的长度, Buffer 将存储 new Char[bufLen] 指针, BufferState = BufferState_New, Length = 0, 否则按默认情况初始化. 
    explicit String(Int32 const &bufLen);
    String(Char const * const &s, UInt32 const &len);                 // 按传入的数据来初始化, 复制字串内容, BufferState_New
    String(Char const * const &s);
    template<UInt32 len>
    String(Char const (&s)[len]);
    String(UInt32 const &inBufLen, Char * const &inBuf);              // 存引用, BufferState_Custom
    String(String const &s);                                          // 深度复制
    String(String &&s);                                               // 复制或继承 Buffer
    template<typename T>
    String(T const &v);                                                // 通过简单类型来初始化
    ~String();                                                         // 当 BufferState == BufferState_New 时 释放 Buffer
    String &        EnsureSpace(UInt32 const &len);                     // 检查 Buffer 剩余空间大小，根据情况 Resize. if len > BufferLength - Length, 则扩容为 (Length + len) * 2
    String &        EnsureLength();                                     // 检查 BufferLength, if Length > BufferLength, 则扩容为 Length  (常用于复制数据进来之前先改 Length 再调这个函数确保空间足够)
    String &        AppendZero();                                       // 在数据区后面填充1个0但不计入长度 (实现拿 Buffer() 当 wchar* 0结尾字串用的效果)
    String &        Clear();                                            // 清除 New 出来的 Buffer 或 Custom 引用, 将 String 置为初创建状态 (Length = 0, BufferLength & Buffer & State = Original)
    Char * &        Buffer();                                           // 引用到 field
    UInt32 &        BufferLength();                                     // 引用到 field
    BufferStates &  BufferState();                                      // 引用到 field
    UInt32 &        Length();                                           // 引用到 field
    Char * const &  Buffer() const;                                     // 引用到 field
    UInt32 const &  BufferLength() const;                               // 引用到 field
    BufferStates const &   BufferState() const;                         // 引用到 field
    UInt32 const &  Length() const;                                     // 引用到 field
protected:
    String          TrimCore(Char const * const &s, UInt32 const &len) const;         // 返回一个新的前后去掉空格/排版符后的 String 对象
    String          TrimStartCore(Char const * const &s, UInt32 const &len) const;    // 返回一个新的去掉前空格/排版符后的 String 对象
    String          TrimEndCore(Char const * const &s, UInt32 const &len) const;      // 返回一个新的去掉后空格/排版符后的 String 对象
    String          TrimAllCore(Char const * const &s, UInt32 const &len) const;      // 返回一个新的去掉所有位置空格/排版符后的 String 对象
    Boolean         ContainsCore(Char const * const &s, UInt32 const &len) const;     // 判断是否含有某串
    Int32           IndexOfCore(Char const * const &s, UInt32 const &len) const;      // 返回查找到的索引. 未找到返回 -1
    Int32           IndexOfCore(UInt32 const &offset, Char const * const &s, UInt32 const &len) const;    // 从偏移处查找　找到返回索引. 未找到返回 -1
    Boolean         StartsWithCore(Char const * const &s, UInt32 const &len) const;   // 判断字串开始部分是不是等于某串/值
    Boolean         EndsWithCore(Char const * const &s, UInt32 const &len) const;     // 判断字串结尾部分是不是等于某串/值
    String &        InsertCore(UInt32 const &offset, Char const * const &s, UInt32 const &len);   // 在指定偏移处插入 xxx. 如果 offset 大于长度, 当前长度到 offset 这之间将补空格
    String &        AppendCore(Char const * const &s, UInt32 const &len);             // 在当前内容最后面追加 xxx.
    String &        AppendCore(UInt32 const &fixLen, Char const * const &s, UInt32 const &len);   // 在当前内容最后面追加 xxx. 如果 fixLen 小于追加物长度, 将在后面补 空格
    String          ReplaceCore(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) const;  // 返回 查找&替换 所有 xxx1 为 xxx2 后的新串
    List<String>    SplitCore(Char const * const &s, UInt32 const &sLen);             // 按切割符来将字串分成几个小段以数组返回
    String &        ReplaceSelfCore(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len);  // 返回 查找&替换 所有 xxx1 为 xxx2 后的新串
public:
	String          Trim() const;
    String          Trim(String const &s) const;
    String          Trim(Char const * const &s, UInt32 const &len) const;
    String          Trim(Char const * const &s) const;

	UInt32          TrimSelf();                                         // 当前文本前后去掉空格/排版符 返回共去掉多少个
    UInt32          TrimSelf(String const &s);
    UInt32          TrimSelf(Char const * const &s, UInt32 const &len);
    UInt32          TrimSelf(Char const * const &s);

  	String          TrimStart() const;
    String          TrimStart(String const &s) const;
    String          TrimStart(Char const * const &s, UInt32 const &len) const;
    String          TrimStart(Char const * const &s) const;

  	UInt32          TrimStartSelf();                                    // 当前文本前面去空格/排版符 返回共去掉多少个
    UInt32          TrimStartSelf(String const &s);
    UInt32          TrimStartSelf(Char const * const &s, UInt32 const &len);
    UInt32          TrimStartSelf(Char const * const &s);

	String          TrimEnd() const;
    String          TrimEnd(String const &s) const;
    String          TrimEnd(Char const * const &s, UInt32 const &len) const;
    String          TrimEnd(Char const * const &s) const;

	UInt32          TrimEndSelf();                                      // 当前文本后面去空格/排版符 返回共去掉多少个
    UInt32          TrimEndSelf(String const &s);
    UInt32          TrimEndSelf(Char const * const &s, UInt32 const &len);
    UInt32          TrimEndSelf(Char const * const &s);

	String          TrimAll() const;
    String          TrimAll(String const &s) const;
    String          TrimAll(Char const * const &s, UInt32 const &len) const;
    String          TrimAll(Char const * const &s) const;

	UInt32          TrimAllSelf();                                      // 当前文本所有位置去空格/排版符 返回共去掉多少个
    UInt32          TrimAllSelf(String const &s);
    UInt32          TrimAllSelf(Char const * const &s, UInt32 const &len);
    UInt32          TrimAllSelf(Char const * const &s);

    Boolean         Contains(Char const &c) const;
    Boolean         Contains(String const &s) const;
    Boolean         Contains(Char const * const &s, UInt32 const &len) const;
    Boolean         Contains(Char const * const &s) const;
    template<typename T>
    Boolean         Contains(T const &v) const;
    
    Int32           IndexOf(Char const &c) const;
    Int32           IndexOf(String const &s) const;
    Int32           IndexOf(Char const * const &s, UInt32 const &len) const;
    Int32           IndexOf(Char const * const &s) const;
    template<typename T>
    Int32           IndexOf(T const &v) const;

    Int32           IndexOf(UInt32 const &offset, Char const &c) const;
    Int32           IndexOf(UInt32 const &offset, String const &s) const;
    Int32           IndexOf(UInt32 const &offset, Char const * const &s, UInt32 const &len) const;
    Int32           IndexOf(UInt32 const &offset, Char const * const &s) const;
    template<typename T>
    Int32           IndexOf(UInt32 const &offset, T const &v) const;

    Boolean         StartsWith(String const &s) const;
    Boolean         StartsWith(Char const &c) const;
    Boolean         StartsWith(Char const * const &s, UInt32 const &len) const;
    Boolean         StartsWith(Char const * const &s) const;
    template<typename T>
    Boolean         StartsWith(T const &v) const;
    
    Boolean         EndsWith(String const &s) const;
    Boolean         EndsWith(Char const &c) const;
    Boolean         EndsWith(Char const * const &s, UInt32 const &len) const;
    Boolean         EndsWith(Char const * const &s) const;
    template<typename T>
    Boolean         EndsWith(T const &v) const;
    
    String          Substring(UInt32 const &offset, UInt32 const &len) const;         // 切割一段串并返回
    String          Substring(UInt32 const &offset) const;

    String &        SubstringSelf(UInt32 const &offset, UInt32 const &len);           // 内容裁剪
    String &        SubstringSelf(UInt32 const &offset);
    
    String &        Insert(UInt32 const &offset, Char const &c);
    String &        Insert(UInt32 const &offset, String const &s);
    String &        Insert(UInt32 const &offset, String &&s);
    String &        Insert(UInt32 const &offset, Char const * const &s, UInt32 const &len);
    String &        Insert(UInt32 const &offset, Char const * const &s);
    template<typename T>
    String &        Insert(UInt32 const &offset, T const &v);
    
    String &        Append(Char const &c);
    String &        Append(Char const &c, UInt32 const &num);
    String &        Append(String const &s);
    String &        Append(String &&s);
    String &        Append(Char const * const &s, UInt32 const &len);
    String &        Append(Char const * const &s);
    template<typename T>
    String &        Append(T const &v);

    String &        Append(UInt32 const &fixLen, Char const &c);
    String &        Append(UInt32 const &fixLen, Char const &c, UInt32 const &num);
    String &        Append(UInt32 const &fixLen, String const &s);
    String &        Append(UInt32 const &fixLen, Char const * const &s, UInt32 const &len);
    String &        Append(UInt32 const &fixLen, Char const * const &s);
    template<typename T>
    String &        Append(UInt32 const &fixLen, T const &v);

    String          Remove(UInt32 const &offset, UInt32 const &len) const;            // 返回移除掉一段字符的新串
    String          Remove(UInt32 const &offset) const;

    String &        RemoveSelf(UInt32 const &offset, UInt32 const &len);              // 移除掉一段字符
    String &        RemoveSelf(UInt32 const &offset);

    String          Replace(Char const &c1, Char const &c2) const;
    String          Replace(String const &s1, String const &s2) const;
    String          Replace(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) const;
    String          Replace(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2) const;
    String          Replace(Char const * const &s1, Char const * const &s2, UInt32 const &s2Len) const;
    String          Replace(Char const * const &s1, Char const * const &s2) const;
    String          Replace(String const &s1, Char const &c2) const;
    String          Replace(Char const &c1, String const &s2) const;
    String          Replace(Char const &c1, Char const * const &s2, UInt32 const &s2Len) const;
    String          Replace(Char const &c1, Char const * const &s2) const;
    String          Replace(Char const * const &s1, UInt32 const &s1Len, Char const &c2) const;
    String          Replace(Char const * const &s1, Char const &c2) const;
    String          Replace(Char const * const &s1, UInt32 const &s1Len, String const &s2) const;
    String          Replace(Char const * const &s1, String const &s2) const;
    String          Replace(String const &s1, Char const * const &s2, UInt32 const &s2Len) const;
    String          Replace(String const &s1, Char const * const &s2) const;
    template<typename T>
    String          Replace(T const &v1, T const &v2) const;
    template<typename T>
    String          Replace(Char const &c1, T const &v2) const;
    template<typename T>
    String          Replace(T const &v1, Char const &c2) const;
    template<typename T>
    String          Replace(String const &s1, T const &v2) const;
    template<typename T>
    String          Replace(T const &v1, String const &s2) const;
    template<typename T>
    String          Replace(Char const * const &s1, UInt32 const &s1Len, T const &v2) const;
    template<typename T>
    String          Replace(Char const * const &s1, T const &v2) const;
    template<typename T>
    String          Replace(T const &v1, Char const * const &s2, UInt32 const &s2Len) const;
    template<typename T>
    String          Replace(T const &v1, Char const * const &s2) const;

    String &        ReplaceSelf(Char const &c1, Char const &c2);
    String &        ReplaceSelf(String const &s1, String const &s2);
    String &        ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len);
    String &        ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2);
    String &        ReplaceSelf(Char const * const &s1, Char const * const &s2, UInt32 const &s2Len);
    String &        ReplaceSelf(Char const * const &s1, Char const * const &s2);
    String &        ReplaceSelf(String const &s1, Char const &c2);
    String &        ReplaceSelf(Char const &c1, String const &s2);
    String &        ReplaceSelf(Char const &c1, Char const * const &s2, UInt32 const &s2Len);
    String &        ReplaceSelf(Char const &c1, Char const * const &s2);
    String &        ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, Char const &c2);
    String &        ReplaceSelf(Char const * const &s1, Char const &c2);
    String &        ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, String const &s2);
    String &        ReplaceSelf(Char const * const &s1, String const &s2);
    String &        ReplaceSelf(String const &s1, Char const * const &s2, UInt32 const &s2Len);
    String &        ReplaceSelf(String const &s1, Char const * const &s2);
    template<typename T>
    String &        ReplaceSelf(T const &v1, T const &v2);
    template<typename T>
    String &        ReplaceSelf(Char const &c1, T const &v2);
    template<typename T>
    String &        ReplaceSelf(T const &v1, Char const &c2);
    template<typename T>
    String &        ReplaceSelf(String const &s1, T const &v2);
    template<typename T>
    String &        ReplaceSelf(T const &v1, String const &s2);
    template<typename T>
    String &        ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, T const &v2);
    template<typename T>
    String &        ReplaceSelf(Char const * const &s1, T const &v2);
    template<typename T>
    String &        ReplaceSelf(T const &v1, Char const * const &s2, UInt32 const &s2Len);
    template<typename T>
    String &        ReplaceSelf(T const &v1, Char const * const &s2);

    List<String>    Split(Char const &c);
    List<String>    Split(Char const * const &s, UInt32 const &sLen);
    List<String>    Split(Char const * const &s);
    List<String>    Split(String const &s);
    
    String          ToLower() const;                                    // A~Z 转为小写 a~z 并返回新串
    String          ToUpper() const;                                    // a~z 转为大写 A~Z 并返回新串

    String &        ToLowerSelf();                                      // 文本中的大写 A~Z 转为小写 a~z. 
    String &        ToUpperSelf();                                      // 文本中的小写 a~z 转为大写 A~Z. 

    /*
     * todo:
     * LastIndexOf      这个要倒着扫, 再说
     * Like             这个再说
     **/
    

    // 只复制数据, 尽量利用已经创建的空间 (如果是 BufferState_Custom 则根据 s 长度情况来复制或 new)
    String & operator=(Char const * const &s);
    String & operator=(String const &s);
    String & operator=(String &&s);
    // 实现追加效果  (如果是 BufferState_Custom 则根据 s 长度情况来复制或 new, 再追加)
    void operator+=(String const &s);
    void operator+=(Char const * const &s);
    // 引用某索引所在字符
    Char & operator[](UInt32 const &idx);
    Char const & operator[](UInt32 const &idx) const;

    // 下列函数有用到 AppendCore 非公开函数
    friend String && operator+(String &&s1, Char const * const &s2);
    friend String && operator+(String &&s1, String &&s2);


    // todo: 考虑将 AppendFormat 设计为一次性 EnsureSpace

    // 往 String 写入 格式化串, T 可以是 Char 或 Char, 类似 .NET 的 String.Format 用法. 已支持的 format 参数列表：
    // %%   %的转义
    // 0~9  代指后面的第 0 ~ 9 个参数
    template<UInt32 len>
    String & AppendFormat(SByte const (&s)[len]);
    template<UInt32 len, typename T0>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0);
    template<UInt32 len, typename T0, typename T1>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1);
    template<UInt32 len, typename T0, typename T1, typename T2>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    String & AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);

    template<UInt32 len>
    String & AppendFormat(Char const (&s)[len]);
    template<UInt32 len , typename T0>
    String & AppendFormat(Char const (&s)[len], T0 const &p0);
    template<UInt32 len , typename T0, typename T1>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1);
    template<UInt32 len , typename T0, typename T1, typename T2>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    String & AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);

    template<typename T>
    String & AppendFormat(T const &t);
    template<typename T0>
    String & AppendFormat(SString const &str, T0 const &p0);
    template<typename T0, typename T1>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1);
    template<typename T0, typename T1, typename T2>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2);
    template<typename T0, typename T1, typename T2, typename T3>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    String & AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);

    template<typename T0>
    String & AppendFormat(String const &str, T0 const &p0);
    template<typename T0, typename T1>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1);
    template<typename T0, typename T1, typename T2>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2);
    template<typename T0, typename T1, typename T2, typename T3>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3);
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8);
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    String & AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9);








    // 下面是和 SString/SByte 的互日区
    //
    String(SByte const * const &s, UInt32 const &len);
    String(SByte const * const &s);
    template<UInt32 len>
    String(SByte const (&s)[len]);
    String(SString const &s);
    String &           Append(SByte const &c);
    String &           Append(SString const &s);
    String &           Append(SByte const * const &s, UInt32 const &len);
    String &           Append(SByte const * const &s);
    String &           Insert(UInt32 const &offset, SByte const &c);
    String &           Insert(UInt32 const &offset, SString const &s);
    String &           Insert(UInt32 const &offset, SByte const * const &s, UInt32 const &len);
    String &           Insert(UInt32 const &offset, SByte const * const &s);
    String &           operator=(SByte const * const &s);
    String &           operator=(SString const &s);
    void                operator+=(SString const &s);
    void                operator+=(SByte const * const &s);
    friend String &&   operator+(String &&s1, SByte const * const &s2);
    //
    // 上面是和 SString/SByte 的互日区
    
    // todo: 理论上讲　查找类的函数　都需要实现互日 (不支持在 SString 里查找单个 Char. 但 String 里查找单个 SByte 应该支持)
};

// 下面是和 SString/SByte 的互日区
//
String & operator<<(String &o, SString const &in);
String & operator<<(String &o, SByte const * const &in);
String && operator<<(String &&o, SString const &in);
String && operator<<(String &&o, SByte const * const &in);
String operator+(String const &s1, SString const &s2);
String operator+(String const &s1, SByte const * const &s2);
String operator+(SByte const * const &s1, String const &s2);
String && operator+(SByte const * const &s1, String &&s2);
String && operator+(String &&s1, SString const &s2);
String && operator+(String &&s1, SByte const * const &s2);
Boolean operator==(String const &s1, SString const &s2);
Boolean operator==(String const &s1, SByte const * const &s2);
Boolean operator==(SByte const * const &s1, String const &s2);
Boolean operator!=(String const &s1, SString const &s2);
Boolean operator!=(String const &s1, SByte const * const &s2);
Boolean operator!=(SByte const * const &s1, String const &s2);
//
// 上面是和 SString/SByte 的互日区


// 利用 << 重载 实现追加写效果
template<typename T>
String & operator<<(String &o, T const &in);
// 复杂类型避免复制
String & operator<<(String &o, String const &in);
String & operator<<(String &o, String &&in);
String & operator<<(String &o, Char const *in);
// 左参右值版
template<typename T>
String && operator<<(String &&o, T const &in);
String && operator<<(String &&o, String const &in);
String && operator<<(String &&o, String &&in);
String && operator<<(String &&o, Char const *in);

// 字串连接 (右值版为追加) 符号重载 (带 + 的操作会产生临时对象)
// String, T 分别出现在 + 号左右侧, String 必须出现至少 1 次
// 
// 注: 其他复杂类型支持, 需要重载该符号. 
// 
template<typename T>
String operator+(String const &s, T const &v);
template<typename T>
String operator+(T const &v, String const &s);
// 复杂对象连接时传引用 避免复制
String operator+(String const &s1, String const &s2);
String operator+(String const &s1, Char const * const &s2);
String operator+(Char const * const &s1, String const &s2);
// 右值版
template<typename T>
String && operator+(String &&s, T const &v);
String && operator+(String &&s1, String const &s2);
String && operator+(String &&s1, String &&s2);
String && operator+(String &&s1, Char const * const &s2);
String && operator+(String const &s1, String &&s2);
String && operator+(Char const * const &s1, String &&s2);

template<typename T>
Boolean operator==(String const &s, T const &v);
template<typename T>
Boolean operator==(T const &v, String const &s);
// 复杂对象连接时传引用 避免复制
Boolean operator==(String const &s1, String const &s2);
Boolean operator==(String const &s1, Char const * const &s2);
Boolean operator==(Char const * const &s1, String const &s2);

template<typename T>
Boolean operator!=(String const &s, T const &v);
template<typename T>
Boolean operator!=(T const &v, String const &s);
// 复杂对象连接时传引用 避免复制
Boolean operator!=(String const &s1, String const &s2);
Boolean operator!=(String const &s1, Char const * const &s2);
Boolean operator!=(Char const * const &s1, String const &s2);



}
#endif