#ifndef _LIST_HPP__
#define _LIST_HPP__


template<typename T>
List<T>::List( int capacity )
{
    assert( capacity >= 0 );
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
    if( Utils::isValueType<T>() )
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
    if( Utils::isValueType<T>() )
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
    if( _size == _maxSize ) reserve( _size + 1 );
    if( Utils::isValueType<T>() )
        _buf[ _size ] = v;
    else
        new ( _buf + _size ) T( std::forward<VT>( v ) );
    ++_size;
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
    if( Utils::isValueType<T>() )
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
        if( !Utils::isValueType<T>() )
            for( int i = capacity; i < _size; ++i )
                _buf[ i ].~T();
    }
    else
    {
        reserve( capacity );
        if( init )
        {

            if( Utils::isValueType<T>() )
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
    if( Utils::isValueType<T>() )
        _buf[ idx ] = v;
    else
        _buf[ idx ] = std::forward<VT>( v );
}





template<typename T>
int List<T>::getBufferSize() const
{
    if( Utils::isValueType<T>() )
    {
        return sizeof( int ) + sizeof( T ) * _size;
    }

    int siz = sizeof( int );
    for( int i = 0; i < _size; ++i )
    {
        siz += _buf[ i ].getBufferSize();
    }
    return siz;
}

template<typename T>
void List<T>::writeBuffer( FlatBuffer& fb ) const
{
    fb.write( _size );
    if( Utils::isValueType<T>() )
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
    if( Utils::isValueType<T>() )
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
    if( !fb.read( len )
        || len < 0 ) return false;              // todo: || len > maxListLength
    if( Utils::isValueType<T>() )
    {
        int siz = len * (int)sizeof( T );
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
