template<typename T>
List<T>::List( int capacity )
{
    assert( capacity > 0 );
    auto len = int( capacity * sizeof( T ) );
    if( len < 64 ) len = 64;
    else len = (int)Utils::round2n( len );
    _size = 0;
    _buf = new char[ len ];
    _maxSize = len / sizeof( T );
}

template<typename T>
List<T>::~List()
{
    if( _buf )
    {
        clear();
        delete[] _buf;
    }
}

template<typename T>
List<T>::List( List && other )
    : _buf( other._buf )
    , _size( other._size )
    , _maxSize( other._maxSize )
{
    other._buf = nullptr;
}

template<typename T>
List<T>::List( List const& other )
    : List( other._size )
{
    if( Utils::isValueType<T>() )
        memcpy( _buf, other._buf, other.byteSize() );
    else
        for( int i = 0; i < other._size; ++i )
            new ( (T*)_buf + i ) T( ( (T*)other._buf )[ i ] );
}

template<typename T>
List<T>& List<T>::operator=( List && other )
{
    clear();
    delete[] _buf;
    _buf = other._buf;
    _size = other._size;
    _maxSize = other._maxSize;
    other._buf = nullptr;
    return *this;
}

template<typename T>
List<T>& List<T>::operator=( List const& other )
{
    if( this == &other ) return *this;
    clear();
    _size = other._size;
    if( _maxSize < other._size )
    {
        auto len = int( other._size * sizeof( T ) );
        if( len < 64 ) len = 64;
        else len = (int)Utils::round2n( len );
        _maxSize = len / sizeof( T );
        delete[] _buf;
        _buf = new char[ len ];
    }
    if( Utils::isValueType<T>() )
        memcpy( _buf, other._buf, other.byteSize() );
    else
        for( int i = 0; i < other._size; ++i )
            new ( (T*)_buf + i ) T( ( (T*)other._buf )[ i ] );
    return *this;
}

template<typename T>
template<typename VT>
void List<T>::push( VT&& v )
{
    if( _size == _maxSize ) reserve( _size + 1 );
    set( _size++, std::forward<VT>( v ) );
}

template<typename T>
void List<T>::pop()
{
    assert( _size > 0 );
    ( (T*)_buf )[ --_size ].~T();
}

template<typename T>
T& List<T>::top()
{
    assert( _size > 0 );
    return ( (T*)_buf )[ _size - 1 ];
}

template<typename T>
void List<T>::clear()
{
    for( int i = 0; i < _size; ++i )
        ( (T*)_buf )[ i ].~T();
    _size = 0;
}

template<typename T>
void List<T>::reserve( int capacity )
{
    assert( capacity > 0 );
    if( capacity <= _maxSize ) return;
    auto len = (int)Utils::round2n( capacity * sizeof( T ) );
    _maxSize = len / sizeof( T );
    auto newBuffer = new char[ len ];
    if( Utils::isValueType<T>() )
        memcpy( newBuffer, _buf, byteSize() );
    else
        for( int i = 0; i < _size; ++i )
            new ( (T*)newBuffer + i ) T( std::move( ( (T*)_buf )[ i ] ) );
    delete[] _buf;
    _buf = newBuffer;
}

template<typename T>
void List<T>::resize( int capacity, bool init /*= true */ )
{
    if( capacity == _size ) return;
    else if( capacity < _size )
    {
        if( !Utils::isValueType<T>() )
            for( int i = capacity; i < _size; ++i )
                ( (T*)_buf )[ i ].~T();
        _size = capacity;
    }
    else
    {
        reserve( capacity );
        if( init )
        {

            if( Utils::isValueType<T>() )
                memset( _buf + _size * sizeof( T ), 0, ( capacity - _size ) * sizeof( T ) );
            else
                for( int i = _size; i < capacity; ++i )
                    new ( (T*)newBuffer + i ) T( std::move( ( (T*)_buf )[ i ] ) );
            _size = capacity;
        }
    }
}

template<typename T>
T* List<T>::data() const
{
    return (T*)_buf;
}

template<typename T>
int List<T>::size() const
{
    return _size;
}

template<typename T>
int List<T>::byteSize() const
{
    return _size * sizeof( T );
}

template<typename T>
int List<T>::byteMaxSize() const
{
    return _maxSize * size( T );
}

template<typename T>
T const & List<T>::operator[]( int idx ) const
{
    return ( (T*)_buf )[ idx ];
}

template<typename T>
T& List<T>::operator[]( int idx )
{
    return ( (T*)_buf )[ idx ];
}

template<typename T>
T const & List<T>::at( int idx ) const
{
    return ( (T*)_buf )[ idx ];
}

template<typename T>
template<typename VT>
void List<T>::set( int idx, VT&& v )
{
    assert( idx >= 0 && idx < _size );
    if( Utils::isValueType<T>() )
        ( (T*)_buf )[ idx ] = v;
    else
        ( (T*)_buf )[ idx ] = std::forward<VT>( v );
}
