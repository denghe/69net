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
        TK              _key;
        TV              _value;
    };
    Dict() : Pool( sizeof( TK ) )
    {
        int prime = Utils::getPrime( _pool.size() );
        _nodes.reserve( prime, false );                     // 确保空间足够 不清 0
        memset( _nodes.data(), 0, _nodes.byteSize() );      // _size 后面的单元要用来存 已分配但未使用的指针，所以也要清 0
        _buckets.resize( prime, false );                    // 将桶的空间划出来 不清 0
        memset( _buckets.data(), 0, _buckets.byteSize() );  // 要用于桶内有没有数据的判断条件，0: 空指针 就是没有
    }

private:
    List<Node*>         _nodes;                                         // 节点指针数组， 0 ~ _len-1 为已使用节点数， _len 到 _buckets._len 为已创建未使用区
    List<Node*>         _buckets;                                       // 桶（头节点）数组， _len 为质数。 
    Pool                _pool;
};


//~Dict();
//explicit Dict( int capacity );  // todo: call reserve
//Dict( Dict && o );
//Dict( Dict const & o ) = delete;    // todo
//Dict & operator=( Dict && o );
//Dict & operator=( Dict const & o ) = delete;    // todo

//// todo: key 自动路由它的 getHashCode 函数来算 hash ( 如果类型带了这个函数的话 )
//template <typename TKey, typename TValue>
//bool add( TKey && k, TValue && v, bool replace = false );           // 添加 k & v 到字典。replace 为 true 时, k 值已存在则替换。返回 k 是否已存在
//void clear();                                                       // 不释放内存块, 析构已有数据, 并将节点挪到 _freeNodeHead 链
//bool exists( TK const & k ) const;                                  // 判断是否包含某 k
//bool tryGet( TK const & k, TV & v ) const;                          // 根据键来取值，返回该项是否存在( 存在则复制 value 到 v )
//bool tryRemove( TK const & k, TV & v );                             // 从字典中移除 k 项, 返回该项是否存在( 存在则移动 value 到 v )
//bool tryRemove( TK const & k );                                     // 从字典中移除 k 项, 返回该项是否存在

// todo:
// operator[]
// void tryRemove( Node* n );
// Node* find( TK const & k ) const;                                   // 查找并返回节点。找不到返回 nullptr
// reserve( 质数 )
// size()
// data()


class HashString
{
public:
    template<typename T>
    HashString( T&& s )
    {
        _s = std::forward<T>( s );
        _h = _s.getHashCode();
    }
    HashString( HashString const& other )
        : _s( other._s )
        , _h( other._h )
    {
    }
    HashString( HashString && other )
        : _s( std::move( other._s ) )
        , _h( other._h )
    {
    }
    inline HashString & operator=( HashString && other )
    {
        _s = std::move( other._s );
        _h = other._h;
    }
    inline HashString & operator=( HashString const & other )
    {
        _s = other._s;
        _h = other._h;
    }
    inline bool operator==( HashString const& other )
    {
        return _h == other._h && _s == other._s;
    }
    inline operator String const&( )
    {
        return _s;
    }
private:
    String _s;
    int _h;
};

template <typename TV>
struct Dict<String, TV>
{
    struct Node                                                         // 节点（直含键值数据）
    {
        Node *          _next;                                          // 指向同桶下一个节点
        int             _index;                                         // 于 _nodes 的索引（移除的时候方便定位）
        HashString      _key;
        TV              _value;
    };

private:
    List<Node*>         _nodes;                                         // 节点指针数组， 0 ~ _len-1 为已使用节点数， _len 到 _buckets._len 为已创建未使用区
    List<Node*>         _buckets;                                       // 桶（头节点）数组， _len 为质数。 
    Pool                _pool;
};






//#include "Dict.hpp"



#endif
