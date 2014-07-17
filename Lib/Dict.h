#ifndef _DICT_H_
#define _DICT_H_

template <typename TK, typename TV>
struct Dict : Memmoveable
{
    struct Node
    {
        Node*           next;
        uint            hash;                               // key's hashCode
        int             index;                              // _nodes[ index ]
        TK              key;
        TV              value;
    };
    typedef Node NT;
    explicit Dict( int capacity = 64 );
    Dict( Dict&& o );
    Dict( Dict const& o );
    Dict& operator=( Dict&& o );
    Dict& operator=( Dict const& o );
    ~Dict();
    template <typename KT, typename VT>
    std::pair<Node*, bool> insert( KT&& k, VT&& v, bool replace = false );      // if exists, return Node* + false. else add and return Node* + true
    template<typename KT, typename ...VPTS>
    std::pair<Node*, bool> emplace( bool replace, KT&& k, VPTS&& ...vps );      // same as insert but need value type's construct parameters
    Node* find( TK const& k );                                                  // if exists, return Node*. else return nullptr
    void erase( TK const& k );
    void erase( Node* n );
    void clear();
    void reserve( int capacity );
    List<Node*> const& data() const;
    int size() const;
    template <typename KT>
    TV& operator[]( KT&& k );                   // find or insert ( default value ) and return
    template <typename KT>
    TV& at( KT&& k );                           // same as operator[]

    

    // for FlatBuffer write
    int getBufferSize() const;
    void writeBuffer( FlatBuffer& fb ) const;
    void writeBufferDirect( FlatBuffer& fb ) const;
    bool readBuffer( FlatBuffer& fb );
private:
    void dispose( Node* n );
    void resize();
    List<Node*>         _buckets;
    List<Node*>         _nodes;
    Pool                _pool;
};

#endif
