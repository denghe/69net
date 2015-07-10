#pragma once

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
            int             index;                              // nodes[ index ]
            TK              key;
        };
        explicit Hash( int capacity = 64 );
        Hash( Hash&& o );
        Hash( Hash const& o );
        Hash& operator=( Hash&& o );
        Hash& operator=( Hash const& o );
        ~Hash();
        std::pair<Node*, bool> Insert( TK const& k );           // if exists, return Node* + false. else add and return Node* + true
        Node* Find( TK const& k );                              // if exists, return Node*. else return nullptr
        void Erase( TK const& k );
        void Erase( Node* n );
        void Clear();
        bool Empty();
        void Reserve( int capacity );
        int Size() const;
        List<Node*> const& Data() const;
        List<Node*> & Data();
        Node const* operator[]( int idx ) const;
        Node* operator[]( int idx );
        Node const* IndexAt( int idx ) const;
        Node* IndexAt( int idx );

    protected:
        void Dispose( Node* n );
        void Resize();
        List<Node*>         buckets;
        List<Node*>         nodes;
        Pool                pool;
    };

}

