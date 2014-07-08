template <typename TK, typename TV>
Dict<TK, TV>::Dict( int capacity /*= 64 */ )
{
    _pool.init( sizeof( Node ), sizeof( Node ) * capacity, 2 );
    int prime = Utils::getPrime( _pool.size() );
    _nodes.reserve( prime );
    _buckets.resize( prime );
}

template <typename TK, typename TV>
Dict<TK, TV>::~Dict()
{
    for( int i = 0; i < _nodes.size(); ++i )
        dispose( _nodes[ i ] );
}

template <typename TK, typename TV>
template <typename KT, typename VT>
typename Dict<TK, TV>::Node* Dict<TK, TV>::insert( KT && k, VT && v, bool replace /*= false */ )
{
    uint hashCode = (uint)Utils::getHashCode( k );          // calc
    uint mod = hashCode % (uint)_buckets.size();            // find
    auto node = _buckets[ mod ];                            // get chain header
    while( node )
    {
        if( node->hash == hashCode && Utils::equalsTo( node->key, k ) )      // scan chain
        {
            if( replace ) node->value = std::forward<VT>( v );
            return node;
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
    return nullptr;
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
}

template <typename TK, typename TV>
void Dict<TK, TV>::clear()
{
    for( int i = 0; i < _nodes; ++i )
    {
        dispose( _nodes[ i ] );
        _pool.free( _nodes[ i ] );
    }
    memset( _buckets.data(), 0, _buckets.byteSize() );
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
