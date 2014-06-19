#ifndef DICTIONARY_H
#define DICTIONARY_H
namespace Lib3 {


// 字典
// 要求 TKey 类型实现复制构造, operator== 重载, 提供 GetHashCode 函数 (当前仅支持 Lib 内建类型), TValue 类型实现复制构造, operator= 重载
template <typename TKey, typename TValue>
struct Dictionary {
    enum Settings {
        Dictionary_OriginalNodeLength           = 63,
        Dictionary_OriginalPrimeIndex           = 0
    };
    struct Node {                                                       // 节点直接包含键值数据
        Node *      Next;                                               // 存放数据后指向本Hash入口下一节点
        Node *      UsedPrev;                                           // 存放数据后指向无分类的上一节点(双向链表，用于检索所有已有数据)
        Node *      UsedNext;                                           // 存放数据后指向无分类的下一节点(双向链表，用于检索所有已有数据)
        Int32       HashCode;
        TKey        Key;
        TValue      Value;
    };
    Dictionary();                                                       // 初始化默认长度Buffer
    explicit Dictionary(UInt32 const &len);                             // 初始化指定长度Buffer. 如果指定长度小于默认长度, 将初始化默认长度
    ~Dictionary();                                                      // 析构所有已有数据，回收历史请求内存块
    Dictionary(Dictionary<TKey, TValue> const &dict);                   // 深度复制 (每个元素依次调复制构造)
    Dictionary<TKey, TValue> & operator=(Dictionary<TKey, TValue> const &dict); // 视情况清掉当前字典, 并深度复制目标字典的数据
	TValue         *operator[](TKey const &key) const;                  // 获得某 Key 对应的 Value 指针，未找到返回空指针
    bool            Add(TKey const &key, TValue const &value, Boolean const &replace = false);   // 添加 k & v 到字典。replace 为 true 时, 替换已存在。返回 key 是否已存在
    bool            Add(TKey &&key, TValue &&value, Boolean const &replace = false);         // 右值版
    bool            Add(TKey const &key, TValue &&value, Boolean const &replace = false);    // 右值版
    bool            Add(TKey &&key, TValue const &value, Boolean const &replace = false);    // 右值版
    void            Clear();                                            // 不清内存, 只是调已有数据的析构, 并将已有数据的节点挪到 FreeNodeHead 链
    Boolean            ContainsKey(TKey const &key) const;                 // 判断是否包含某 key
    Boolean            Get(TKey const &key, TValue &outValue) const;       // 根据键来取值，返回是否存在该项 (存在则复制 Value 到 outValue)
    Boolean            GetAndRemove(TKey const &key, TValue &outValue);    // 从字典中移除 key 项, 返回该项是否存在 (存在则复制 Value 到 outValue)
    Boolean            Remove(TKey const &key);                            // 从字典中移除 key 项, 返回该项是否存在
    Node * const &  UsedNodeHead() const;                               // 取已使用的节点头 (遍历用)
    UInt32 const &  Count() const;                                      // 获取已使用的节点数
private:
    // 自带原始 Buffer (含 指向下一个Buffer的指针 以及 Dictionary_OriginalNodeLength个节点 以及 哈希节点链头对应数组 )
    Byte            _originalBuffer[sizeof(Byte*) + sizeof(Node) * Dictionary_OriginalNodeLength + sizeof(Node*) * Dictionary_OriginalNodeLength];
    Byte *          _buffer;                                            // 指向当前Buffer内存块链表头. 
    Node **         _hashNodeHeadMap;                                   // 指向 哈希节点链头对应数组 避免重复运算 (HeadBuffer + sizeof(Byte*) + sizeof(Node) * Dictionary_OriginalNodeLength)
    Node *          _freeNodeHead;                                      // 指向空闲节点链表头
    Node *          _usedNodeHead;                                      // 指向已使用节点链表头
    UInt32          _primeIndex;                                        // 保存长度质数位于质数表中的索引 以方便 resize 时快速取到下一个质数长度
    UInt32          _count;                                             // 已使用的节点个数
    UInt32          _length;                                            // 所有已创建的 Buffer 的总节点数 ( 质数. 使用 HashCode 按该长度取模来确定哈希节点对应关系 )

    void            Init();                                             // 初始化默认长度Buffer (供构造函数调用)
    void            Init(UInt32 const &len);                            // 初始化指定长度Buffer (供构造函数调用)
    // 取得新的内存尺寸, 请求1个内存块，链接，扩展 FreeNodeHead 链表，同步 Length, PrimeIndex, 重新计算 Hash映射关系 到新的 HashNodeHeadMap
    void            Resize();
    void            Resize(UInt32 const &len);                          // 这个给 Init(len) 和 operator=用
};



}
#endif