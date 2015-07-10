#pragma once

namespace xxx
{
    template <typename TK, typename TV>
    struct Dict : Memmoveable
    {
        struct Node
        {
            Node*           next;
            uint            hash;                               // key's hashCode
            int             index;                              // nodes[ index ]
            TK              key;
            TV              value;
        };

        explicit Dict( int capacity = 64 );
        ~Dict();

        Dict( Dict&& o );
        Dict( Dict const& o );
        Dict& operator=( Dict&& o );
        Dict& operator=( Dict const& o );

        template<typename ...VPTS>
        std::pair<Node*, bool> Emplace( bool replace, TK const& k, VPTS&& ...vps );      // same as Insert but need value type's construct parameters
        template <typename VT>
        std::pair<Node*, bool> Insert( TK const& k, VT&& v, bool replace = false );      // if exists, return Node* + false. else add and return Node* + true

        Node const* Find( TK const& k ) const;                  // if exists, return Node*. else return nullptr
        Node* Find( TK const& k );
        TV& At( TK const& k );                                  // Find or Insert default value( if not found ) and return
        TV& operator[]( TK const& k );                          // return at( k )

        void Erase( TK const& k );
        void Erase( Node* n );

        void Reserve( int capacity );
        void Clear();
        bool Empty();
        int Size() const;

        List<Node*> const& Data() const;                        // ref to nodes
        List<Node*>& Data();
        Node const* IndexAt( int idx ) const;                   // return nodes[ idx ]
        Node* IndexAt( int idx );


        // ByteBuffer interface
        void WriteTo( ByteBuffer& bb ) const;
        void FastWriteTo( ByteBuffer& bb ) const;
        bool ReadFrom( ByteBuffer& bb );
    protected:
        void Dispose( Node* n );
        void Resize();
        List<Node*>         buckets;
        List<Node*>         nodes;
        Pool                pool;
    };
}
