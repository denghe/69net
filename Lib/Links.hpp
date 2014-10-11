#ifndef _LINKS_HPP__
#define _LINKS_HPP__

template <typename T>
Links<T>::Links( int capacity /*= 64 */ )
{
    _pool.init( sizeof( Node ), 4096, capacity );
    _nodes.reserve( capacity );
}

template <typename T>
Links<T>::Links( Links const& o )
    : Links( o.size() )
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
    clear();
    reserve( o.size() );
    for( int i = 0; i < o.size(); ++i )
    {
        insert( o[ i ]->value );
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
    for( int i = 0; i < _nodes.size(); ++i )
        dispose( _nodes[ i ] );
}

template <typename T>
template <typename VT>
typename Links<T>::Node* Links<T>::insert( VT && v )
{
    auto n = (Node*)_pool.alloc();
    new ( &n->value ) T( std::forward<VT>( v ) );
    n->index = _nodes.size();
    _nodes.push( n );
    return n;
}


template <typename T>
typename Links<T>::Node* Links<T>::find( T const& v )
{
    for( int i = 0; i < _nodes.size(); ++i )
    {
        if( Utils::equalsTo( _nodes[ i ]->value, v ) )
        {
            return node;
        }
    }
    return nullptr;
}

template <typename T>
void Links<T>::erase( T const& v )
{
    auto n = find( v );
    if( !n ) return;
    erase( n );
}

template <typename T>
void Links<T>::erase( Node* n )
{
    auto last = _nodes.top();
    _nodes.pop();
    _nodes[ n->index ] = last;
    last->index = n->index;

    dispose( n );
    _pool.free( n );
}

template <typename T>
void Links<T>::clear()
{
    for( int i = 0; i < _nodes.size(); ++i )
    {
        dispose( _nodes[ i ] );
        _pool.free( _nodes[ i ] );
    }
    _nodes.clear();
}


template <typename T>
void Links<T>::reserve( int capacity )
{
    _nodes.reserve( capacity );
    // todo: pool reserve ?
}


template <typename T>
List<typename Links<T>::Node*> const & Links<T>::data() const
{
    return _nodes;
}

template <typename T>
int Links<T>::size() const
{
    return _nodes.size();
}

template <typename T>
void Links<T>::dispose( Node* n )
{
    n->value.~T();
}


template <typename T>
bool Links<T>::empty()
{
    return _nodes.size() == 0;
}


template <typename T>
typename Links<T>::Node* Links<T>::operator[]( int idx ) const
{
    assert( idx < _nodes.size() );
    return _nodes[ idx ];
}








template <typename T>
int Links<T>::getWriteBufferSize() const
{
    int siz = sizeof( int );
    if( std::is_pod<T>::value )
    {
        siz += sizeof( T ) * _nodes.size();
    }
    else
    {
        for( int i = 0; i < _nodes.size(); ++i )
        {
            siz += _nodes[ i ]->value.getWriteBufferSize();
        }
    }
    return siz;
}
template <typename T>
void Links<T>::writeBuffer( FlatBuffer& fb ) const
{
    fb.write( _nodes.size() );
    for( int i = 0; i < _nodes.size(); ++i )
    {
        fb.write( _nodes[ i ]->value );
    }
}
template <typename T>
void Links<T>::writeBufferDirect( FlatBuffer& fb ) const
{
    fb.writeDirect( _nodes.size() );
    for( int i = 0; i < _nodes.size(); ++i )
    {
        fb.writeDirect( _nodes[ i ]->value );
    }
}

template <typename T>
bool Links<T>::readBuffer( FlatBuffer& fb )
{
    int len;
    if( !fb.read( len ) || len < 0 ) return false;
    clear();
    reserve( len );
    for( int i = 0; i < len; ++i )
    {
        auto n = (Node*)_pool.alloc();                              // malloc
        if( !std::is_pod<T>::value ) new ( &n->value ) T();          // new data
        if( !fb.read( n->value ) )
        {
            if( !std::is_pod<T>::value ) n->value.~T();              // delete data
            _pool.free( n );                                        // free
            return false;
        }
        n->index = i;//_nodes.size();
        _nodes.push( n );
    }
    return true;
}





#endif
