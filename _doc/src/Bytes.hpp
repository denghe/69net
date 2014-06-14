#ifndef _BYTES_HPP_
#define _BYTES_HPP_


template<typename T>
Bytes & Bytes::operator<<( T const & v )
{
    write( v );
    return *this;
}


template<typename T>
ReadStatus Bytes::operator>>( T & v )
{
    return read( v );
}


template<typename T>
void Bytes::write( T const & v )
{
    v.bytesW( *this );
}


template<typename T>
ReadStatus Bytes::read( T & v )
{
    return v.bytesR( *this );
}


template<typename T>
void Bytes::writeDirect( T const & v )
{
    v.bytesDW( *this );
}


template<typename T>
void Bytes::readDirect( T & v )
{
    v.bytesDR( *this );
}













template<typename T>
void Bytes::writeDirect( List<T> const & vs )
{
    writeDirect( (uint)vs._len );
    for( int i = 0; i < vs._len; ++i )
    {
        writeDirect( vs[ i ] );
    }
}


template<typename T>
void Bytes::readDirect( List<T> & vs )
{
    uint len = 0; 
    readDirect( len );
    vs.clear();
    vs.ensure( len );
    vs._len = len;
    for( int i = 0; i < len; ++i )
    {
        new ( &vs._buf[ i ] ) T();
        readDirect( vs._buf[ i ] );
    }
}


template<typename T>
void Bytes::write( List<T> const & vs )
{
    write( (uint)vs._len );
    for( int i = 0; i < vs._len; ++i )
    {
        write( vs[ i ] );
    }
}


template<typename T>
ReadStatus Bytes::read( List<T> & vs )
{
    vs.clear();
    uint len = 0; 
    auto rtv = read( len );
    if( rtv != ReadStatus::Success ) return rtv;
    vs.ensure( len );
    for( uint i = 0; i < len; ++i )
    {
        new ( &vs._buf[ i ] ) T();
        vs._len = i + 1;
        rtv = read( vs._buf[ i ] );
        if( rtv != ReadStatus::Success ) return rtv;
    }
    return ReadStatus::Success;
}











// todo: 支持 nullptr

template<typename T>
ReadStatus Bytes::read( std::unique_ptr<T> & v )
{
    // todo
    return ReadStatus.Success;
}

template<typename T>
void Bytes::write( std::unique_ptr<T> const & v )
{
    // todo
}




#endif

