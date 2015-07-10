#pragma once

namespace xxx
{

    template <typename T>
    Links<T>::Links( int capacity /*= 64 */ )
    {
        pool.Init( sizeof( Node ), 4096, capacity );
        nodes.Reserve( capacity );
    }

    template <typename T>
    Links<T>::Links( Links const& o )
        : Links( o.Size() )
    {
        operator=( o );
    }

    template <typename T>
    Links<T>::Links( Links&& o )
    {
        operator=( std::move( o ) );
    }

    template <typename T>
    Links<T>& Links<T>::operator=( Links const& o )
    {
        if( this == &o ) return *this;
        Clear();
        Reserve( o.Size() );
        for( int i = 0; i < o.Size(); ++i )
        {
            Insert( o[ i ]->value );
        }
        return *this;
    }

    template <typename T>
    Links<T>& Links<T>::operator=( Links&& o )
    {
        nodes = std::move( o.nodes );
        pool = std::move( o.pool );
        return *this;
    }

    template <typename T>
    Links<T>::~Links()
    {
        for( int i = 0; i < nodes.Size(); ++i )
            Dispose( nodes[ i ] );
    }

    template <typename T>
    template <typename VT>
    typename Links<T>::Node* Links<T>::Insert( VT && v )
    {
        auto n = (Node*)pool.Alloc();
        new ( &n->value ) T( std::forward<VT>( v ) );
        n->index = nodes.Size();
        nodes.Push( n );
        return n;
    }


    template <typename T>
    typename Links<T>::Node* Links<T>::Find( T const& v )
    {
        for( int i = 0; i < nodes.Size(); ++i )
        {
            if( EqualsTo( nodes[ i ]->value, v ) )
            {
                return nodes[ i ];
            }
        }
        return nullptr;
    }

    template <typename T>
    void Links<T>::Erase( T const& v )
    {
        auto n = Find( v );
        if( !n ) return;
        Erase( n );
    }

    template <typename T>
    void Links<T>::Erase( Node* n )
    {
        auto last = nodes.Top();
        nodes.Pop();
        if( n != last )
        {
            nodes[ n->index ] = last;
        }
        last->index = n->index;

        Dispose( n );
        pool.Free( n );
    }

    template <typename T>
    void Links<T>::Clear()
    {
        for( int i = 0; i < nodes.Size(); ++i )
        {
            Dispose( nodes[ i ] );
            pool.Free( nodes[ i ] );
        }
        nodes.Clear();
    }


    template <typename T>
    void Links<T>::Reserve( int capacity )
    {
        nodes.Reserve( capacity );
        // todo: pool Reserve ?
    }


    template <typename T>
    List<typename Links<T>::Node*> const & Links<T>::Data() const
    {
        return nodes;
    }
    template <typename T>
    List<typename Links<T>::Node*>& Links<T>::Data()
    {
        return nodes;
    }

    template <typename T>
    int Links<T>::Size() const
    {
        return nodes.Size();
    }

    template <typename T>
    void Links<T>::Dispose( Node* n )
    {
        n->value.~T();
    }


    template <typename T>
    bool Links<T>::Empty()
    {
        return nodes.Size() == 0;
    }


    template <typename T>
    typename Links<T>::Node const* Links<T>::operator[]( int idx ) const
    {
        assert( idx < nodes.Size() );
        return nodes[ idx ];
    }
    template <typename T>
    typename Links<T>::Node* Links<T>::operator[]( int idx )
    {
        assert( idx < nodes.Size() );
        return nodes[ idx ];
    }
    template <typename T>
    typename Links<T>::Node const* Links<T>::IndexAt( int idx ) const
    {
        assert( idx < nodes.Size() );
        return nodes[ idx ];
    }
    template <typename T>
    typename Links<T>::Node* Links<T>::IndexAt( int idx )
    {
        assert( idx < nodes.Size() );
        return nodes[ idx ];
    }

}
