#ifndef _FLATBUFFER_HPP__
#define _FLATBUFFER_HPP__



HAS_FUNC( writeBuffer_checker, writeBuffer, void ( T::* )( FlatBuffer& ) const );

template<typename T>
typename std::enable_if<writeBuffer_checker<T>::value, void>::type writeBuffer_switch( FlatBuffer& fb, T const& v )
{
    v.writeBuffer( fb );
};
template<typename T>
typename std::enable_if<!writeBuffer_checker<T>::value, void>::type writeBuffer_switch( FlatBuffer& fb, T const& v )
{
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
void FlatBuffer::write( char const( &s )[ len ] )
{
    write( len );
    write( s, len - 1 );
}




template<typename T>
void FlatBuffer::writesCore( T const& v )
{
    write( v );
}

template<typename T, typename ...TS>
void FlatBuffer::writesCore( T const& v, TS const& ...vs )
{
    write( v );
    writesCore( vs... );
}

template<typename ...TS>
void FlatBuffer::writes( TS const& ...vs )
{
    // todo: writeDirect
    // reserve( _dataLen + Utils::getSize( vs... ) );
    writesCore( vs... );
}



#endif
