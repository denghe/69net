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
    assert( Utils::isValueType<T>() );
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
    assert( Utils::isValueType<T>() );
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
    writeDirect( len );
    writeDirect( s, len - 1 );
}
template<int len>
void FlatBuffer::write( char const( &s )[ len ] )
{
    reserve( sizeof( int ) + len - 1 );
    writeDirect( s );
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



#endif
