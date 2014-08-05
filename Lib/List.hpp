#ifndef _LIST_HPP__
#define _LIST_HPP__


template<typename T>
List<T>::List( int capacity )
{
    assert( capacity > 0 );
    auto byteLen = int( capacity * sizeof( T ) );
    if( byteLen < 64 ) byteLen = 64;
    else byteLen = (int)Utils::round2n( byteLen );
    _size = 0;
    _buf = ( T* )new char[ byteLen ];
    _maxSize = byteLen / sizeof( T );
}

template<typename T>
List<T>::~List()
{
    if( _buf )
    {
        clear();
        delete[]( char* )_buf;
    }
}

template<typename T>
List<T>::List( List&& other )
    : _buf( other._buf )
    , _size( other._size )
    , _maxSize( other._maxSize )
{
    other._buf = nullptr;
    other._size = 0;
    other._maxSize = 0;
}

template<typename T>
List<T>::List( List const& other )
    : List( other._size )
{
    if( std::is_pod<T>::value )
        memcpy( _buf, other._buf, other.byteSize() );
    else
        for( int i = 0; i < other._size; ++i )
            new ( _buf + i ) T( other._buf[ i ] );
    _size = other._size;
}

template<typename T>
List<T>& List<T>::operator=( List&& other )
{
    clear();
    delete[]( char* )_buf;
    _buf = other._buf;
    _size = other._size;
    _maxSize = other._maxSize;
    other._buf = nullptr;
    other._size = 0;
    other._maxSize = 0;
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
        auto byteLen = int( other._size * sizeof( T ) );
        if( byteLen < 64 ) byteLen = 64;
        else byteLen = (int)Utils::round2n( byteLen );
        _maxSize = byteLen / sizeof( T );
        delete[]( char* )_buf;
        _buf = ( T* )new char[ byteLen ];
    }
    if( std::is_pod<T>::value )
        memcpy( _buf, other._buf, other.byteSize() );
    else
        for( int i = 0; i < other._size; ++i )
            new ( _buf + i ) T( other._buf[ i ] );
    return *this;
}

template<typename T>
template<typename VT>
void List<T>::push( VT&& v )
{
    emplace( std::forward<VT>( v ) );
}

template<typename T>
template<typename ...PTS>
T& List<T>::emplace( PTS&& ...ps )
{
    if( _size == _maxSize ) reserve( _size + 1 );
    return *new ( _buf + _size++ ) T( std::forward<PTS>( ps )... );
}


template<typename T>
void List<T>::pop()
{
    assert( _size > 0 );
    _buf[ --_size ].~T();
}

template<typename T>
T& List<T>::top()
{
    assert( _size > 0 );
    return _buf[ _size - 1 ];
}
template<typename T>
T const& List<T>::top() const
{
    assert( _size > 0 );
    return _buf[ _size - 1 ];
}

template<typename T>
void List<T>::clear()
{
    for( int i = 0; i < _size; ++i )
        _buf[ i ].~T();
    _size = 0;
}

template<typename T>
void List<T>::reserve( int capacity )
{
    assert( capacity > 0 );
    if( capacity <= _maxSize ) return;
    auto byteLen = (int)Utils::round2n( capacity * sizeof( T ) );
    _maxSize = byteLen / sizeof( T );
    auto newBuf = ( T* )new char[ byteLen ];
    if( std::is_pod<T>::value
        || std::is_base_of<Memmoveable, T>::value )
        memcpy( newBuf, _buf, byteSize() );
    else
        for( int i = 0; i < _size; ++i )
            new ( newBuf + i ) T( std::move( _buf[ i ] ) );
    delete[]( char* )_buf;
    _buf = newBuf;
}

template<typename T>
void List<T>::resize( int capacity, bool init /*= true */ )
{
    if( capacity == _size ) return;
    else if( capacity < _size )
    {
        if( !std::is_pod<T>::value )
            for( int i = capacity; i < _size; ++i )
                _buf[ i ].~T();
    }
    else
    {
        reserve( capacity );
        if( init )
        {

            if( std::is_pod<T>::value )
                memset( _buf + _size, 0, ( capacity - _size ) * sizeof( T ) );
            else
                for( int i = _size; i < capacity; ++i )
                    new ( _buf + i ) T();
        }
    }
    _size = capacity;
}

template<typename T>
T* List<T>::data() const
{
    return _buf;
}

template<typename T>
int List<T>::size() const
{
    return _size;
}

template<typename T>
int& List<T>::size()
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
    return _buf[ idx ];
}

template<typename T>
T& List<T>::operator[]( int idx )
{
    return _buf[ idx ];
}

