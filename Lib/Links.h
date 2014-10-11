#ifndef _LINKS_H_
#define _LINKS_H_

// 直接由 Hash 去 buckets 简化而来
// 特点是 insert 返回的 Node* 值不会变化，对象可存储该值以实现高速 erase

template <typename T>
struct Links : Memmoveable
{
    struct Node
    {
        T               value;
        int             index;                              // _nodes[ index ]
    };
    explicit Links( int capacity = 64 );
    Links( Links&& o );
    Links( Links const& o );
    Links& operator=( Links&& o );
    Links& operator=( Links const& o );
    ~Links();
    template <typename VT>
    Node* insert( VT&& v );
    Node* find( T const& v );                              // foreach scan. if exists, return Node*. else return nullptr
    void erase( T const& v );
    void erase( Node* n );
    void clear();
    bool empty();
    void reserve( int capacity );
    List<Node*> const& data() const;
    int size() const;
    Node* operator[]( int idx ) const;
    

    // for FlatBuffer write
    int getWriteBufferSize() const;
    void writeBuffer( FlatBuffer& fb ) const;
    void writeBufferDirect( FlatBuffer& fb ) const;
    bool readBuffer( FlatBuffer& fb );
protected:
    void dispose( Node* n );
    void resize();
    List<Node*>         _nodes;
    Pool                _pool;
};

#endif
