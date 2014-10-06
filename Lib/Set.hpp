#ifndef _SET_HPP__
#define _SET_HPP__

template <typename TK>
Set<TK>::Set( int capacity /*= 64 */ )
{
    _pool.init( sizeof( Node ), 4096, capacity );
    int prime = Utils::getPrime( capacity );
    _nodes.reserve( prime );
    _buckets.resize( prime );
}

template <typename TK>
Set<TK>::Set( Set const& o )
    : Set( o.size() )
{
    operator=( o );
}

template <typename TK>
Set<TK>::Set( Set&& o )
{
    _buckets = std::move( o._buckets );
    _nodes = std::move( o._nodes );
    _pool = std::move( o._pool );
}

template <typename TK>
Set<TK>& Set<TK>::operator=( Set const& o )
{
    if( this == &o ) return *this;
    clear();
    reserve( o.size() );
    for( int i = 0; i < o.size(); ++i )
    {
        auto on = o.data()[ i ];
        uint mod = on->hash % (uint)_buckets.size();
        auto n = (Node*)_pool.alloc();
        n->next = _buckets[ mod ];
        n->hash = on->hash;
        n->index = _nodes.size();
        new ( &n->key ) TK( on->key );
        _buckets[ mod ] = n;
        _nodes.push( n );
    }
    return *this;
}

template <typename TK>
Set<TK>& Set<TK>::operator=( Set&& o )
{
    _buckets = std::move( o._buckets );
    _nodes = std::move( o._nodes );
    _pool = std::move( o._pool );
    return *this;
}

template <typename TK>
Set<TK>::~Set()
{
    for( int i = 0; i < _nodes.size(); ++i )
        dispose( _nodes[ i ] );
}

template <typename TK>
template <typename KT>
std::pair<typename Set<TK>::Node*, bool> Set<TK>::insert( KT && k )
{
    std::pair<typename Set<TK>::Node*, bool> rtv;
    uint hashCode = (uint)Utils::getHashCode( k );
    uint mod = hashCode % (uint)_buckets.size();
    auto node = _buckets[ mod ];
    while( node )
    {
        if( node->hash == hashCode && Utils::equalsTo( node->key, k ) )
        {
            rtv.first = node;
            rtv.second = false;
            return rtv;
        }
        node = node->next;
    };
    auto n = (Node*)_pool.alloc();
    n->next = _buckets[ mod ];
    n->hash = hashCode;
    n->index = _nodes.size();
    new ( &n->key ) TK( std::forward<KT>( k ) );
    _buckets[ mod ] = n;
    _nodes.push( n );
    if( _nodes.size() == _buckets.size() ) resize();
    rtv.first = n;
    rtv.second = true;
    return rtv;
}


template <typename TK>
typename Set<TK>::Node* Set<TK>::find( TK const& k )
{
    uint hashCode = (uint)Utils::getHashCode( k );
    uint mod = hashCode % (uint)_buckets.size();
    auto node = _buckets[ mod ];
    while( node )
    {
        if( node->hash == hashCode && Utils::equalsTo( node->key, k ) ) return node;
        node = node->next;
    }
    return nullptr;
}

template <typename TK>
void Set<TK>::erase( TK const& k )
{
    auto n = find( k );
    if( !n ) return;
    erase( n );
}

template <typename TK>
void Set<TK>::erase( Node* n )
{
    auto mod = n->hash % (uint)_buckets.size();
    Node* parent = nullptr;
    auto node = _buckets[ mod ];
    do
    {
        if( node == n )
        {
            if( parent )
            {
                parent->next = n->next;
                break;
            }
        }
        parent = node;
        node = node->next;
    } while( node );

    auto last = _nodes.top();
    _nodes.pop();
    _nodes[ n->index ] = last;
    last->index = n->index;

    dispose( n );
    _pool.free( n );
    if( n == _buckets[ mod ] ) _buckets[ mod ] = nullptr;
}

