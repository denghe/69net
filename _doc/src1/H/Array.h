#ifndef ARRAY_H
#define ARRAY_H
namespace Lib3 {



// 变长数组容器 (适合高速操作顺序添加的值类型)
// 要求 T 要么是简单类型，要么已经实现了 默认构造函数, 复制构造函数(含右值版) 并重载 = 号, == 号
// 为简化设计，本容器不实现右值相关，不提供 + << 啥的运算符重载
// todo: Sort 啥的
template <typename T>
struct Array {
    enum Settings {
        Array_OriginalBufferLength      = 8
    };
    Array();                                                                        // 调 Init()
    explicit Array(UInt32 const &len);                                              // 调 Init(len)
    Array(T const * const &a, UInt32 const &len);                                   // 调 Init(a, len)
    template<UInt32 len>
    Array(T const (&a)[len]);                                                       // 调 Init(a, len)
    Array(Array<T> const &a);                                                       // 深度复制 (每个元素依次调复制构造)
    ~Array();                                                                       // 当 OriginalBuffer != Buffer 时 释放 Buffer
	T * &                           Buffer();                                       // 引用 field
    T * const &                     Buffer() const;                                 // 引用 field
    UInt32 &                        BufferLength();                                 // 引用 field
    UInt32 const &                  BufferLength() const;                           // 引用 field
    UInt32 &                        Length();                                       // 引用 field
    UInt32 const &                  Length() const;                                 // 引用 field
    void                            EnsureSpace(UInt32 const &len);                 // 检查 Buffer 剩余空间大小，根据情况 Resize. if len > BufferLength - Length, 则扩容为 (Length + len) * 2
    void                            EnsureLength();                                 // 检查 BufferLength, if Length > BufferLength, 则扩容为 Length  (常用于复制数据进来之前先改 Length 再调这个函数确保空间足够)
    Array<T> &                      Clear();                                        // 析构内容，令 Length = 0, 不清除空间
    Array<T> &                      operator=(Array<T> const &a);                   // 视情况清掉当前数组, 并复制目标数组的内容
    Boolean                         operator==(Array<T> const &a) const;            // 判断两数组是否相等(对比长度，元素。都为空视为相等)
    Boolean                         operator!=(Array<T> const &a) const;            // 判断不等
	T &                             operator[](UInt32 const &idx);                  // 引用某索引所在元素
    T const &                       operator[](UInt32 const &idx) const;            // 引用某索引所在元素
    Array<T> &                      operator<<(T const &t);                         // 在最后面追加 t
    Array<T> &                      operator<<(T &&t);                              // 在最后面追加 t (右值版)
    Array<T> &                      Add(T const &t);                                // 在最后面追加 t
    Array<T> &                      Add(T &&t);                                     // 在最后面追加 t (右值版)
    Array<T> &                      Add(T const * const &a, UInt32 const &len);     // 在最后追加数组 (数组版)
    template<UInt32 len>
    Array<T> &                      Add(T const (&a)[len]);                         // 在最后追加数组 (定长数组版)
    Array<T> &                      Add(Array<T> const &a);                         // 在最后追加数组
    Boolean                         Contains(T const &t) const;                     // 判断数组中是否含有 t
    Int32                           IndexOf(T const &t) const;                      // 返回数组中 t 所在 index. 未找到则返回 -1
    Int32                           IndexOf(UInt32 const &offset, T const &t) const;// 返回数组中 t 从 offset 开始查找的 所在 index. 未找到则返回 -1
    Array<T> &                      Insert(UInt32 const &offset, T const &t);       // 在指定偏移处插入 t. 如果 offset 大于长度, 将追加到最后
    Array<T> &                      Insert(UInt32 const &offset, T &&t);            // 在指定偏移处插入 t. 如果 offset 大于长度, 将追加到最后 (右值版)
    Array<T> &                      Insert(UInt32 const &offset, T const * const &a, UInt32 const &len);  // 在指定偏移处插入数组. 如果 offset 大于长度, 将追加到最后 (数组版)
    template<UInt32 len>            
    Array<T> &                      Insert(UInt32 const &offset, T const (&a)[len]);// 在指定偏移处插入数组. 如果 offset 大于长度, 将追加到最后 (定长数组版)
    Array<T> &                      Insert(UInt32 const &offset, Array<T> const &a);// 在指定偏移处插入数组. 如果 offset 大于长度, 将追加到最后
    Array<T> &                      Remove(UInt32 const &offset);                   // 返回 移除指定偏移后面所有内容 后的新数组
    Array<T> &                      Remove(UInt32 const &offset, UInt32 const &len);// 返回 移除指定偏移后面指定长度的内容 后的新数组
    Array<T> &                      RemoveAt(UInt32 const &idx);                    // 返回 移除指定偏移后面所有内容 后的新数组

private:                                                                            // 初始化的公用代码
    void                            Init();                                         // 默认情况下 Buffer 为 OriginalBuffer, BufferLength = Array_OriginalBufferLength, Length = 0
    void                            Init(UInt32 const &len);                        // 根据长度来初始化, Buffer new, BufferLength = len, Length = 0
    void                            Init(T const * const &a, UInt32 const &len);    // 按传入的数据来初始化, 复制内容, Length = len

    Byte *                          _originalBuffer[sizeof(T) * Array_OriginalBufferLength];   // 自带一个 Buffer
    T *                             _buffer;
    UInt32                          _bufferLength;
    UInt32                          _length;
};




}
#endif