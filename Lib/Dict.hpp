#ifndef _DICT_HPP__
#define _DICT_HPP__

namespace xxx
{


    template <typename TK, typename TV>
    Dict<TK, TV>::Dict( int capacity /*= 64 */ )
    {
        pool.Init( sizeof( Node ), 4096, capacity );
        int prime = GetPrime( capacity );
        nodes.Reserve( prime );
        buckets.Resize( prime );
    }

    template <typename TK, typename TV>
    Dict<TK, TV>::Dict( Dict const& o )
        : Dict( o.Size() )
    {
        operator=( o );
    }

    template <typename TK, typename TV>
    Dict<TK, TV>::Dict( Dict&& o )
    {
        buckets = std::move( o.buckets );
        nodes = std::move( o.nodes );
        pool = std::move( o.pool );
    }

    template <typename TK, typename TV>
    Dict<TK, TV>& Dict<TK, TV>::operator=( Dict const& o )
    {
        if( this == &o ) return *this;
        Clear();
        Reserve( o.Size() );
        for( int i = 0; i < o.Size(); ++i )
        {
            auto on = o.Data()[ i ];
            uint mod = on->hash % (uint)buckets.Size();
            auto n = (Node*)pool.Alloc();
            n->next = buckets[ mod ];
            n->hash = on->hash;
            n->index = nodes.Size();
            new ( &n->key ) TK( on->key );
            new ( &n->value ) TV( on->value );
            buckets[ mod ] = n;
            nodes.Push( n );
        }
        return *this;
    }

    template <typename TK, typename TV>
    Dict<TK, TV>& Dict<TK, TV>::operator=( Dict&& o )
    {
        buckets = std::move( o.buckets );
        nodes = std::move( o.nodes );
        pool = std::move( o.pool );
        return *this;
    }

    template <typename TK, typename TV>
    Dict<TK, TV>::~Dict()
    {
        for( int i = 0; i < nodes.Size(); ++i )
            Dispose( nodes[ i ] );
    }

    template <typename TK, typename TV>
    template <typename VT>
    std::pair<typename Dict<TK, TV>::Node*, bool> Dict<TK, TV>::Insert( TK const& k, VT && v, bool replace /*= false */ )
    {
        return Emplace( replace, k, std::forward<VT>( v ) );
    }

