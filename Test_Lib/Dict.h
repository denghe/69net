#ifndef _DICT_H_
#define _DICT_H_
#include "Lib/All.h"

template <typename TK, typename TV>
struct Dict
{
    struct Node
    {
        Node *          next;
        uint            hash;                               // key's hashCode
        int             index;                              // _nodes[ index ]
        TK              key;
        TV              value;
    };
    Dict( int capacity = 64 )
    {
        _pool.init( sizeof( Node ), sizeof( Node ) * capacity, 2 );
        int prime = Utils::getPrime( _pool.size() );
        _nodes.reserve( prime );
        _buckets.resize( prime );
    }
    ~Dict()
    {
        for( int i = 0; i < _nodes.size(); ++i )
            dispose( _nodes[ i ] );
    }

    // if exists, return Node*. else add and return nullptr
    template <typename KT, typename VT>
    Node* insert( KT && k, VT && v, bool replace = false )
    {
        uint hashCode = (uint)Utils::getHashCode( k );          // calc
        uint mod = hashCode % (uint)_buckets.size();            // find
        auto node = _buckets[ mod ];                            // get chain header
        while( node )
        {
            if( node->hash == hashCode && node->key == k )      // scan chain
            {
                if( replace ) node->value = std::forward<VT>( v );
                return node;
            }
            node = node->next;
        };
        auto n = (Node*)_pool.alloc();                          // new & init
        n->next = _buckets[ mod ];
        n->hash = hashCode;
        n->index = _nodes.size();
        new ( &n->key ) TK( std::forward<KT>( k ) );
        new ( &n->value ) TV( std::forward<VT>( v ) );
        _buckets[ mod ] = n;
        _nodes.push( n );
        if( _nodes.size() == _buckets.size() ) resize();        // grow
        return nullptr;
    }

    // if exists, return Node*. else return nullptr
    Node* find( TK const& k )
    {
        uint hashCode = (uint)Utils::getHashCode( k );
        uint mod = hashCode % (uint)_buckets.size();
        auto node = _buckets[ mod ];
        while( node )
        {
            if( node->hash == hashCode && node->key == k ) return node;
            node = node->next;
        }
        return nullptr;
    }

    void erase( Node* n )
    {
        auto mod = n->hash % (uint)_buckets.size();
        Node* parent = nullptr;
        auto node = _buckets[ mod ];
        do
        {
            if( node == n )
            {
                if( parent )
                {
                    parent->next = n->next;
                    break;
                }
            }
            parent = node;
            node = node->next;
        } while( node );

        auto last = _nodes.top();
        _nodes.pop();
        _nodes[ n->index ] = last;
        last->index = n->index;

        dispose( n );
        _pool.free( n );
    }

    void clear()
    {
        for( int i = 0; i < _nodes; ++i )
        {
            dispose( _nodes[ i ] );
            _pool.free( _nodes[ i ] );
        }
        memset( _buckets.data(), 0, _buckets.byteSize() );
    }

    List<Node*> const & data() const
    {
        return _nodes;
    }
    int size() const
    {
        return _nodes.size();
    }
private:
    void dispose( Node* n )
    {
        n->key.~TK();
        n->value.~TV();
    }
    void resize()
    {
        int prime = Utils::getPrime( _nodes.size() * 3 );
        _nodes.reserve( prime );
        _buckets.resize( prime, false );
        memset( _buckets.data(), 0, _buckets.byteSize() );      // clean up
        for( int i = 0; i < _nodes.size(); ++i )
        {
            auto& o = _nodes[ i ];
            auto mod = o->hash % prime;
            o->next = _buckets[ mod ];
            _buckets[ mod ] = o;
        }
    }
    List<Node*>         _buckets;
    List<Node*>         _nodes;
    Pool                _pool;
};


//~Dict();
//explicit Dict( int capacity );  // todo: call reserve
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
