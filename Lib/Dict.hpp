#ifndef _DICT_HPP__
#define _DICT_HPP__

template <typename TK, typename TV>
Dict<TK, TV>::Dict( int capacity /*= 64 */ )
{
    _pool.init( sizeof( Node ), (int)Utils::round2n( sizeof( Node ) * capacity ), 2 );
    int prime = Utils::getPrime( _pool.size() );
    _nodes.reserve( prime );
    _buckets.resize( prime );
}

template <typename TK, typename TV>
Dict<TK, TV>::Dict( Dict const& o )
    : Dict( o.size() )
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
        new ( (TK*)&n->key ) TK( on->key );
        new ( &n->value ) TV( on->value );
        _buckets[ mod ] = n;
        _nodes.push( n );
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
    for( int i = 0; i < _nodes.size(); ++i )
        dispose( _nodes[ i ] );
}

template <typename TK, typename TV>
template <typename KT, typename VT>
std::pair<typename Dict<TK, TV>::Node*, bool> Dict<TK, TV>::insert( KT && k, VT && v, bool replace /*= false */ )
{
    std::pair<typename Dict<TK, TV>::Node*, bool> rtv;
    uint hashCode = (uint)Utils::getHashCode( k );          // calc
    uint mod = hashCode % (uint)_buckets.size();            // find
    auto node = _buckets[ mod ];                            // get chain header
    while( node )
    {
        if( node->hash == hashCode && Utils::equalsTo( node->key, k ) )      // scan chain
        {
            if( replace ) node->value = std::forward<VT>( v );
            rtv.first = node;
            rtv.second = false;
            return rtv;
        }
        node = node->next;
    };
    auto n = (Node*)_pool.alloc();                          // new & init
    n->next = _buckets[ mod ];
    n->hash = hashCode;
    n->index = _nodes.size();
    new ( (TK*)&n->key ) TK( std::forward<KT>( k ) );
    new ( &n->value ) TV( std::forward<VT>( v ) );
    _buckets[ mod ] = n;
    _nodes.push( n );
    if( _nodes.size() == _buckets.size() ) resize();        // grow
    rtv.first = n;
    rtv.second = true;
    return rtv;
}

template <typename TK, typename TV>
typename Dict<TK, TV>::Node* Dict<TK, TV>::find( TK const& k )
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

template <typename TK, typename TV>
void Dict<TK, TV>::erase( TK const& k )
{
    auto n = find( k );
    if( !n ) return;
    erase( n );
}

template <typename TK, typename TV>
void Dict<TK, TV>::erase( Node* n )
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

template <typename TK, typename TV>
void Dict<TK, TV>::clear()
{
    for( int i = 0; i < _nodes.size(); ++i )
    {
        dispose( _nodes[ i ] );
        _pool.free( _nodes[ i ] );
    }
    _nodes.clear();
    memset( _buckets.data(), 0, _buckets.byteSize() );
}


template <typename TK, typename TV>
void Dict<TK, TV>::reserve( int capacity )
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


template <typename TK, typename TV>
List<typename Dict<TK, TV>::Node*> const & Dict<TK, TV>::data() const
{
    return _nodes;
}

template <typename TK, typename TV>
int Dict<TK, TV>::size() const
{
    return _nodes.size();
}


template <typename TK, typename TV>
void Dict<TK, TV>::dispose( Node* n )
{
    n->key.~TK();
    n->value.~TV();
}

template <typename TK, typename TV>
void Dict<TK, TV>::resize()
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










template <typename TK, typename TV>
int Dict<TK, TV>::getBufferSize() const
{
    //if( Utils::isValueType<T>() )
    //{
    //    return sizeof( int ) + sizeof( T ) * _size;
    //}

    //int siz = sizeof( int );
    //for( int i = 0; i < _size; ++i )
    //{
    //    siz += _buf[ i ].getBufferSize();
    //}
    //return siz;
    return 0;
}
template <typename TK, typename TV>
void Dict<TK, TV>::writeBuffer( FlatBuffer& fb ) const
{
    //fb.write( _size );
    //if( Utils::isValueType<T>() )
    //{
    //    fb.write( (char*)_buf, _size * sizeof( T ) );
    //    return;
    //}
    //for( int i = 0; i < _size; ++i )
    //{
    //    fb.write( _buf[ i ] );
    //}
}
template <typename TK, typename TV>
void Dict<TK, TV>::writeBufferDirect( FlatBuffer& fb ) const
{
    //fb.writeDirect( _size );
    //if( Utils::isValueType<T>() )
    //{
    //    fb.writeDirect( (char*)_buf, _size * sizeof( T ) );
    //    return;
    //}
    //for( int i = 0; i < _size; ++i )
    //{
    //    fb.writeDirect( _buf[ i ] );
    //}
}

template <typename TK, typename TV>
bool Dict<TK, TV>::readBuffer( FlatBuffer& fb )
{
    //int len;
    //if( !fb.read( len )
    //    || len < 0 ) return false;              // todo: || len > maxListLength
    //if( Utils::isValueType<T>() )
    //{
    //    int siz = len * ( int )sizeof( T );
    //    if( fb.offset() + siz > fb.size() ) return false;
    //    clear();
    //    resize( len, false );
    //    memcpy( _buf, fb.data() + fb.offset(), siz );
    //    fb.offset() += siz;
    //    return true;
    //}
    //clear();
    //reserve( len );
    //for( int i = 0; i < len; ++i )
    //{
    //    new ( _buf + i ) T();
    //    _size = i + 1;
    //    if( !_buf[ i ].readBuffer( fb ) ) return false;
    //}
    return true;
}





#endif
