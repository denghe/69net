#ifndef LIST_H
#define LIST_H
namespace Lib3 {


// 有序的，批量内存请求的，扩容不会导致复制的，可遍历的，可索引的，支持按索引移除的容器
// todo: Sort, Contains 啥啥
template<typename T>
struct List {
    List();                                                                 // 默认创建含有 OriBufLen 个元素存放空间的 Buffer
    explicit List(UInt32 const &len);                                       // 指定初始容器长
    List(List const &o);                                                    // 深度复制  todo: 这个其实可以再根据具体数据类型来优化复制过程，下同
    template<UInt32 len>
    List(T const (&arr)[len]);                                              // 复制数组的数据来作为初始化数据
    ~List();                                                                // 析构元素，释放内存块
    List &              Add(T const &v);                                    // 添加元素
    List &              Add(T &&v);                                         // 添加元素（右值版）
    List &              RemoveAt(UInt32 const &idx);                        // 移除元素 (当前索引与尾索引交换, Len-1, 析构)
    List &              Remove(UInt32 const &offset, UInt32 const &len);    // 移除一段元素 (析构这段数据，把后面的索引移上来，修正 Len)，如果 len 传 0 , 相当于删 offset 起后所有数据
    List &              Remove(UInt32 const &offset);                       // 移除 offset 起，后面的所有元素 (析构这段数据，修正 Len)
    List &              Clear();                                            // 清数据但并不回收已请求内存
    List &              Insert(UInt32 const &idx, T const &v);              // 往 idx 索引入插入一个值
    List &              Insert(UInt32 const &idx, T &&v);                   // 往 idx 索引入插入一个值（右值版）
    Boolean             Contains(T const &t) const;                         // 判断 List 中是否含有 t
    Boolean             operator==(List const &o) const;                    // 逐个判断元素是否相等
    T &                 operator[](UInt32 const &idx);                      // 引用索引处的元素值
    T const &           operator[](UInt32 const &idx) const;                // 引用索引处的元素值
    List &              operator<<(T const &t);                             // 在最后面追加 t
    List &              operator<<(T &&t);                                  // 在最后面追加 t (右值版)
    UInt32 &            Length();                                           // 引用 _count (已有元素个数)
    UInt32 const &      Length() const;                                     // 引用 _count (已有元素个数)
    T *                 Buffer();                                           // 引用 _buffer (数据区，只对 Array 态有效)
    T * const           Buffer() const;                                     // 引用 _buffer (数据区，只对 Array 态有效)
    void                EnsureLength(UInt32 const &len);                    // 确保空间 且置当前 Length 为 len
    void                EnsureSpace(UInt32 const &len);                     // 确保所剩空间足够
// todo: 加 Add(), Remove(p) 函数. Add() 的功能是直接得到内存指针，但并不执行构造函数，相当于 add 了一个空对象. remove 指针，不析构
protected:
    void                Init();                                             // 置变量为 0
    void                Init(UInt32 const &len);                            // 请求内存
    void                InitArray(UInt32 const &len);                       // 用于 Init(len) 调
    void                ConsFromArray(List<T> const &o);                    // memcpy 被 复制构造 调
    template<UInt32 len>
    void                ConsFromFixedArray(T const (&arr)[len]);            // memcpy 被 从数组复制 构造 调
    void                FreeArray();                                        // 不调析构, 释放当前内存块 被 析构 调
    void                Resize(UInt32 const &len);                          // 重分配内存，被 Add 调
    void                ResizeArray(UInt32 const &len);                     // 被 Resize 调
    List &              RemoveAtArray(UInt32 const &idx);                   // 被 RemoveAt 调
    List &              RemoveArray(UInt32 const &offset, UInt32 const &len);   // 被 Remove 调
    List &              RemoveArray(UInt32 const &offset);                  // 被 Remove 调
    List &              InsertArray(UInt32 const &idx, T const &v);         // 被 Insert 调
    List &              InsertArray(UInt32 const &idx, T &&v);              // 被 Insert 调
private:
    Byte *              _buffer;                                            // 指向原始或新 new Buffer. 头部含指向下一块 Buffer 的指针. resize 创建一倍大的新内存块
    UInt32              _count;                                             // Buffer 中已有的数据个数。初始为 0
    UInt32              _length;                                            // 当前 Buffer 中的 Indexs 个数 (数组形态为当前 Buffer 最大元素个数容量)
        
};



template<> Byte              * List<Byte             >::Buffer();
template<> UInt16            * List<UInt16           >::Buffer();
template<> UInt32            * List<UInt32           >::Buffer();
template<> SByte             * List<SByte            >::Buffer();
template<> Int16             * List<Int16            >::Buffer();
template<> Int32             * List<Int32            >::Buffer();
template<> Single            * List<Single           >::Buffer();
template<> Char              * List<Char             >::Buffer();
template<> Boolean           * List<Boolean          >::Buffer();
template<> Nullable<Byte   > * List<Nullable<Byte   >>::Buffer();
template<> Nullable<UInt16 > * List<Nullable<UInt16 >>::Buffer();
template<> Nullable<SByte  > * List<Nullable<SByte  >>::Buffer();
template<> Nullable<Int16  > * List<Nullable<Int16  >>::Buffer();
template<> Nullable<Char   > * List<Nullable<Char   >>::Buffer();
template<> Nullable<Boolean> * List<Nullable<Boolean>>::Buffer();
#ifdef __X64
template<> UInt64            * List<UInt64           >::Buffer();
template<> Int64             * List<Int64            >::Buffer();
template<> Double            * List<Double           >::Buffer();
template<> Decimal           * List<Decimal          >::Buffer();
template<> Nullable<UInt32 > * List<Nullable<UInt32 >>::Buffer();
template<> Nullable<Int32  > * List<Nullable<Int32  >>::Buffer();
template<> Nullable<Single > * List<Nullable<Single >>::Buffer();
#endif



template<> Byte              * const List<Byte             >::Buffer() const;
template<> UInt16            * const List<UInt16           >::Buffer() const;
template<> UInt32            * const List<UInt32           >::Buffer() const;
template<> SByte             * const List<SByte            >::Buffer() const;
template<> Int16             * const List<Int16            >::Buffer() const;
template<> Int32             * const List<Int32            >::Buffer() const;
template<> Single            * const List<Single           >::Buffer() const;
template<> Char              * const List<Char             >::Buffer() const;
template<> Boolean           * const List<Boolean          >::Buffer() const;
template<> Nullable<Byte   > * const List<Nullable<Byte   >>::Buffer() const;
template<> Nullable<UInt16 > * const List<Nullable<UInt16 >>::Buffer() const;
template<> Nullable<SByte  > * const List<Nullable<SByte  >>::Buffer() const;
template<> Nullable<Int16  > * const List<Nullable<Int16  >>::Buffer() const;
template<> Nullable<Char   > * const List<Nullable<Char   >>::Buffer() const;
template<> Nullable<Boolean> * const List<Nullable<Boolean>>::Buffer() const;
#ifdef __X64
template<> UInt64            * const List<UInt64           >::Buffer() const;
template<> Int64             * const List<Int64            >::Buffer() const;
template<> Double            * const List<Double           >::Buffer() const;
template<> Decimal           * const List<Decimal          >::Buffer() const;
template<> Nullable<UInt32 > * const List<Nullable<UInt32 >>::Buffer() const;
template<> Nullable<Int32  > * const List<Nullable<Int32  >>::Buffer() const;
template<> Nullable<Single > * const List<Nullable<Single >>::Buffer() const;
#endif


template<> void List<Byte             >::Init(UInt32 const &len);
template<> void List<UInt16           >::Init(UInt32 const &len);
template<> void List<UInt32           >::Init(UInt32 const &len);
template<> void List<SByte            >::Init(UInt32 const &len);
template<> void List<Int16            >::Init(UInt32 const &len);
template<> void List<Int32            >::Init(UInt32 const &len);
template<> void List<Single           >::Init(UInt32 const &len);
template<> void List<Char             >::Init(UInt32 const &len);
template<> void List<Boolean          >::Init(UInt32 const &len);
template<> void List<Nullable<Byte   >>::Init(UInt32 const &len);
template<> void List<Nullable<UInt16 >>::Init(UInt32 const &len);
template<> void List<Nullable<SByte  >>::Init(UInt32 const &len);
template<> void List<Nullable<Int16  >>::Init(UInt32 const &len);
template<> void List<Nullable<Char   >>::Init(UInt32 const &len);
template<> void List<Nullable<Boolean>>::Init(UInt32 const &len);
#ifdef __X64
template<> void List<UInt64           >::Init(UInt32 const &len);
template<> void List<Int64            >::Init(UInt32 const &len);
template<> void List<Double           >::Init(UInt32 const &len);
template<> void List<Decimal          >::Init(UInt32 const &len);
template<> void List<Nullable<UInt32 >>::Init(UInt32 const &len);
template<> void List<Nullable<Int32  >>::Init(UInt32 const &len);
template<> void List<Nullable<Single >>::Init(UInt32 const &len);
#endif


template<> List<Byte             >::List(List<Byte               > const &o);
template<> List<UInt16           >::List(List<UInt16             > const &o);
template<> List<UInt32           >::List(List<UInt32             > const &o);
template<> List<SByte            >::List(List<SByte              > const &o);
template<> List<Int16            >::List(List<Int16              > const &o);
template<> List<Int32            >::List(List<Int32              > const &o);
template<> List<Single           >::List(List<Single             > const &o);
template<> List<Char             >::List(List<Char               > const &o);
template<> List<Boolean          >::List(List<Boolean            > const &o);
template<> List<Nullable<Byte   >>::List(List<Nullable<Byte     >> const &o);
template<> List<Nullable<UInt16 >>::List(List<Nullable<UInt16   >> const &o);
template<> List<Nullable<SByte  >>::List(List<Nullable<SByte    >> const &o);
template<> List<Nullable<Int16  >>::List(List<Nullable<Int16    >> const &o);
template<> List<Nullable<Char   >>::List(List<Nullable<Char     >> const &o);
template<> List<Nullable<Boolean>>::List(List<Nullable<Boolean  >> const &o);
#ifdef __X64
template<> List<UInt64           >::List(List<UInt64             > const &o);
template<> List<Int64            >::List(List<Int64              > const &o);
template<> List<Double           >::List(List<Double             > const &o);
template<> List<Decimal          >::List(List<Decimal            > const &o);
template<> List<Nullable<UInt32 >>::List(List<Nullable<UInt32   >> const &o);
template<> List<Nullable<Int32  >>::List(List<Nullable<Int32    >> const &o);
template<> List<Nullable<Single >>::List(List<Nullable<Single   >> const &o);
#endif


template<> template<UInt32 len> List<Byte             >::List(Byte              const (&arr)[len]);
template<> template<UInt32 len> List<UInt16           >::List(UInt16            const (&arr)[len]);
template<> template<UInt32 len> List<UInt32           >::List(UInt32            const (&arr)[len]);
template<> template<UInt32 len> List<SByte            >::List(SByte             const (&arr)[len]);
template<> template<UInt32 len> List<Int16            >::List(Int16             const (&arr)[len]);
template<> template<UInt32 len> List<Int32            >::List(Int32             const (&arr)[len]);
template<> template<UInt32 len> List<Single           >::List(Single            const (&arr)[len]);
template<> template<UInt32 len> List<Char             >::List(Char              const (&arr)[len]);
template<> template<UInt32 len> List<Boolean          >::List(Boolean           const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<Byte   >>::List(Nullable<Byte   > const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<UInt16 >>::List(Nullable<UInt16 > const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<SByte  >>::List(Nullable<SByte  > const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<Int16  >>::List(Nullable<Int16  > const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<Char   >>::List(Nullable<Char   > const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<Boolean>>::List(Nullable<Boolean> const (&arr)[len]);
#ifdef __X64
template<> template<UInt32 len> List<UInt64           >::List(UInt64            const (&arr)[len]);
template<> template<UInt32 len> List<Int64            >::List(Int64             const (&arr)[len]);
template<> template<UInt32 len> List<Double           >::List(Double            const (&arr)[len]);
template<> template<UInt32 len> List<Decimal          >::List(Decimal           const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<UInt32 >>::List(Nullable<UInt32 > const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<Int32  >>::List(Nullable<Int32  > const (&arr)[len]);
template<> template<UInt32 len> List<Nullable<Single >>::List(Nullable<Single > const (&arr)[len]);
#endif


template<> List<Byte             >::~List();
template<> List<UInt16           >::~List();
template<> List<UInt32           >::~List();
template<> List<SByte            >::~List();
template<> List<Int16            >::~List();
template<> List<Int32            >::~List();
template<> List<Single           >::~List();
template<> List<Char             >::~List();
template<> List<Boolean          >::~List();
template<> List<Nullable<Byte   >>::~List();
template<> List<Nullable<UInt16 >>::~List();
template<> List<Nullable<SByte  >>::~List();
template<> List<Nullable<Int16  >>::~List();
template<> List<Nullable<Char   >>::~List();
template<> List<Nullable<Boolean>>::~List();
#ifdef __X64             
template<> List<UInt64           >::~List();
template<> List<Int64            >::~List();
template<> List<Double           >::~List();
template<> List<Decimal          >::~List();
template<> List<Nullable<UInt32 >>::~List();
template<> List<Nullable<Int32  >>::~List();
template<> List<Nullable<Single >>::~List();
#endif


template<> void List<Byte             >::Resize(UInt32 const &len);
template<> void List<UInt16           >::Resize(UInt32 const &len);
template<> void List<UInt32           >::Resize(UInt32 const &len);
template<> void List<SByte            >::Resize(UInt32 const &len);
template<> void List<Int16            >::Resize(UInt32 const &len);
template<> void List<Int32            >::Resize(UInt32 const &len);
template<> void List<Single           >::Resize(UInt32 const &len);
template<> void List<Char             >::Resize(UInt32 const &len);
template<> void List<Boolean          >::Resize(UInt32 const &len);
template<> void List<Nullable<Byte   >>::Resize(UInt32 const &len);
template<> void List<Nullable<UInt16 >>::Resize(UInt32 const &len);
template<> void List<Nullable<SByte  >>::Resize(UInt32 const &len);
template<> void List<Nullable<Int16  >>::Resize(UInt32 const &len);
template<> void List<Nullable<Char   >>::Resize(UInt32 const &len);
template<> void List<Nullable<Boolean>>::Resize(UInt32 const &len);
#ifdef __X64
template<> void List<UInt64           >::Resize(UInt32 const &len);
template<> void List<Int64            >::Resize(UInt32 const &len);
template<> void List<Double           >::Resize(UInt32 const &len);
template<> void List<Decimal          >::Resize(UInt32 const &len);
template<> void List<Nullable<UInt32 >>::Resize(UInt32 const &len);
template<> void List<Nullable<Int32  >>::Resize(UInt32 const &len);
template<> void List<Nullable<Single >>::Resize(UInt32 const &len);
#endif


template<> List<Byte             > & List<Byte             >::RemoveAt(UInt32 const &idx);
template<> List<UInt16           > & List<UInt16           >::RemoveAt(UInt32 const &idx);
template<> List<UInt32           > & List<UInt32           >::RemoveAt(UInt32 const &idx);
template<> List<SByte            > & List<SByte            >::RemoveAt(UInt32 const &idx);
template<> List<Int16            > & List<Int16            >::RemoveAt(UInt32 const &idx);
template<> List<Int32            > & List<Int32            >::RemoveAt(UInt32 const &idx);
template<> List<Single           > & List<Single           >::RemoveAt(UInt32 const &idx);
template<> List<Char             > & List<Char             >::RemoveAt(UInt32 const &idx);
template<> List<Boolean          > & List<Boolean          >::RemoveAt(UInt32 const &idx);
template<> List<Nullable<Byte   >> & List<Nullable<Byte   >>::RemoveAt(UInt32 const &idx);
template<> List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::RemoveAt(UInt32 const &idx);
template<> List<Nullable<SByte  >> & List<Nullable<SByte  >>::RemoveAt(UInt32 const &idx);
template<> List<Nullable<Int16  >> & List<Nullable<Int16  >>::RemoveAt(UInt32 const &idx);
template<> List<Nullable<Char   >> & List<Nullable<Char   >>::RemoveAt(UInt32 const &idx);
template<> List<Nullable<Boolean>> & List<Nullable<Boolean>>::RemoveAt(UInt32 const &idx);
#ifdef __X64                                                           
template<> List<UInt64           > & List<UInt64           >::RemoveAt(UInt32 const &idx);
template<> List<Int64            > & List<Int64            >::RemoveAt(UInt32 const &idx);
template<> List<Double           > & List<Double           >::RemoveAt(UInt32 const &idx);
template<> List<Decimal          > & List<Decimal          >::RemoveAt(UInt32 const &idx);
template<> List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::RemoveAt(UInt32 const &idx);
template<> List<Nullable<Int32  >> & List<Nullable<Int32  >>::RemoveAt(UInt32 const &idx);
template<> List<Nullable<Single >> & List<Nullable<Single >>::RemoveAt(UInt32 const &idx);
#endif


template<> Byte              & List<Byte             >::operator[](UInt32 const &idx);
template<> UInt16            & List<UInt16           >::operator[](UInt32 const &idx);
template<> UInt32            & List<UInt32           >::operator[](UInt32 const &idx);
template<> SByte             & List<SByte            >::operator[](UInt32 const &idx);
template<> Int16             & List<Int16            >::operator[](UInt32 const &idx);
template<> Int32             & List<Int32            >::operator[](UInt32 const &idx);
template<> Single            & List<Single           >::operator[](UInt32 const &idx);
template<> Char              & List<Char             >::operator[](UInt32 const &idx);
template<> Boolean           & List<Boolean          >::operator[](UInt32 const &idx);
template<> Nullable<Byte   > & List<Nullable<Byte   >>::operator[](UInt32 const &idx);
template<> Nullable<UInt16 > & List<Nullable<UInt16 >>::operator[](UInt32 const &idx);
template<> Nullable<SByte  > & List<Nullable<SByte  >>::operator[](UInt32 const &idx);
template<> Nullable<Int16  > & List<Nullable<Int16  >>::operator[](UInt32 const &idx);
template<> Nullable<Char   > & List<Nullable<Char   >>::operator[](UInt32 const &idx);
template<> Nullable<Boolean> & List<Nullable<Boolean>>::operator[](UInt32 const &idx);
#ifdef __X64                                                       
template<> UInt64            & List<UInt64           >::operator[](UInt32 const &idx);
template<> Int64             & List<Int64            >::operator[](UInt32 const &idx);
template<> Double            & List<Double           >::operator[](UInt32 const &idx);
template<> Decimal           & List<Decimal          >::operator[](UInt32 const &idx);
template<> Nullable<UInt32 > & List<Nullable<UInt32 >>::operator[](UInt32 const &idx);
template<> Nullable<Int32  > & List<Nullable<Int32  >>::operator[](UInt32 const &idx);
template<> Nullable<Single > & List<Nullable<Single >>::operator[](UInt32 const &idx);
#endif


template<> Byte              const & List<Byte             >::operator[](UInt32 const &idx) const;
template<> UInt16            const & List<UInt16           >::operator[](UInt32 const &idx) const;
template<> UInt32            const & List<UInt32           >::operator[](UInt32 const &idx) const;
template<> SByte             const & List<SByte            >::operator[](UInt32 const &idx) const;
template<> Int16             const & List<Int16            >::operator[](UInt32 const &idx) const;
template<> Int32             const & List<Int32            >::operator[](UInt32 const &idx) const;
template<> Single            const & List<Single           >::operator[](UInt32 const &idx) const;
template<> Char              const & List<Char             >::operator[](UInt32 const &idx) const;
template<> Boolean           const & List<Boolean          >::operator[](UInt32 const &idx) const;
template<> Nullable<Byte   > const & List<Nullable<Byte   >>::operator[](UInt32 const &idx) const;
template<> Nullable<UInt16 > const & List<Nullable<UInt16 >>::operator[](UInt32 const &idx) const;
template<> Nullable<SByte  > const & List<Nullable<SByte  >>::operator[](UInt32 const &idx) const;
template<> Nullable<Int16  > const & List<Nullable<Int16  >>::operator[](UInt32 const &idx) const;
template<> Nullable<Char   > const & List<Nullable<Char   >>::operator[](UInt32 const &idx) const;
template<> Nullable<Boolean> const & List<Nullable<Boolean>>::operator[](UInt32 const &idx) const;
#ifdef __X64                                                                   
template<> UInt64            const & List<UInt64           >::operator[](UInt32 const &idx) const;
template<> Int64             const & List<Int64            >::operator[](UInt32 const &idx) const;
template<> Double            const & List<Double           >::operator[](UInt32 const &idx) const;
template<> Decimal           const & List<Decimal          >::operator[](UInt32 const &idx) const;
template<> Nullable<UInt32 > const & List<Nullable<UInt32 >>::operator[](UInt32 const &idx) const;
template<> Nullable<Int32  > const & List<Nullable<Int32  >>::operator[](UInt32 const &idx) const;
template<> Nullable<Single > const & List<Nullable<Single >>::operator[](UInt32 const &idx) const;
#endif


template<> List<Byte             > & List<Byte             >::Remove(UInt32 const &offset);
template<> List<UInt16           > & List<UInt16           >::Remove(UInt32 const &offset);
template<> List<UInt32           > & List<UInt32           >::Remove(UInt32 const &offset);
template<> List<SByte            > & List<SByte            >::Remove(UInt32 const &offset);
template<> List<Int16            > & List<Int16            >::Remove(UInt32 const &offset);
template<> List<Int32            > & List<Int32            >::Remove(UInt32 const &offset);
template<> List<Single           > & List<Single           >::Remove(UInt32 const &offset);
template<> List<Char             > & List<Char             >::Remove(UInt32 const &offset);
template<> List<Boolean          > & List<Boolean          >::Remove(UInt32 const &offset);
template<> List<Nullable<Byte   >> & List<Nullable<Byte   >>::Remove(UInt32 const &offset);
template<> List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Remove(UInt32 const &offset);
template<> List<Nullable<SByte  >> & List<Nullable<SByte  >>::Remove(UInt32 const &offset);
template<> List<Nullable<Int16  >> & List<Nullable<Int16  >>::Remove(UInt32 const &offset);
template<> List<Nullable<Char   >> & List<Nullable<Char   >>::Remove(UInt32 const &offset);
template<> List<Nullable<Boolean>> & List<Nullable<Boolean>>::Remove(UInt32 const &offset);
#ifdef __X64                                                            
template<> List<UInt64           > & List<UInt64           >::Remove(UInt32 const &offset);
template<> List<Int64            > & List<Int64            >::Remove(UInt32 const &offset);
template<> List<Double           > & List<Double           >::Remove(UInt32 const &offset);
template<> List<Decimal          > & List<Decimal          >::Remove(UInt32 const &offset);
template<> List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Remove(UInt32 const &offset);
template<> List<Nullable<Int32  >> & List<Nullable<Int32  >>::Remove(UInt32 const &offset);
template<> List<Nullable<Single >> & List<Nullable<Single >>::Remove(UInt32 const &offset);
#endif


template<> List<Byte             > & List<Byte             >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<UInt16           > & List<UInt16           >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<UInt32           > & List<UInt32           >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<SByte            > & List<SByte            >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Int16            > & List<Int16            >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Int32            > & List<Int32            >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Single           > & List<Single           >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Char             > & List<Char             >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Boolean          > & List<Boolean          >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<Byte   >> & List<Nullable<Byte   >>::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<SByte  >> & List<Nullable<SByte  >>::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<Int16  >> & List<Nullable<Int16  >>::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<Char   >> & List<Nullable<Char   >>::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<Boolean>> & List<Nullable<Boolean>>::Remove(UInt32 const &offset, UInt32 const &len);
#ifdef __X64                                                                               
template<> List<UInt64           > & List<UInt64           >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Int64            > & List<Int64            >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Double           > & List<Double           >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Decimal          > & List<Decimal          >::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<Int32  >> & List<Nullable<Int32  >>::Remove(UInt32 const &offset, UInt32 const &len);
template<> List<Nullable<Single >> & List<Nullable<Single >>::Remove(UInt32 const &offset, UInt32 const &len);
#endif


template<> List<Byte             > & List<Byte             >::Clear();
template<> List<UInt16           > & List<UInt16           >::Clear();
template<> List<UInt32           > & List<UInt32           >::Clear();
template<> List<SByte            > & List<SByte            >::Clear();
template<> List<Int16            > & List<Int16            >::Clear();
template<> List<Int32            > & List<Int32            >::Clear();
template<> List<Single           > & List<Single           >::Clear();
template<> List<Char             > & List<Char             >::Clear();
template<> List<Boolean          > & List<Boolean          >::Clear();
template<> List<Nullable<Byte   >> & List<Nullable<Byte   >>::Clear();
template<> List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Clear();
template<> List<Nullable<SByte  >> & List<Nullable<SByte  >>::Clear();
template<> List<Nullable<Int16  >> & List<Nullable<Int16  >>::Clear();
template<> List<Nullable<Char   >> & List<Nullable<Char   >>::Clear();
template<> List<Nullable<Boolean>> & List<Nullable<Boolean>>::Clear();
#ifdef __X64                                       
template<> List<UInt64           > & List<UInt64           >::Clear();
template<> List<Int64            > & List<Int64            >::Clear();
template<> List<Double           > & List<Double           >::Clear();
template<> List<Decimal          > & List<Decimal          >::Clear();
template<> List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Clear();
template<> List<Nullable<Int32  >> & List<Nullable<Int32  >>::Clear();
template<> List<Nullable<Single >> & List<Nullable<Single >>::Clear();
#endif


template<> List<Byte             > & List<Byte             >::Insert(UInt32 const &idx, Byte              const &v);
template<> List<UInt16           > & List<UInt16           >::Insert(UInt32 const &idx, UInt16            const &v);
template<> List<UInt32           > & List<UInt32           >::Insert(UInt32 const &idx, UInt32            const &v);
template<> List<SByte            > & List<SByte            >::Insert(UInt32 const &idx, SByte             const &v);
template<> List<Int16            > & List<Int16            >::Insert(UInt32 const &idx, Int16             const &v);
template<> List<Int32            > & List<Int32            >::Insert(UInt32 const &idx, Int32             const &v);
template<> List<Single           > & List<Single           >::Insert(UInt32 const &idx, Single            const &v);
template<> List<Char             > & List<Char             >::Insert(UInt32 const &idx, Char              const &v);
template<> List<Boolean          > & List<Boolean          >::Insert(UInt32 const &idx, Boolean           const &v);
template<> List<Nullable<Byte   >> & List<Nullable<Byte   >>::Insert(UInt32 const &idx, Nullable<Byte   > const &v);
template<> List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Insert(UInt32 const &idx, Nullable<UInt16 > const &v);
template<> List<Nullable<SByte  >> & List<Nullable<SByte  >>::Insert(UInt32 const &idx, Nullable<SByte  > const &v);
template<> List<Nullable<Int16  >> & List<Nullable<Int16  >>::Insert(UInt32 const &idx, Nullable<Int16  > const &v);
template<> List<Nullable<Char   >> & List<Nullable<Char   >>::Insert(UInt32 const &idx, Nullable<Char   > const &v);
template<> List<Nullable<Boolean>> & List<Nullable<Boolean>>::Insert(UInt32 const &idx, Nullable<Boolean> const &v);
#ifdef __X64                                                                                     
template<> List<UInt64           > & List<UInt64           >::Insert(UInt32 const &idx, UInt64            const &v);
template<> List<Int64            > & List<Int64            >::Insert(UInt32 const &idx, Int64             const &v);
template<> List<Double           > & List<Double           >::Insert(UInt32 const &idx, Double            const &v);
template<> List<Decimal          > & List<Decimal          >::Insert(UInt32 const &idx, Decimal           const &v);
template<> List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Insert(UInt32 const &idx, Nullable<UInt32 > const &v);
template<> List<Nullable<Int32  >> & List<Nullable<Int32  >>::Insert(UInt32 const &idx, Nullable<Int32  > const &v);
template<> List<Nullable<Single >> & List<Nullable<Single >>::Insert(UInt32 const &idx, Nullable<Single > const &v);
#endif


template<> List<Byte             > & List<Byte             >::Insert(UInt32 const &idx, Byte              &&v);
template<> List<UInt16           > & List<UInt16           >::Insert(UInt32 const &idx, UInt16            &&v);
template<> List<UInt32           > & List<UInt32           >::Insert(UInt32 const &idx, UInt32            &&v);
template<> List<SByte            > & List<SByte            >::Insert(UInt32 const &idx, SByte             &&v);
template<> List<Int16            > & List<Int16            >::Insert(UInt32 const &idx, Int16             &&v);
template<> List<Int32            > & List<Int32            >::Insert(UInt32 const &idx, Int32             &&v);
template<> List<Single           > & List<Single           >::Insert(UInt32 const &idx, Single            &&v);
template<> List<Char             > & List<Char             >::Insert(UInt32 const &idx, Char              &&v);
template<> List<Boolean          > & List<Boolean          >::Insert(UInt32 const &idx, Boolean           &&v);
template<> List<Nullable<Byte   >> & List<Nullable<Byte   >>::Insert(UInt32 const &idx, Nullable<Byte   > &&v);
template<> List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Insert(UInt32 const &idx, Nullable<UInt16 > &&v);
template<> List<Nullable<SByte  >> & List<Nullable<SByte  >>::Insert(UInt32 const &idx, Nullable<SByte  > &&v);
template<> List<Nullable<Int16  >> & List<Nullable<Int16  >>::Insert(UInt32 const &idx, Nullable<Int16  > &&v);
template<> List<Nullable<Char   >> & List<Nullable<Char   >>::Insert(UInt32 const &idx, Nullable<Char   > &&v);
template<> List<Nullable<Boolean>> & List<Nullable<Boolean>>::Insert(UInt32 const &idx, Nullable<Boolean> &&v);
#ifdef __X64                                                                                
template<> List<UInt64           > & List<UInt64           >::Insert(UInt32 const &idx, UInt64            &&v);
template<> List<Int64            > & List<Int64            >::Insert(UInt32 const &idx, Int64             &&v);
template<> List<Double           > & List<Double           >::Insert(UInt32 const &idx, Double            &&v);
template<> List<Decimal          > & List<Decimal          >::Insert(UInt32 const &idx, Decimal           &&v);
template<> List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Insert(UInt32 const &idx, Nullable<UInt32 > &&v);
template<> List<Nullable<Int32  >> & List<Nullable<Int32  >>::Insert(UInt32 const &idx, Nullable<Int32  > &&v);
template<> List<Nullable<Single >> & List<Nullable<Single >>::Insert(UInt32 const &idx, Nullable<Single > &&v);
#endif



}
#endif
