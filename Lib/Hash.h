#ifndef _HASH_H_
#define _HASH_H_

namespace xxx
{
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
        std::pair<Node*, bool> Insert( KT&& k );      // if exists, return Node* + false. else add and return Node* + true
        Node* Find( TK const& k );                                                  // if exists, return Node*. else return nullptr
        void Erase( TK const& k );
        void Erase( Node* n );
        void Clear();
        bool Empty();
        void Reserve( int capacity );
        List<Node*> const& Data() const;
        int Size() const;
        Node* operator[]( int idx ) const;


        // for FlatBuffer Write
        int GetWriteBufferSize() const;
        void WriteBuffer( FlatBuffer& fb ) const;
        void WriteBufferDirect( FlatBuffer& fb ) const;
        bool ReadBuffer( FlatBuffer& fb );
    protected:
        void Dispose( Node* n );
        void Resize();
        List<Node*>         _buckets;
        List<Node*>         _nodes;
        Pool                _pool;
    };

}

#endif
