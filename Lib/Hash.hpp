#ifndef _HASH_HPP__
#define _HASH_HPP__

namespace xxx
{


    template <typename TK>
    Hash<TK>::Hash( int capacity /*= 64 */ )
    {
        pool.Init( sizeof( Node ), 4096, capacity );
        int prime = GetPrime( capacity );
        nodes.Reserve( prime );
        buckets.Resize( prime );
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
        buckets = std::move( o.buckets );
        nodes = std::move( o.nodes );
        pool = std::move( o.pool );
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
            uint mod = on->hash % (uint)buckets.Size();
            auto n = (Node*)pool.Alloc();
            n->next = buckets[ mod ];
            n->hash = on->hash;
            n->index = nodes.Size();
            new ( &n->key ) TK( on->key );
            buckets[ mod ] = n;
            nodes.Push( n );
        }
        return *this;
    }

    template <typename TK>
    Hash<TK>& Hash<TK>::operator=( Hash&& o )
    {
        buckets = std::move( o.buckets );
        nodes = std::move( o.nodes );
        pool = std::move( o.pool );
        return *this;
    }

    template <typename TK>
    Hash<TK>::~Hash()
    {
        for( int i = 0; i < nodes.Size(); ++i )
            Dispose( nodes[ i ] );
    }

    template <typename TK>
    std::pair<typename Hash<TK>::Node*, bool> Hash<TK>::Insert( TK const& k )
    {
        std::pair<typename Hash<TK>::Node*, bool> rtv;
        uint hashCode = (uint)GetHashCode( k );
        uint mod = hashCode % (uint)buckets.Size();
        auto node = buckets[ mod ];
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
        auto n = (Node*)pool.Alloc();
        n->next = buckets[ mod ];
        n->hash = hashCode;
        n->index = nodes.Size();
        new ( &n->key ) TK( k );
        buckets[ mod ] = n;
        nodes.Push( n );
        if( nodes.Size() == buckets.Size() ) Resize();
        rtv.first = n;
        rtv.second = true;
        return rtv;
    }


    template <typename TK>
    typename Hash<TK>::Node* Hash<TK>::Find( TK const& k )
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
        auto mod = n->hash % (uint)buckets.Size();
        auto node = buckets[ mod ];
        if( node == n )
        {
            buckets[ mod ] = node->next;
        }
        else
        {
            auto parent = node;
            while( (node = node->next) )
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
        if( n != last )
        {
            nodes[ n->index ] = last;
            last->index = n->index;
        }

        Dispose( n );
        pool.Free( n );
    }

    template <typename TK>
    void Hash<TK>::Clear()
    {
        for( int i = 0; i < nodes.Size(); ++i )
        {
            Dispose( nodes[ i ] );
            pool.Free( nodes[ i ] );
        }
        nodes.Clear();
        memset( buckets.Data(), 0, buckets.ByteSize() );
    }


    template <typename TK>
    void Hash<TK>::Reserve( int capacity )
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


    template <typename TK>
    List<typename Hash<TK>::Node*> const & Hash<TK>::Data() const
    {
        return nodes;
    }

    template <typename TK>
    int Hash<TK>::Size() const
    {
        return nodes.Size();
    }

    template <typename TK>
    void Hash<TK>::Dispose( Node* n )
    {
        n->key.~TK();
    }


    template <typename TK>
    bool Hash<TK>::Empty()
    {
        return nodes.Size() == 0;
    }


    template <typename TK>
    void Hash<TK>::Resize()
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

    template <typename TK>
    typename Hash<TK>::Node* Hash<TK>::operator[]( int idx ) const
    {
        assert( idx < nodes.Size() );
        return nodes[ idx ];
    }






}


#endif