    template <typename TK, typename TV>
    template<typename ...VPTS>
    std::pair<typename Dict<TK, TV>::Node*, bool> Dict<TK, TV>::Emplace( bool replace, TK const& k, VPTS&& ...vps )
    {
        std::pair<typename Dict<TK, TV>::Node*, bool> rtv;
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)buckets.Size();
        auto node = buckets[ mod ];
        while( node )
        {
            if( node->hash == hashCode && EqualsTo( node->key, k ) )
            {
                if( replace ) node->value = TV( std::forward<VPTS>( vps )... );
                rtv.first = node;
                rtv.second = false;
                return rtv;
            }
            node = node->next;
        };
        auto n = (Node*)pool.Alloc();
        n->next = buckets[ mod ];
        n->hash = hashCode;
        n->index = nodes.Size();
        new ( &n->key ) TK( k );
        new ( &n->value ) TV( std::forward<VPTS>( vps )... );
        buckets[ mod ] = n;
        nodes.Push( n );
        if( nodes.Size() == buckets.Size() ) Resize();
        rtv.first = n;
        rtv.second = true;
        return rtv;
    }


    template <typename TK, typename TV>
    typename Dict<TK, TV>::Node* Dict<TK, TV>::Find( TK const& k )
    {
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)buckets.Size();
        auto node = buckets[ mod ];
        while( node )
        {
            if( node->hash == hashCode && EqualsTo( node->key, k ) ) return node;
            node = node->next;
        }
        return nullptr;
    }

    template <typename TK, typename TV>
    void Dict<TK, TV>::Erase( TK const& k )
    {
        auto n = Find( k );
        if( !n ) return;
        Erase( n );
    }

    template <typename TK, typename TV>
    void Dict<TK, TV>::Erase( Node* n )
    {
        auto mod = n->hash % (uint)buckets.Size();
        auto node = buckets[ mod ];
        if( node == n )
        {
            buckets[ mod ] = node->next;
        }
        else
        {
            auto parent = node;
            while( node = node->next )
            {
                if( node == n )
                {
                    parent->next = node->next;
                    break;
                }
                parent = node;
            }
        }

        auto last = nodes.Top();
        nodes.Pop();
        nodes[ n->index ] = last;
        last->index = n->index;

        Dispose( n );
        pool.Free( n );
    }

    template <typename TK, typename TV>
    TV& Dict<TK, TV>::operator[]( TK const& k )
    {
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)buckets.Size();
        auto node = buckets[ mod ];
        while( node )
        {
            if( node->hash == hashCode && EqualsTo( node->key, k ) )
            {
                return node->value;
            }
            node = node->next;
        };
        auto n = (Node*)pool.Alloc();                          // new & Init
        n->next = buckets[ mod ];
        n->hash = hashCode;
        n->index = nodes.Size();
        new ( &n->key ) TK( k );
        new ( &n->value ) TV();
        buckets[ mod ] = n;
        nodes.Push( n );
        if( nodes.Size() == buckets.Size() ) Resize();        // grow
        return n->value;
    }

    template <typename TK, typename TV>
    TV& Dict<TK, TV>::At( TK const& k )
    {
        return operator[]( k );
    }

    template <typename TK, typename TV>
    void Dict<TK, TV>::Clear()
    {
        for( int i = 0; i < nodes.Size(); ++i )
        {
            Dispose( nodes[ i ] );
            pool.Free( nodes[ i ] );
        }
        nodes.Clear();
        memset( buckets.Data(), 0, buckets.ByteSize() );
    }


    template <typename TK, typename TV>
    void Dict<TK, TV>::Reserve( int capacity )
    {
        if( capacity <= buckets.Size() ) return;
        int prime = GetPrime( (int)Round2n( capacity ) );
        nodes.Reserve( prime );
        buckets.Resize( prime, false );
        memset( buckets.Data(), 0, buckets.ByteSize() );      // clean up
        for( int i = 0; i < nodes.Size(); ++i )
        {
            auto& o = nodes[ i ];
            auto mod = o->hash % prime;
            o->next = buckets[ mod ];
            buckets[ mod ] = o;
        }
    }


    template <typename TK, typename TV>
    List<typename Dict<TK, TV>::Node*> const & Dict<TK, TV>::Data() const
    {
        return nodes;
    }

    template <typename TK, typename TV>
    int Dict<TK, TV>::Size() const
    {
        return nodes.Size();
    }

    template <typename TK, typename TV>
    void Dict<TK, TV>::Dispose( Node* n )
    {
        n->key.~TK();
        n->value.~TV();
    }


    template <typename TK, typename TV>
    bool Dict<TK, TV>::Empty()
    {
        return nodes.Size() == 0;
    }


    template <typename TK, typename TV>
    void Dict<TK, TV>::Resize()
    {
        int prime = GetPrime( nodes.Size() * 3 );
        nodes.Reserve( prime );
        buckets.Resize( prime, false );
        memset( buckets.Data(), 0, buckets.ByteSize() );      // clean up
        for( int i = 0; i < nodes.Size(); ++i )
        {
            auto& o = nodes[ i ];
            auto mod = o->hash % prime;
            o->next = buckets[ mod ];
            buckets[ mod ] = o;
        }
    }










    template <typename TK, typename TV>
    int Dict<TK, TV>::GetWriteBufferSize() const
    {
        int siz = sizeof( int );
        if( !std::is_pod<TK>::value && !std::is_pod<TV>::value )
        {
            for( int i = 0; i < nodes.Size(); ++i )
            {
                siz += nodes[ i ]->key.GetWriteBufferSize();
                siz += nodes[ i ]->value.GetWriteBufferSize();
            }
            return siz;
        }
        if( std::is_pod<TK>::value )
        {
            siz += sizeof( TK ) * nodes.Size();
        }
        else
        {
            for( int i = 0; i < nodes.Size(); ++i )
            {
                siz += nodes[ i ]->value.GetWriteBufferSize();
            }
        }
        if( std::is_pod<TV>::value )
        {
            siz += sizeof( TV ) * nodes.Size();
        }
        else
        {
            for( int i = 0; i < nodes.Size(); ++i )
            {
                siz += nodes[ i ]->key.GetWriteBufferSize();
            }
        }
        return siz;
    }
    template <typename TK, typename TV>
    void Dict<TK, TV>::WriteBuffer( FlatBuffer& fb ) const
    {
        fb.Write( nodes.Size() );
        for( int i = 0; i < nodes.Size(); ++i )
        {
            fb.Write( nodes[ i ]->key );
            fb.Write( nodes[ i ]->value );
        }
    }
    template <typename TK, typename TV>
    void Dict<TK, TV>::WriteBufferDirect( FlatBuffer& fb ) const
    {
        fb.WriteDirect( nodes.Size() );
        for( int i = 0; i < nodes.Size(); ++i )
        {
            fb.WriteDirect( nodes[ i ]->key );
            fb.WriteDirect( nodes[ i ]->value );
        }
    }

    template <typename TK, typename TV>
    bool Dict<TK, TV>::ReadBuffer( FlatBuffer& fb )
    {
        int len;
        if( !fb.Read( len ) || len < 0 ) return false;
        Clear();
        Reserve( len );
        for( int i = 0; i < len; ++i )
        {
            auto n = (Node*)pool.Alloc();                              // malloc
            if( !std::is_pod<TK>::value ) new ( &n->key ) TK();         // new key
            if( !fb.Read( n->key ) )
            {
                if( !std::is_pod<TK>::value ) n->key.~TK();             // delete key
                pool.Free( n );                                        // free
                return false;
            }
            if( !std::is_pod<TV>::value ) new ( &n->value ) TV();       // new value
            if( !fb.Read( n->value ) )
            {
                if( !std::is_pod<TV>::value ) n->value.~TV();           // delete value
                pool.Free( n );                                        // free
                return false;
            }
            n->hash = (uint)GetHashCode( n->key );
            uint mod = n->hash % (uint)buckets.Size();
            n->index = i;//nodes.Size();
            n->next = buckets[ mod ];
            buckets[ mod ] = n;
            nodes.Push( n );
        }
        return true;
    }


}


#endif
