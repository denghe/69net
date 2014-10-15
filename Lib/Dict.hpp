#ifndef _DICT_HPP__
#define _DICT_HPP__

namespace xxx
{


    template <typename TK, typename TV>
    Dict<TK, TV>::Dict( int capacity /*= 64 */ )
    {
        _pool.Init( sizeof( Node ), 4096, capacity );
        int prime = GetPrime( capacity );
        _nodes.Reserve( prime );
        _buckets.Resize( prime );
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
        _buckets = std::move( o._buckets );
        _nodes = std::move( o._nodes );
        _pool = std::move( o._pool );
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
            uint mod = on->hash % (uint)_buckets.Size();
            auto n = (Node*)_pool.Alloc();
            n->next = _buckets[ mod ];
            n->hash = on->hash;
            n->index = _nodes.Size();
            new ( &n->key ) TK( on->key );
            new ( &n->value ) TV( on->value );
            _buckets[ mod ] = n;
            _nodes.Push( n );
        }
        return *this;
    }

    template <typename TK, typename TV>
    Dict<TK, TV>& Dict<TK, TV>::operator=( Dict&& o )
    {
        _buckets = std::move( o._buckets );
        _nodes = std::move( o._nodes );
        _pool = std::move( o._pool );
        return *this;
    }

    template <typename TK, typename TV>
    Dict<TK, TV>::~Dict()
    {
        for( int i = 0; i < _nodes.Size(); ++i )
            Dispose( _nodes[ i ] );
    }

    template <typename TK, typename TV>
    template <typename KT, typename VT>
    std::pair<typename Dict<TK, TV>::Node*, bool> Dict<TK, TV>::Insert( KT && k, VT && v, bool replace /*= false */ )
    {
        return Emplace( replace, std::forward<KT>( k ), std::forward<VT>( v ) );
    }

    template <typename TK, typename TV>
    template<typename KT, typename ...VPTS>
    std::pair<typename Dict<TK, TV>::Node*, bool> Dict<TK, TV>::Emplace( bool replace, KT&& k, VPTS&& ...vps )
    {
        std::pair<typename Dict<TK, TV>::Node*, bool> rtv;
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)_buckets.Size();
        auto node = _buckets[ mod ];
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
        auto n = (Node*)_pool.Alloc();
        n->next = _buckets[ mod ];
        n->hash = hashCode;
        n->index = _nodes.Size();
        new ( &n->key ) TK( std::forward<KT>( k ) );
        new ( &n->value ) TV( std::forward<VPTS>( vps )... );
        _buckets[ mod ] = n;
        _nodes.Push( n );
        if( _nodes.Size() == _buckets.Size() ) Resize();
        rtv.first = n;
        rtv.second = true;
        return rtv;
    }


    template <typename TK, typename TV>
    typename Dict<TK, TV>::Node* Dict<TK, TV>::Find( TK const& k )
    {
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)_buckets.Size();
        auto node = _buckets[ mod ];
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
        auto mod = n->hash % (uint)_buckets.Size();
        auto node = _buckets[ mod ];
        if( node == n )
        {
            _buckets[ mod ] = node->next;
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

        auto last = _nodes.Top();
        _nodes.Pop();
        _nodes[ n->index ] = last;
        last->index = n->index;

        Dispose( n );
        _pool.Free( n );
    }

    template <typename TK, typename TV>
    template <typename KT>
    TV& Dict<TK, TV>::operator[]( KT&& k )
    {
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)_buckets.Size();
        auto node = _buckets[ mod ];
        while( node )
        {
            if( node->hash == hashCode && EqualsTo( node->key, k ) )
            {
                return node->value;
            }
            node = node->next;
        };
        auto n = (Node*)_pool.Alloc();                          // new & Init
        n->next = _buckets[ mod ];
        n->hash = hashCode;
        n->index = _nodes.Size();
        new ( &n->key ) TK( std::forward<KT>( k ) );
        new ( &n->value ) TV();
        _buckets[ mod ] = n;
        _nodes.Push( n );
        if( _nodes.Size() == _buckets.Size() ) Resize();        // grow
        return n->value;
    }

    template <typename TK, typename TV>
    template <typename KT>
    TV& Dict<TK, TV>::At( KT&& k )
    {
        return operator[]( std::forward<KT>( k ) );
    }

    template <typename TK, typename TV>
    void Dict<TK, TV>::Clear()
    {
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            Dispose( _nodes[ i ] );
            _pool.Free( _nodes[ i ] );
        }
        _nodes.Clear();
        memset( _buckets.Data(), 0, _buckets.ByteSize() );
    }


    template <typename TK, typename TV>
    void Dict<TK, TV>::Reserve( int capacity )
    {
        if( capacity <= _buckets.Size() ) return;
        int prime = GetPrime( (int)Round2n( capacity ) );
        _nodes.Reserve( prime );
        _buckets.Resize( prime, false );
        memset( _buckets.Data(), 0, _buckets.ByteSize() );      // clean up
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            auto& o = _nodes[ i ];
            auto mod = o->hash % prime;
            o->next = _buckets[ mod ];
            _buckets[ mod ] = o;
        }
    }


    template <typename TK, typename TV>
    List<typename Dict<TK, TV>::Node*> const & Dict<TK, TV>::Data() const
    {
        return _nodes;
    }

    template <typename TK, typename TV>
    int Dict<TK, TV>::Size() const
    {
        return _nodes.Size();
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
        return _nodes.Size() == 0;
    }


    template <typename TK, typename TV>
    void Dict<TK, TV>::Resize()
    {
        int prime = GetPrime( _nodes.Size() * 3 );
        _nodes.Reserve( prime );
        _buckets.Resize( prime, false );
        memset( _buckets.Data(), 0, _buckets.ByteSize() );      // clean up
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            auto& o = _nodes[ i ];
            auto mod = o->hash % prime;
            o->next = _buckets[ mod ];
            _buckets[ mod ] = o;
        }
    }










    template <typename TK, typename TV>
    int Dict<TK, TV>::GetWriteBufferSize() const
    {
        int siz = sizeof( int );
        if( !std::is_pod<TK>::value && !std::is_pod<TV>::value )
        {
            for( int i = 0; i < _nodes.Size(); ++i )
            {
                siz += _nodes[ i ]->key.GetWriteBufferSize();
                siz += _nodes[ i ]->value.GetWriteBufferSize();
            }
            return siz;
        }
        if( std::is_pod<TK>::value )
        {
            siz += sizeof( TK ) * _nodes.Size();
        }
        else
        {
            for( int i = 0; i < _nodes.Size(); ++i )
            {
                siz += _nodes[ i ]->value.GetWriteBufferSize();
            }
        }
        if( std::is_pod<TV>::value )
        {
            siz += sizeof( TV ) * _nodes.Size();
        }
        else
        {
            for( int i = 0; i < _nodes.Size(); ++i )
            {
                siz += _nodes[ i ]->key.GetWriteBufferSize();
            }
        }
        return siz;
    }
    template <typename TK, typename TV>
    void Dict<TK, TV>::WriteBuffer( FlatBuffer& fb ) const
    {
        fb.Write( _nodes.Size() );
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            fb.Write( _nodes[ i ]->key );
            fb.Write( _nodes[ i ]->value );
        }
    }
    template <typename TK, typename TV>
    void Dict<TK, TV>::WriteBufferDirect( FlatBuffer& fb ) const
    {
        fb.WriteDirect( _nodes.Size() );
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            fb.WriteDirect( _nodes[ i ]->key );
            fb.WriteDirect( _nodes[ i ]->value );
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
            auto n = (Node*)_pool.Alloc();                              // malloc
            if( !std::is_pod<TK>::value ) new ( &n->key ) TK();         // new key
            if( !fb.Read( n->key ) )
            {
                if( !std::is_pod<TK>::value ) n->key.~TK();             // delete key
                _pool.Free( n );                                        // free
                return false;
            }
            if( !std::is_pod<TV>::value ) new ( &n->value ) TV();       // new value
            if( !fb.Read( n->value ) )
            {
                if( !std::is_pod<TV>::value ) n->value.~TV();           // delete value
                _pool.Free( n );                                        // free
                return false;
            }
            n->hash = (uint)GetHashCode( n->key );
            uint mod = n->hash % (uint)_buckets.Size();
            n->index = i;//_nodes.Size();
            n->next = _buckets[ mod ];
            _buckets[ mod ] = n;
            _nodes.Push( n );
        }
        return true;
    }


}


#endif
