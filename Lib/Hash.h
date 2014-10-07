#ifndef _SET_H_
#define _SET_H_

// 直接由 Dict 去 value 简化而来

template <typename TK>
struct Hash : Memmoveable
{
    struct Node
    {
        Node*           next;
        uint            hash;                               // key's hashCode
        int             index;                              // _nodes[ index ]
        TK              key;
    };
    explicit Hash( int capacity = 64 );
    Hash( Hash&& o );
    Hash( Hash const& o );
    Hash& operator=( Hash&& o );
    Hash& operator=( Hash const& o );
    ~Hash();
    template <typename KT>
    std::pair<Node*, bool> insert( KT&& k );      // if exists, return Node* + false. else add and return Node* + true
    Node* find( TK const& k );                                                  // if exists, return Node*. else return nullptr
    void erase( TK const& k );
    void erase( Node* n );
    void clear();
    bool empty();
    void reserve( int capacity );
    List<Node*> const& data() const;
    int size() const;
    Node* operator[]( int idx );
    

    // for FlatBuffer write
    int getWriteBufferSize() const;
    void writeBuffer( FlatBuffer& fb ) const;
    void writeBufferDirect( FlatBuffer& fb ) const;
    bool readBuffer( FlatBuffer& fb );
protected:
    void dispose( Node* n );
    void resize();
    List<Node*>         _buckets;
    List<Node*>         _nodes;
    Pool                _pool;
};

#endif
