#ifndef _HASH_HPP__
#define _HASH_HPP__

namespace xxx
{


    template <typename TK>
    Hash<TK>::Hash( int capacity /*= 64 */ )
    {
        _pool.Init( sizeof( Node ), 4096, capacity );
        int prime = GetPrime( capacity );
        _nodes.Reserve( prime );
        _buckets.Resize( prime );
    }

    template <typename TK>
    Hash<TK>::Hash( Hash const& o )
        : Hash( o.Size() )
    {
        operator=( o );
    }

    template <typename TK>
    Hash<TK>::Hash( Hash&& o )
    {
        _buckets = std::move( o._buckets );
        _nodes = std::move( o._nodes );
        _pool = std::move( o._pool );
    }

    template <typename TK>
    Hash<TK>& Hash<TK>::operator=( Hash const& o )
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
            _buckets[ mod ] = n;
            _nodes.Push( n );
        }
        return *this;
    }

    template <typename TK>
    Hash<TK>& Hash<TK>::operator=( Hash&& o )
    {
        _buckets = std::move( o._buckets );
        _nodes = std::move( o._nodes );
        _pool = std::move( o._pool );
        return *this;
    }

    template <typename TK>
    Hash<TK>::~Hash()
    {
        for( int i = 0; i < _nodes.Size(); ++i )
            Dispose( _nodes[ i ] );
    }

    template <typename TK>
    template <typename KT>
    std::pair<typename Hash<TK>::Node*, bool> Hash<TK>::Insert( KT && k )
    {
        std::pair<typename Hash<TK>::Node*, bool> rtv;
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)_buckets.Size();
        auto node = _buckets[ mod ];
        while( node )
        {
            if( node->hash == hashCode && EqualsTo( node->key, k ) )
            {
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
        _buckets[ mod ] = n;
        _nodes.Push( n );
        if( _nodes.Size() == _buckets.Size() ) Resize();
        rtv.first = n;
        rtv.second = true;
        return rtv;
    }


    template <typename TK>
    typename Hash<TK>::Node* Hash<TK>::Find( TK const& k )
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

    template <typename TK>
    void Hash<TK>::Erase( TK const& k )
    {
        auto n = Find( k );
        if( !n ) return;
        Erase( n );
    }

    template <typename TK>
    void Hash<TK>::Erase( Node* n )
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

    template <typename TK>
    void Hash<TK>::Clear()
    {
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            Dispose( _nodes[ i ] );
            _pool.Free( _nodes[ i ] );
        }
        _nodes.Clear();
        memset( _buckets.Data(), 0, _buckets.ByteSize() );
    }


    template <typename TK>
    void Hash<TK>::Reserve( int capacity )
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


    template <typename TK>
    List<typename Hash<TK>::Node*> const & Hash<TK>::Data() const
    {
        return _nodes;
    }

    template <typename TK>
    int Hash<TK>::Size() const
    {
        return _nodes.Size();
    }

    template <typename TK>
    void Hash<TK>::Dispose( Node* n )
    {
        n->key.~TK();
    }


    template <typename TK>
    bool Hash<TK>::Empty()
    {
        return _nodes.Size() == 0;
    }


    template <typename TK>
    void Hash<TK>::Resize()
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

    template <typename TK>
    typename Hash<TK>::Node* Hash<TK>::operator[]( int idx ) const
    {
        assert( idx < _nodes.Size() );
        return _nodes[ idx ];
    }








    template <typename TK>
    int Hash<TK>::GetWriteBufferSize() const
    {
        int siz = sizeof( int );
        if( std::is_pod<TK>::value )
        {
            siz += sizeof( TK ) * _nodes.Size();
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
    template <typename TK>
    void Hash<TK>::WriteBuffer( FlatBuffer& fb ) const
    {
        fb.Write( _nodes.Size() );
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            fb.Write( _nodes[ i ]->key );
        }
    }
    template <typename TK>
    void Hash<TK>::WriteBufferDirect( FlatBuffer& fb ) const
    {
        fb.WriteDirect( _nodes.Size() );
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            fb.WriteDirect( _nodes[ i ]->key );
        }
    }

    template <typename TK>
    bool Hash<TK>::ReadBuffer( FlatBuffer& fb )
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
