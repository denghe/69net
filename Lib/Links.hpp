#ifndef _LINKS_HPP__
#define _LINKS_HPP__

namespace xxx
{


    template <typename T>
    Links<T>::Links( int capacity /*= 64 */ )
    {
        _pool.Init( sizeof( Node ), 4096, capacity );
        _nodes.Reserve( capacity );
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
        _nodes = std::move( o._nodes );
        _pool = std::move( o._pool );
        return *this;
    }

    template <typename T>
    Links<T>::~Links()
    {
        for( int i = 0; i < _nodes.Size(); ++i )
            Dispose( _nodes[ i ] );
    }

    template <typename T>
    template <typename VT>
    typename Links<T>::Node* Links<T>::Insert( VT && v )
    {
        auto n = (Node*)_pool.Alloc();
        new ( &n->value ) T( std::forward<VT>( v ) );
        n->index = _nodes.Size();
        _nodes.Push( n );
        return n;
    }


    template <typename T>
    typename Links<T>::Node* Links<T>::Find( T const& v )
    {
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            if( EqualsTo( _nodes[ i ]->value, v ) )
            {
                return _nodes[ i ];
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
        auto last = _nodes.Top();
        _nodes.Pop();
        _nodes[ n->index ] = last;
        last->index = n->index;

        Dispose( n );
        _pool.Free( n );
    }

    template <typename T>
    void Links<T>::Clear()
    {
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            Dispose( _nodes[ i ] );
            _pool.Free( _nodes[ i ] );
        }
        _nodes.Clear();
    }


    template <typename T>
    void Links<T>::Reserve( int capacity )
    {
        _nodes.Reserve( capacity );
        // todo: pool Reserve ?
    }


    template <typename T>
    List<typename Links<T>::Node*> const & Links<T>::Data() const
    {
        return _nodes;
    }

    template <typename T>
    int Links<T>::Size() const
    {
        return _nodes.Size();
    }

    template <typename T>
    void Links<T>::Dispose( Node* n )
    {
        n->value.~T();
    }


    template <typename T>
    bool Links<T>::Empty()
    {
        return _nodes.Size() == 0;
    }


    template <typename T>
    typename Links<T>::Node* Links<T>::operator[]( int idx ) const
    {
        assert( idx < _nodes.Size() );
        return _nodes[ idx ];
    }








    template <typename T>
    int Links<T>::GetWriteBufferSize() const
    {
        int siz = sizeof( int );
        if( std::is_pod<T>::value )
        {
            siz += sizeof( T ) * _nodes.Size();
        }
        else
        {
            for( int i = 0; i < _nodes.Size(); ++i )
            {
                siz += _nodes[ i ]->value.GetWriteBufferSize();
            }
        }
        return siz;
    }
    template <typename T>
    void Links<T>::WriteBuffer( FlatBuffer& fb ) const
    {
        fb.Write( _nodes.Size() );
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            fb.Write( _nodes[ i ]->value );
        }
    }
    template <typename T>
    void Links<T>::WriteBufferDirect( FlatBuffer& fb ) const
    {
        fb.WriteDirect( _nodes.Size() );
        for( int i = 0; i < _nodes.Size(); ++i )
        {
            fb.WriteDirect( _nodes[ i ]->value );
        }
    }

    template <typename T>
    bool Links<T>::ReadBuffer( FlatBuffer& fb )
    {
        int len;
        if( !fb.Read( len ) || len < 0 ) return false;
        Clear();
        Reserve( len );
        for( int i = 0; i < len; ++i )
        {
            auto n = (Node*)_pool.Alloc();                              // malloc
            if( !std::is_pod<T>::value ) new ( &n->value ) T();          // new data
            if( !fb.Read( n->value ) )
            {
                if( !std::is_pod<T>::value ) n->value.~T();              // delete data
                _pool.Free( n );                                        // free
                return false;
            }
            n->index = i;//_nodes.Size();
            _nodes.Push( n );
        }
        return true;
    }



}

#endif
