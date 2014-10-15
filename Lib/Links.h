#ifndef _LINKS_H_
#define _LINKS_H_

namespace xxx
{


    // 直接由 Hash 去 buckets 简化而来
    // 特点是 Insert 返回的 Node* 值不会变化，对象可存储该值以实现高速 Erase

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
        Node* Insert( VT&& v );
        Node* Find( T const& v );                              // foreach scan. if exists, return Node*. else return nullptr
        void Erase( T const& v );
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
        List<Node*>         _nodes;
        Pool                _pool;
    };

}

#endif
