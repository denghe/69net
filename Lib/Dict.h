#ifndef _DICT_H_
#define _DICT_H_

template <typename TK, typename TV>
struct Dict
{
    struct Node
    {
        Node*           next;
        uint            hash;                               // key's hashCode
        int             index;                              // _nodes[ index ]
        TK const        key;
        TV              value;
    };
    typedef Node NT;
    explicit Dict( int capacity = 64 );
    // todo:
    //Dict( Dict && o );
    //Dict( Dict const & o ) = delete;    // todo
    //Dict & operator=( Dict && o );
    //Dict & operator=( Dict const & o ) = delete;    // todo
    ~Dict();
    template <typename KT, typename VT>
    std::pair<Node*, bool> insert( KT && k, VT && v, bool replace = false );    // if exists, return Node* + false. else add and return Node* + true
    Node* find( TK const& k );                                                  // if exists, return Node*. else return nullptr
    void erase( TK const& k );
    void erase( Node* n );
    void clear();
    void reserve( int capacity );
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



#endif
