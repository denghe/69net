#ifndef _DICT_H_
#define _DICT_H_

template <typename TK, typename TV>
struct Dict
{
    struct Node
    {
        Node *          next;
        uint            hash;                               // key's hashCode
        int             index;                              // _nodes[ index ]
        TK const        key;
        TV              value;
    };
    typedef Node NT;
    explicit Dict( int capacity = 64 );
    ~Dict();
    template <typename KT, typename VT>
    Node* insert( KT && k, VT && v, bool replace = false ); // if exists, return Node*. else add and return nullptr
    Node* find( TK const& k );                              // if exists, return Node*. else return nullptr
    void erase( Node* n );
    void clear();
    List<Node*> const & data() const;
    int size() const;
private:
    void dispose( Node* n );
    void resize();
    List<Node*>         _buckets;
    List<Node*>         _nodes;
    Pool                _pool;
};

#include "Dict.hpp"

//Dict( Dict && o );
//Dict( Dict const & o ) = delete;    // todo
//Dict & operator=( Dict && o );
//Dict & operator=( Dict const & o ) = delete;    // todo

// todo: key 自动路由它的 getHashCode 函数来算 hash ( 如果类型带了这个函数的话 )

//
//class HashString
//{
//public:
//    template<typename T>
//    HashString( T&& s )
//    {
//        _s = std::forward<T>( s );
//        _h = _s.getHashCode();
//    }
//    HashString( HashString const& other )
//        : _s( other._s )
//        , _h( other._h )
//    {
//    }
//    HashString( HashString && other )
//        : _s( std::move( other._s ) )
//        , _h( other._h )
//    {
//    }
//    inline HashString & operator=( HashString && other )
//    {
//        _s = std::move( other._s );
//        _h = other._h;
//    }
//    inline HashString & operator=( HashString const & other )
//    {
//        _s = other._s;
//        _h = other._h;
//    }
//    inline bool operator==( HashString const& other )
//    {
//        return _h == other._h && _s == other._s;
//    }
//    inline operator String const&( )
//    {
//        return _s;
//    }
//private:
//    String _s;
//    int _h;
//};
//
//template <typename TV>
//struct Dict < String, TV >
//{
//    struct Node                                                         // 节点（直含键值数据）
//    {
//        Node *          next;                                          // 指向同桶下一个节点
//        int             index;                                         // 于 _nodes 的索引（移除的时候方便定位）
//        HashString      key;
//        TV              value;
//    };
//
//private:
//    List<Node*>         _nodes;                                         // 节点指针数组， 0 ~ _len-1 为已使用节点数， _len 到 _buckets._len 为已创建未使用区
//    List<Node*>         _buckets;                                       // 桶（头节点）数组， _len 为质数。 
//    Pool                _pool;
//};


//#include "Dict.hpp"



#endif
