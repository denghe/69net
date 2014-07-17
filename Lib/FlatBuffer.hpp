#ifndef _FLATBUFFER_HPP__
#define _FLATBUFFER_HPP__

HAS_FUNC( writeBufferDirect_checker, writeBufferDirect, void ( T::* )( FlatBuffer& ) const );
template<typename T>
typename std::enable_if<writeBufferDirect_checker<T>::value, void>::type writeBufferDirect_switch( FlatBuffer& fb, T const& v )
{
    v.writeBufferDirect( fb );
};
template<typename T>
typename std::enable_if<!writeBufferDirect_checker<T>::value, void>::type writeBufferDirect_switch( FlatBuffer& fb, T const& v )
{
    assert( std::is_pod<T>::value );
    BufferUtils::write( fb.data() + fb.size(), v );
    fb.size() += BufferUtils::getSize( v );
};
template<typename T>
void FlatBuffer::writeDirect( T const& v )
{
    writeBufferDirect_switch( *this, v );
}


HAS_FUNC( writeBuffer_checker, writeBuffer, void ( T::* )( FlatBuffer& ) const );
template<typename T>
typename std::enable_if<writeBuffer_checker<T>::value, void>::type writeBuffer_switch( FlatBuffer& fb, T const& v )
{
    v.writeBuffer( fb );
};
template<typename T>
typename std::enable_if<!writeBuffer_checker<T>::value, void>::type writeBuffer_switch( FlatBuffer& fb, T const& v )
{
    assert( std::is_pod<T>::value );
    int siz = BufferUtils::getSize( v );
    fb.reserve( fb.size() + siz );
    BufferUtils::write( fb.data() + fb.size(), v );
    fb.size() += siz;
};
template<typename T>
void FlatBuffer::write( T const& v )
{
    writeBuffer_switch( *this, v );
}


template<int len>
void FlatBuffer::writeDirect( char const( &s )[ len ] )
{
    writeDirect( len - 1 );
    writeDirect( s, len - 1 );
}
template<int len>
void FlatBuffer::write( char const( &s )[ len ] )
{
    reserve( _dataLen + sizeof( int ) + len - 1 );
    writeDirect( s );
}


template<typename T, int len>
void FlatBuffer::writeDirect( T const( &a )[ len ] )
{
    if( std::is_pod<T>::value )
    {
        writeDirect( (char*)a, len * sizeof( T ) );
    }
    else
    {
        for( int i = 0; i < len; ++i )
        {
            writeBufferDirect_switch( *this, a[ i ] );
        }
    }
}
template<typename T, int len>
void FlatBuffer::write( T const( &a )[ len ] )
{
    if( std::is_pod<T>::value )
    {
        auto siz = len * ( int )sizeof( T );
        reserve( _dataLen + siz );
        writeDirect( (char*)a, len * sizeof( T ) );
    }
    else
    {
        for( int i = 0; i < len; ++i )
        {
            writeBuffer_switch( *this, a[ i ] );
        }
    }
}





template<typename T>
void FlatBuffer::writesCore( T const& v )
{
    writeDirect( v );
}

template<typename T, typename ...TS>
void FlatBuffer::writesCore( T const& v, TS const& ...vs )
{
    writeDirect( v );
    writesCore( vs... );
}

template<typename ...TS>
void FlatBuffer::writes( TS const& ...vs )
{
    reserve( _dataLen + BufferUtils::getSizes( vs... ) );
    writesCore( vs... );
}

template<typename ...TS>
void FlatBuffer::writesDirect( TS const& ...vs )
{
    writesCore( vs... );
}



HAS_FUNC( readBuffer_checker, readBuffer, bool ( T::* )( FlatBuffer& ) );
template<typename T>
typename std::enable_if<readBuffer_checker<T>::value, bool>::type readBuffer_switch( FlatBuffer& fb, T& v )
{
    return v.readBuffer( fb );
};
template<typename T>
typename std::enable_if<!readBuffer_checker<T>::value, bool>::type readBuffer_switch( FlatBuffer& fb, T& v )
{
    assert( std::is_pod<T>::value );
    auto siz = BufferUtils::getSize( v );
    if( fb.offset() + siz > fb.size() ) return false;
    BufferUtils::read( v, fb.data() + fb.offset() );
    fb.offset() += siz;
    return true;
};
template<typename T>
bool FlatBuffer::read( T& v )
{
    return readBuffer_switch( *this, v );
}


template<typename T, int len>
bool FlatBuffer::read( T( &a )[ len ] )
{
    if( std::is_pod<T>::value )
    {
        int siz = len * ( int )sizeof( T );
        if( _offset + siz > _dataLen ) return false;
        memcpy( &a, _buf + _offset, siz );
        _offset += siz;
        return true;
    }
    for( int i = 0; i < len; ++i )
    {
        if( !readBuffer_switch( *this, a[ i ] ) ) return false;
    }
    return true;
}



template<typename T>
bool FlatBuffer::readsCore( T& v )
{
    return read( v );
}

template<typename T, typename ...TS>
bool FlatBuffer::readsCore( T& v, TS& ...vs )
{
    if( !read( v ) ) return false;
    return readsCore( vs... );
}

template<typename ...TS>
bool FlatBuffer::reads( TS& ...vs )
{
    return readsCore( vs... );
}





#endif