template <typename TK>
void Set<TK>::clear()
{
    for( int i = 0; i < _nodes.size(); ++i )
    {
        dispose( _nodes[ i ] );
        _pool.free( _nodes[ i ] );
    }
    _nodes.clear();
    memset( _buckets.data(), 0, _buckets.byteSize() );
}


template <typename TK>
void Set<TK>::reserve( int capacity )
{
    if( capacity <= _buckets.size() ) return;
    int prime = Utils::getPrime( (int)Utils::round2n( capacity ) );
    _nodes.reserve( prime );
    _buckets.resize( prime, false );
    memset( _buckets.data(), 0, _buckets.byteSize() );      // clean up
    for( int i = 0; i < _nodes.size(); ++i )
    {
        auto& o = _nodes[ i ];
        auto mod = o->hash % prime;
        o->next = _buckets[ mod ];
        _buckets[ mod ] = o;
    }
}


template <typename TK>
List<typename Set<TK>::Node*> const & Set<TK>::data() const
{
    return _nodes;
}

template <typename TK>
int Set<TK>::size() const
{
    return _nodes.size();
}

template <typename TK>
void Set<TK>::dispose( Node* n )
{
    n->key.~TK();
}


template <typename TK>
bool Set<TK>::empty()
{
    return _nodes.size() == 0;
}


template <typename TK>
void Set<TK>::resize()
{
    int prime = Utils::getPrime( _nodes.size() * 3 );
    _nodes.reserve( prime );
    _buckets.resize( prime, false );
    memset( _buckets.data(), 0, _buckets.byteSize() );      // clean up
    for( int i = 0; i < _nodes.size(); ++i )
    {
        auto& o = _nodes[ i ];
        auto mod = o->hash % prime;
        o->next = _buckets[ mod ];
        _buckets[ mod ] = o;
    }
}

template <typename TK>
typename Set<TK>::Node* Set<TK>::operator[]( int idx )
{
    assert( idx < _nodes.size() );
    return _nodes[ idx ];
}








template <typename TK>
int Set<TK>::getWriteBufferSize() const
{
    int siz = sizeof( int );
    if( std::is_pod<TK>::value )
    {
        siz += sizeof( TK ) * _nodes.size();
    }
    else
    {
        for( int i = 0; i < _nodes.size(); ++i )
        {
            siz += _nodes[ i ]->key.getWriteBufferSize();
        }
    }
    return siz;
}
template <typename TK>
void Set<TK>::writeBuffer( FlatBuffer& fb ) const
{
    fb.write( _nodes.size() );
    for( int i = 0; i < _nodes.size(); ++i )
    {
        fb.write( _nodes[ i ]->key );
    }
}
template <typename TK>
void Set<TK>::writeBufferDirect( FlatBuffer& fb ) const
{
    fb.writeDirect( _nodes.size() );
    for( int i = 0; i < _nodes.size(); ++i )
    {
        fb.writeDirect( _nodes[ i ]->key );
    }
}

template <typename TK>
bool Set<TK>::readBuffer( FlatBuffer& fb )
{
    int len;
    if( !fb.read( len ) || len < 0 ) return false;
    clear();
    reserve( len );
    for( int i = 0; i < len; ++i )
    {
        auto n = (Node*)_pool.alloc();                              // malloc
        if( !std::is_pod<TK>::value ) new ( &n->key ) TK();         // new key
        if( !fb.read( n->key ) )
        {
            if( !std::is_pod<TK>::value ) n->key.~TK();             // delete key
            _pool.free( n );                                        // free
            return false;
        }
        n->hash = (uint)Utils::getHashCode( n->key );
        uint mod = n->hash % (uint)_buckets.size();
        n->index = i;//_nodes.size();
        n->next = _buckets[ mod ];
        _buckets[ mod ] = n;
        _nodes.push( n );
    }
    return true;
}





#endif
