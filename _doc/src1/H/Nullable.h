#ifndef NULLABLE_H
#define NULLABLE_H
namespace Lib3 {



// 可空数据类型系列
// 
// 数据存放基类 for Nullable。较短的值类型特殊处理，令内存占用降低。较长的通通按８字节对齐
template<typename T>
struct NullableBase {
protected:
    ALIGN8( Byte            _buffer[sizeof(T)] );                       // 值容器（刚开始只初始化空间，但不构造。8 字节对齐保障兼容性）
    Boolean                 _hasValue;                                  // 是否有值
};
// _buffer 声明为 1 长数组以方便类型转换代码统一书写
template<> struct NullableBase<SByte  > { SByte   _buffer[1]; Boolean _hasValue; }; // SByte 同时也是 SByte
template<> struct NullableBase<Int16  > { Int16   _buffer[1]; Boolean _hasValue; };
template<> struct NullableBase<Int32  > { Int32   _buffer[1]; Boolean _hasValue; };
template<> struct NullableBase<Byte  > { Byte   _buffer[1]; Boolean _hasValue; };
template<> struct NullableBase<UInt16 > { UInt16  _buffer[1]; Boolean _hasValue; };
template<> struct NullableBase<UInt32 > { UInt32  _buffer[1]; Boolean _hasValue; };
template<> struct NullableBase<Char   > { Char    _buffer[1]; Boolean _hasValue; };
template<> struct NullableBase<Single  > { Single   _buffer[1]; Boolean _hasValue; };
template<> struct NullableBase<Boolean> { Boolean _buffer[1]; Boolean _hasValue; };




// 可空数据类型
template<typename T>
struct Nullable : public NullableBase<T> {
	Nullable();                                                         // 默认置空，值不初始化（不调构造，不清 0）
	Nullable(T const &v);                                               // 置不空，保存值（复制构造）
    Nullable(T &&v);                                                    // 置不空，保存值（复制构造） (右值版)

	Nullable(NullType const &v);                                        // 以空来初始化
    Nullable(Nullable<T> const &v);                                     // 复制构造
    Nullable(Nullable<T> &&v);                                          // 复制构造(右值版)

	Nullable &              operator=(T const &v);                      // 看情况析构, 复制值，置不空
	Nullable &              operator=(T &&v);                           // 看情况析构, 复制值，置不空（右值版）
	Nullable &              operator=(Nullable<T> const &v) ;           // 看情况析构, 复制值，置不空
	Nullable &              operator=(Nullable<T> &&v);                 // 看情况析构, 复制值，置不空（右值版）
	Nullable &              operator=(NullType const &v);               // 看情况析构, 置空

    Boolean                 operator==(T const &v) const;               // 系列判断 (如果 !HasValue 将返回 false) 如果原始数据类型未重载这些 operator 将会出错
	Boolean                 operator!=(T const &v) const;               // 
	Boolean                 operator< (T const &v) const;               //
	Boolean                 operator<=(T const &v) const;               //
	Boolean                 operator> (T const &v) const;               //
	Boolean                 operator>=(T const &v) const;               //

    Boolean                 operator==(Nullable<T> const &v) const;     // 系列判断 (如果都 !HasValue, 将返回 false)
    Boolean                 operator!=(Nullable<T> const &v) const;     //
    Boolean                 operator< (Nullable<T> const &v) const;     //
    Boolean                 operator<=(Nullable<T> const &v) const;     //
    Boolean                 operator> (Nullable<T> const &v) const;     //
    Boolean                 operator>=(Nullable<T> const &v) const;     //

    Boolean                 operator==(NullType const &v) const;        // 和空值比较（等同于 HasValue)

    Boolean &               HasValue();                                 // 引用 是否有值。警告：修改操作主要供 序列化还原 相关使用
    T &                     Value();                                    // 引用 值。警告：如果 !HasValue 也不会报错，但值是未初化的
    Boolean const &         HasValue() const;                           // 引用 是否有值
    T const &               Value() const;                              // 引用 值
    operator                T() const;                                  // 默认允许转换成值的类型。警告：如果 !HasValue 也不会报错，但值是未初化的
};




}   // namespace
#endif
