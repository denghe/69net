#ifndef LINKEDLIST_H
#define LINKEDLIST_H
namespace Lib3 {



// 有序的，批量内存请求的，扩容不会导致复制的，可遍历的，可索引的，支持按索引移除的容器
// todo: Sort, Contains 啥啥
template <typename T>
struct LinkedList {
    enum Settings {
        List_OriginalBufferLength               = 16
    };
    LinkedList();                                                               // 默认创建含有 List_OriginalBufferLength 个元素存放空间的 Buffer
    explicit LinkedList(UInt32 const &len);                                     // 指定初始容器长
    LinkedList(LinkedList const &o);                                            // 深度复制
    ~LinkedList();                                                              // 析构元素，释放内存块
    LinkedList &            Add(T const &v);                                    // 添加元素
    LinkedList &            Add(T &&v);                                         // 添加元素（右值版）
    LinkedList &            RemoveAt(UInt32 const &idx);                        // 移除元素 (当前索引与尾索引交换, Len-1, 析构)
    LinkedList &            Remove(UInt32 const &offset, UInt32 const &len);    // 移除一段元素 (析构这段数据，把后面的索引移上来，修正 Len)，如果 len 传 0 , 相当于删 offset 起后所有数据
    LinkedList &            Remove(UInt32 const &offset);                       // 移除 offset 起，后面的所有元素 (析构这段数据，修正 Len)
    LinkedList &            Clear();                                            // 清数据但并不回收已请求内存
    LinkedList &            Insert(UInt32 const &idx, T const &v);              // 往 idx 索引入插入一个值
    LinkedList &            Insert(UInt32 const &idx, T &&v);                   // 往 idx 索引入插入一个值（右值版）
    Boolean                    Contains(T const &t) const;                         // 判断 LinkedList 中是否含有 t
    Boolean                    operator==(LinkedList const &o) const;              // 逐个判断元素是否相等
    T &                     operator[](UInt32 const &idx);                      // 引用索引处的元素值
    T const &               operator[](UInt32 const &idx) const;                // 引用索引处的元素值
    UInt32 &                Length();                                           // 引用 _length (已有元素个数)
    UInt32 const &          Length() const;                                     // 引用 _length (已有元素个数)
    void                    EnsureLength(UInt32 const &len);                    // 确保空间 且置当前 Length 为 len
    void                    EnsureSpace(UInt32 const &len);                     // 确保所剩空间足够
private:
    Byte                    _originalBuffer[sizeof(T*) * List_OriginalBufferLength + sizeof(T) * List_OriginalBufferLength];   // 原始 Buffer 不需要加 指向下一块 Buffer 的指针头
    Byte *                  _buffer;                                             // 指向原始或新 new Buffer. 头部含指向下一块 Buffer 的指针. resize 创建一倍大的新内存块
    T **                    _indexs;                                             // 始终指向 Buffer + 8 的位置
    UInt32                  _indexsLength;                                       // 当前 Buffer 中的 Indexs 个数 (同时也是总共的) resize += 原 IndexsLength * 2
    UInt32                  _length;                                             // 初始为 0

    void                    Init();
    void                    Init(UInt32 const &len);
    void                    Resize(UInt32 const &len);
};




}
#endif