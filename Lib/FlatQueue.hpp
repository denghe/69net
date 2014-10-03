
template <class T>
template<typename ...PTS>
void FlatQueue<T>::emplace( PTS&& ...ps )
{
    new ( _buf + _tail++ ) T( std::forward<PTS>( ps )... );
    if( _tail == _maxSize ) _tail = 0;
    if( _tail == _head ) reserve( _maxSize + 1, true );
}

template <class T>
FlatQueue<T>::FlatQueue( int capacity )
{
    assert( capacity > 0 );
    auto byteLen = int( capacity * sizeof( T ) );
    if( byteLen < 64 ) byteLen = 64;
    else byteLen = (int)Utils::round2n( byteLen );
    _buf = ( T* )new char[ byteLen ];
    _maxSize = byteLen / sizeof( T );
    _head = 0;
    _tail = 0;
}

template <class T>
FlatQueue<T>::~FlatQueue()
{
    if( _buf )
    {
        clear();
        delete[]( char* )_buf;
    }
}


template <class T>
int FlatQueue<T>::size() const
{
    if( _head <= _tail )
        return _tail - _head;
    else
        return _maxSize - _head + _tail;
}

template <class T>
bool FlatQueue<T>::empty() const
{
    return _head == _tail;
}


template <class T>
void FlatQueue<T>::pop()
{
    assert( _head != _tail );
    _buf[ _head++ ].~T();
    if( _head == _maxSize ) _head = 0;
}

template <class T>
T const& FlatQueue<T>::top() const
{
    assert( _head != _tail );
    return _buf[ _head ];
}

template <class T>
T& FlatQueue<T>::top()
{
    assert( _head != _tail );
    return _buf[ _head ];
}


template <class T>
void FlatQueue<T>::reserve( int capacity, bool afterPush )
{
    assert( capacity > 0 );
    if( capacity <= _maxSize ) return;

    int size = afterPush ? _maxSize : this->size();

    auto byteLen = (int)Utils::round2n( capacity * sizeof( T ) );
    auto newBuf = ( T* )new char[ byteLen ];

    if( std::is_pod<T>::value
        || std::is_base_of<Memmoveable, T>::value )
    {
        if( _head <= _tail )
        {
            memcpy( newBuf, _buf + _tail, ( _maxSize - _tail ) * sizeof( T ) );
            memcpy( newBuf + _maxSize - _tail, _buf, _head * sizeof( T ) );
        }
        else
        {
            memcpy( newBuf, _buf + _tail, size * sizeof( T ) );
        }
    }
    else
    {
        if( _head <= _tail )
        {
            for( int i = 0; i < _maxSize - _tail; ++i )
                new ( newBuf + i ) T( std::move( _buf[ _tail + i ] ) );
            auto p = newBuf + _maxSize - _tail;
            for( int i = 0; i < _head; ++i )
                new ( p + i ) T( std::move( _buf[ i ] ) );
        }
        else
        {
            for( int i = 0; i < size; ++i )
                new ( newBuf + i ) T( std::move( _buf[ _tail + i ] ) );
        }
    }
    delete[]( char* )_buf;
    _buf = newBuf;
    _maxSize = byteLen / sizeof( T );
    _head = 0;
    _tail = size;
}

template <class T>
void FlatQueue<T>::push( T const& v )
{
    new ( _buf + _tail++ ) T( v );
    if( _tail == _maxSize ) _tail = 0;
    if( _tail == _head ) reserve( _maxSize + 1, true );
}


template <class T>
void FlatQueue<T>::clear()
{
    if( !std::is_pod<T>::value )
    {
        if( _head <= _tail )
        {
            for( int i = 0; i < _head; ++i ) _buf[ i ].~T();
            for( int i = 0; i < _maxSize - _tail; ++i ) _buf[ _tail + i ].~T();
        }
        else
        {
            for( int i = _tail; i < _head; ++i ) _buf[ i ].~T();
        }
    }
    _head = 0;
    _tail = 0;
}


template<typename T>
T const & FlatQueue<T>::operator[]( int idx ) const
{
    return at( idx );
}

template<typename T>
T& FlatQueue<T>::operator[]( int idx )
{
    return at( idx );
}

template<typename T>
T const & FlatQueue<T>::at( int idx ) const
{
    if( _head + idx >= _maxSize )
        return _buf[ _head + idx - _maxSize ];
    else
        return _buf[ _head + idx ];
}

template<typename T>
T& FlatQueue<T>::at( int idx )
{
    if( _head + idx >= _maxSize )
        return _buf[ _head + idx - _maxSize ];
    else
        return _buf[ _head + idx ];
}
