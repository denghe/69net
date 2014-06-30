template<typename T>
Stack<T>::Stack( int capacity /*= 32 */ )
{
    auto size = capacity * sizeof( T );
    if( size < 64 ) _bufLen = 64;
    else _bufLen = (int)Utils::round2n( size );
    _dataCount = 0;
    _buf = new char[ _bufLen ];
}


template<typename T>
Stack<T>::~Stack()
{
    delete[] _buf;
}


template<typename T>
int Stack<T>::count() const
{
    return _dataCount;
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
    auto newBuffer = new char[ size ];
    if( Utils::isValueType<T>() )
    {
        memcpy( newBuffer, _buf, _dataCount * sizeof( T ) );
    }
    else
    {
        for( int i = 0; i < _dataCount; ++i ) new ( (T*)newBuffer + i ) T( std::move( ( (T*)_buf )[ i ] ) );
    }
    delete[] _buf;
    _buf = newBuffer;
}

template<typename T>
void Stack<T>::clear()
{
    for( int i = 0; i < _dataCount; ++i ) ( (T*)_buf )[ i ].~T();
    _dataCount = 0;
}

template<typename T>
T& Stack<T>::top()
{
    return ( (T*)_buf )[ _dataCount ];
}

template<typename T>
template<typename VT>
void Stack<T>::push( VT&& v )
{
    int size = ( _dataCount + 1 ) * sizeof( T );
    if( size > _bufLen ) reserve( size );
    new ( (T*)_buf + _dataCount++ ) T( std::forward<VT>( v ) );
}

template<typename T>
void Stack<T>::pop()
{
    assert( _dataCount > 0 );
    ( (T*)_buf )[ --_dataCount ].~T();
}
