template<typename T>
Stack<T>::Stack( int capacity )
{
    auto size = capacity * sizeof( T );
    if( size < 64 ) _bufLen = 64;
    else _bufLen = (int)Utils::round2n( size );
    _itemCount = 0;
    _buf = new char[ _bufLen ];
    _maxItemCount = _bufLen / sizeof( T );
}


template<typename T>
Stack<T>::~Stack()
{
    delete[] _buf;
}


template<typename T>
int Stack<T>::count() const
{
    return _itemCount;
}

template<typename T>
T* Stack<T>::items() const
{
    return (T*)_buf;
}

template<typename T>
void Stack<T>::reserve( int capacity )
{
    auto size = capacity * sizeof( T );
    if( size <= _bufLen ) return;
    size = Utils::round2n( size );
    _bufLen = (int)size;
    _maxItemCount = _bufLen / sizeof( T );
    auto newBuffer = new char[ size ];
    if( Utils::isValueType<T>() )
    {
        memcpy( newBuffer, _buf, _itemCount * sizeof( T ) );
    }
    else
    {
        for( int i = 0; i < _itemCount; ++i )
            new ( (T*)newBuffer + i ) T( std::move( ( (T*)_buf )[ i ] ) );
    }
    delete[] _buf;
    _buf = newBuffer;
}

template<typename T>
void Stack<T>::clear()
{
    for( int i = 0; i < _itemCount; ++i )
        ( (T*)_buf )[ i ].~T();
    _itemCount = 0;
}

template<typename T>
T& Stack<T>::top()
{
    assert( _itemCount > 0 );
    return ( (T*)_buf )[ _itemCount - 1 ];
}

template<typename T>
template<typename VT>
void Stack<T>::push( VT&& v )
{
    if( _itemCount == _maxItemCount ) reserve( _itemCount + 1 );
    if( Utils::isValueType<T>() )
        ( (T*)_buf )[ _itemCount++ ] = v;
    else
        new ( (T*)_buf + _itemCount++ ) T( std::forward<VT>( v ) );
}

template<typename T>
void Stack<T>::pop()
{
    assert( _itemCount > 0 );
    ( (T*)_buf )[ --_itemCount ].~T();
}

template<typename T>
T& Stack<T>::operator[]( int idx ) const
{
    return ( (T*)_buf )[ idx ];
}