template<typename T>
T const & List<T>::at( int idx ) const
{
    return _buf[ idx ];
}

template<typename T>
T& List<T>::at( int idx )
{
    return _buf[ idx ];
}

template<typename T>
template<typename VT>
void List<T>::set( int idx, VT&& v )
{
    assert( idx >= 0 && idx < _size );
    if( std::is_pod<T>::value )
        _buf[ idx ] = v;
    else
        _buf[ idx ] = std::forward<VT>( v );
}

template<typename T>
int List<T>::find( T const& v )
{
    for( int i = 0; i < _size; ++i )
    {
        if( _buf[ idx ] == v ) return i;
    }
    return -1;
}
template<typename T>
void List<T>::erase( int idx )
{
    assert( idx >= 0 && idx < _size );
    --_size;
    if( std::is_pod<T>::value
        || std::is_base_of<Memmoveable, T>::value )
    {
        memmove( _buf + idx, _buf + idx + 1, ( _size - idx )*sizeof( T ) );
    }
    else
    {
        _buf[ idx ].~T();
        for( int i = idx; i < _size; ++i )
        {
            new ( _buf + i ) T( std::move( _buf[ i + 1 ] ) );
        }
    }
}
template<typename T>
void List<T>::eraseFast( int idx )
{
    assert( idx >= 0 && idx < _size );
    --_size;
    if( std::is_pod<T>::value
        || std::is_base_of<Memmoveable, T>::value )
    {
        _buf[ idx ] = _buf[ _size ];
    }
    else
    {
        _buf[ idx ].~T();
        new ( _buf + idx ) T( std::move( _buf[ _size ] ) );
    }
}

template<typename T>
template<typename VT>
void List<T>::insertAt( int idx, VT&& v )
{
    emplaceAt( idx, std::forward<VT>( v ) );
}
template<typename T>
template<typename ...PTS>
T& List<T>::emplaceAt( int idx, PTS&& ...ps )
{
    assert( idx >= 0 && idx <= _size );
    reserve( _size + 1 );                       // todo: 理论上讲这句可以展开，于扩容时直接将 insert 的元素的内存位置留出来

    if( std::is_pod<T>::value
        || std::is_base_of<Memmoveable, T>::value )
    {
        memmove( _buf + idx + 1, _buf + idx, ( _size - idx )*sizeof( T ) );
        ++_size;
        return *new ( _buf + idx ) T( std::forward<PTS>( ps )... );
    }
    else
    {
        for( int i = _size; i > idx; --i )
        {
            new ( _buf + i ) T( std::move( _buf[ i - 1 ] ) );
        }
        ++_size;
        return *new ( _buf + idx ) T( std::forward<PTS>( ps )... );
    }
}
























template<typename T>
int List<T>::getWriteBufferSize() const
{
    if( std::is_pod<T>::value )
    {
        return sizeof( int ) + sizeof( T ) * _size;
    }

    int siz = sizeof( int );
    for( int i = 0; i < _size; ++i )
    {
        siz += BufferUtils::getSize( _buf[ i ] );
    }
    return siz;
}

template<typename T>
void List<T>::writeBuffer( FlatBuffer& fb ) const
{
    fb.write( _size );
    if( std::is_pod<T>::value )
    {
        fb.write( (char*)_buf, _size * sizeof( T ) );
        return;
    }
    for( int i = 0; i < _size; ++i )
    {
        fb.write( _buf[ i ] );
    }
}

template<typename T>
void List<T>::writeBufferDirect( FlatBuffer& fb ) const
{
    fb.writeDirect( _size );
    if( std::is_pod<T>::value )
    {
        fb.writeDirect( (char*)_buf, _size * sizeof( T ) );
        return;
    }
    for( int i = 0; i < _size; ++i )
    {
        fb.writeDirect( _buf[ i ] );
    }
}


template<typename T>
bool List<T>::readBuffer( FlatBuffer& fb )
{
    int len;
    if( !fb.read( len ) || len < 0 ) return false;
    if( std::is_pod<T>::value )
    {
        int siz = len * ( int )sizeof( T );
        if( fb.offset() + siz > fb.size() ) return false;
        clear();
        resize( len, false );
        memcpy( _buf, fb.data() + fb.offset(), siz );
        fb.offset() += siz;
        return true;
    }
    clear();
    reserve( len );
    for( int i = 0; i < len; ++i )
    {
        new ( _buf + i ) T();
        _size = i + 1;
        if( !_buf[ i ].readBuffer( fb ) ) return false;
    }
    return true;
}


#endif
