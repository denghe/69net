﻿#ifndef _HASH_H_
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
        List<Node*> const& Data() const;
        int Size() const;
        Node* operator[]( int idx ) const;

    protected:
        void Dispose( Node* n );
        void Resize();
        List<Node*>         buckets;
        List<Node*>         nodes;
        Pool                pool;
    };

}

#endif
