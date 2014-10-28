#ifndef _DICT_H_
#define _DICT_H_


namespace xxx
{

    // todo: 如果 TK 是简单数据类型（ 可直接拿来做 hash 的 ），路由到无 hash 值版

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
        Dict( Dict&& o );
        Dict( Dict const& o );
        Dict& operator=( Dict&& o );
        Dict& operator=( Dict const& o );
        ~Dict();
        template <typename VT>
        std::pair<Node*, bool> Insert( TK const& k, VT&& v, bool replace = false );      // if exists, return Node* + false. else add and return Node* + true
        template<typename ...VPTS>
        std::pair<Node*, bool> Emplace( bool replace, TK const& k, VPTS&& ...vps );      // same as Insert but need value type's construct parameters
        Node* Find( TK const& k );                                                  // if exists, return Node*. else return nullptr
        void Erase( TK const& k );
        void Erase( Node* n );
        void Clear();
        bool Empty();
        void Reserve( int capacity );
        List<Node*> const& Data() const;
        int Size() const;
        TV& operator[]( TK const& k );                   // Find or Insert ( default value ) and return
        TV& At( TK const& k );                           // same as operator[]



        // for FlatBuffer Write
        int GetWriteBufferSize() const;
        void WriteBuffer( FlatBuffer& fb ) const;
        void WriteBufferDirect( FlatBuffer& fb ) const;
        bool ReadBuffer( FlatBuffer& fb );
    protected:
        void Dispose( Node* n );
        void Resize();
        List<Node*>         buckets;
        List<Node*>         nodes;
        Pool                pool;
    };



}

#endif
