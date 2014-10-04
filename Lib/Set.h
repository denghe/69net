#ifndef _SET_H_
#define _SET_H_

// 直接由 Dict 去 value 简化而来

template <typename TK>
struct Set : Memmoveable
{
    struct Node
    {
        Node*           next;
        uint            hash;                               // key's hashCode
        int             index;                              // _nodes[ index ]
        TK              key;
    };
    explicit Set( int capacity = 64 );
    Set( Set&& o );
    Set( Set const& o );
    Set& operator=( Set&& o );
    Set& operator=( Set const& o );
    ~Set();
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
