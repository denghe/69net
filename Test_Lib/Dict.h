#ifndef _DICT_H_
#define _DICT_H_

// 字典: 有 某质数 个桶（链表），根据 key 的 hashCode（取模）来判断放入哪个桶
// 要求 TK 类型实现复制构造, operator== 重载, 提供 GetHashCode 函数 (当前仅支持 Lib 内建类型), TV 类型实现复制构造, operator= 重载
template <typename TK, typename TV>
struct Dict
{
    struct Node                                                         // 节点（直含键值数据）
    {
        Node *          _next;                                          // 指向同桶下一个节点
        int             _index;                                         // 于 _nodes 的索引（移除的时候方便定位）
        uint            _hashCode;                                      // 根据 key 值算出来的 hash 值
        TK              _key;
        TV              _value;
    };

    Dict();
    ~Dict();
    explicit Dict( int capacity );  // todo: call reserve
    Dict( Dict && o );
    Dict( Dict const & o ) = delete;    // todo
    Dict & operator=( Dict && o );
    Dict & operator=( Dict const & o ) = delete;    // todo

    // todo: key 自动路由它的 getHashCode 函数来算 hash ( 如果类型带了这个函数的话 )
    template <typename TKey, typename TValue>
    bool add( TKey && k, TValue && v, bool replace = false );   // 添加 k & v 到字典。replace 为 true 时, k 值已存在则替换。返回 k 是否已存在
    void clear();                                               // 不释放内存块, 析构已有数据, 并将节点挪到 _freeNodeHead 链
    bool exists( TK const & k ) const;                          // 判断是否包含某 k
    bool tryGet( TK const & k, TV & v ) const;                  // 根据键来取值，返回该项是否存在( 存在则复制 value 到 v )
    bool tryRemove( TK const & k, TV & v );                     // 从字典中移除 k 项, 返回该项是否存在( 存在则移动 value 到 v )
    bool tryRemove( TK const & k );                             // 从字典中移除 k 项, 返回该项是否存在

    // todo: reserve( 质数 )
    // todo: size()
    // todo: data()
    
private:
    List<Node*>         _nodes;                                         // 节点指针数组， 0 ~ _len-1 为已使用节点数， _len 到 _buckets._len 为已创建未使用区
    List<Node*>         _buckets;                                       // 桶（头节点）数组， _len 为质数。 
    // todo: Pool
                                                                        // 因为 List 长度特性是 2^N，故可以用后面一定用不到的单元来放 当前是第几个质数
    int                 getPrimeIdx() const;                            // 从 _buckets 尾部取出当前质数 index
    void                setPrimeIdx( int idx );                         // 将当前质数 index 存到 _buckets 尾部

};

#include "Dict.hpp"

#endif
