#ifndef GUID_H
#define GUID_H
namespace Lib3 {




// UUID 数据类型 (参考自 poco)
// 因为是简单类型。不实现复制构造&operator=和析构
struct Guid {
	enum Version {
		UUID_TIME_BASED     = 0x01,
		UUID_DCE_UID        = 0x02,
		UUID_NAME_BASED     = 0x03,
		UUID_RANDOM         = 0x04
	};
	Guid();                                                             // 啥都不干
	Guid(SByte const * const &s);                                        // 通过一个 UUID 字串来创建 (转换：定长，固定格式, 下同)
    Guid(Char const * const &s);
    Guid(Byte const * const &buf);                                      // 从网络数据构造（本库的网络数据统一为小尾标准）
    void                    Set(SByte const * const &s);                 // 这几个重载类似构造函数
    void                    Set(Char const * const &s);
    void                    Set(Byte const * const &buf);
    Guid &                  operator=(SByte const * const &s);           // 这几个重载类似构造函数
    Guid &                  operator=(Char const * const &s);
    Guid &                  operator=(Byte const * const &buf);
    void                    SetZero();                                  // 将值设成 0
    void                    SetVersion(Version const &v);               // 设置版本
	Version                 GetVersion() const;                         // 获取版本
	Int32                   GetVariant() const;                         // 获取变化部分  0: NCS 向下兼容  2: Leach-Salz  6: 为微软向下兼容保留  7: 为新特性保留
	Boolean                 IsNull() const;                             // 判断值是否全是 0
	Boolean                 operator==(Guid const &guid) const;
	Boolean                 operator!=(Guid const &guid) const;
	Boolean                 operator< (Guid const &guid) const;
	Boolean                 operator<=(Guid const &guid) const;
	Boolean                 operator> (Guid const &guid) const;
	Boolean                 operator>=(Guid const &guid) const;

    static Guid             NewGuid();                                  // 产生一个随机值
protected:
	Guid(UInt32 const &timeLow, UInt16 const &timeMid, UInt16 const &timeHiAndVersion, UInt16 const &clockSeq, Byte const * const &node);
	Int32                   Compare(Guid const &guid) const;
	static UInt32           Nibble(UInt32 const &c);                    // 将 a~f, A~F, 0~9 转成相应的数值
    template<typename CharType>
    void                    SetByStr(CharType const * const &s);        // 通过字串来填充值
private:                                                                // 加起来 16 字节
	UInt32                  _timeLow;
	UInt16                  _timeMid;
	UInt16                  _timeHiAndVersion;
	UInt16                  _clockSeq;                                  // 为与 .net 保持一致性，Guid 字串中，这 2 位当 Byte[2] 处理
	Byte                   _node[6];
};



}
#endif
