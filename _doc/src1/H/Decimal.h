#ifndef DECIMAL_H
#define DECIMAL_H
namespace Lib3 {



// 自定义定点数格式 (相当于对定点数做压缩存储)
// 存储格式为 最高位 符号位, 31 整数,  32 小数  (也就是 Int32 + UInt32)
union Decimal
{
	Decimal();                                                          // 啥都不干
    Decimal(Int32 const &i, UInt32 const &digi);                        // 用整数 + 小数方式初始化
    Decimal(Int32 const &v);                                            // 用整数初始化 (小数部分为 0)
    Decimal(Single const &v);                                            // 用浮点初始化
    Decimal(Double const &v);                                           // 用双精初始化
    template<UInt32 len>
    Decimal(SByte const (&s)[len]);                                      // 从定长资源字串初始化
    Decimal(SByte const * const &s, UInt32 const &len);
    Decimal(Byte const * const &buf);                                   // 从 8 字节网络数据构造（本库的网络数据统一为小尾标准）
    void                    Set(Int32 const &i, UInt32 const &digi);    // 这几个重载类似构造函数
    void                    Set(Int32 const &v); 
    void                    Set(Single const &v); 
    void                    Set(Double const &v);
    template<UInt32 len>
    void                    Set(SByte const (&s)[len]);
    void                    Set(SByte const * const &s, UInt32 const &len);
    void                    Set(Byte const * const &buf);
    Decimal &               operator=(Int32 const &v);                  // 这几个重载类似构造函数
    Decimal &               operator=(Single const &v);
    Decimal &               operator=(Double const &v);
    template<UInt32 len>
    Decimal &               operator=(SByte const (&s)[len]);
    Decimal &               operator=(Byte const * const &buf);
    Int32 &                 Int();                                      // 引用整数部分
    Int32 const &           Int() const;                                // 引用整数部分
	UInt32 &                Digi();                                     // 引用小数部分
	UInt32 const &          Digi() const;                               // 引用小数部分
    UInt64 &                Value();                                    // 引用所有数据 (可以用 Value() == 0 或 = 0 来判断或设置 0 值)
    UInt64 const &          Value() const;                              // 引用所有数据 (可以用 Value() == 0 或 = 0 来判断或设置 0 值)
	Boolean                 operator==(Decimal const &v) const;
	Boolean                 operator!=(Decimal const &v) const;
	Boolean                 operator< (Decimal const &v) const;
	Boolean                 operator<=(Decimal const &v) const;
	Boolean                 operator> (Decimal const &v) const;
	Boolean                 operator>=(Decimal const &v) const;
    operator                Int32() const;
    operator                Single() const;
    operator                Double() const;
protected:
	Int32                   Compare(Decimal const &v) const;
    static void             ShiftLeft(UInt32 &v);                       // 将一个数以 10 进制“升位”左移 以实现小数点后数字补0到定长的效果
    template<typename T>
    static T                ShiftRight(UInt32 const &n);                // 将一个整数以 10 进制右移到小数点后返回
private:
	UInt64                  _value;                                     // 数据主体。H: 整数部分(带符号).  L: 小数部分
	struct {
		UInt32              _low;
		Int32               _high;
	};
};




}
#endif
