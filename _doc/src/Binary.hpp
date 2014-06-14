#ifndef _BINARY_HPP_
#define _BINARY_HPP_


template<typename T>
Binary & Binary::operator<<( T const & v )
{
    write( v );
    return *this;
}


template<typename T>
ReadStatus Binary::operator>>( T & v )
{
    return read( v );
}


template<typename T>
void Binary::write( T const & v )
{
    v.binaryW( *this );
}


template<typename T>
ReadStatus Binary::read( T & v )
{
    return v.binaryR( *this );
}


template<typename T>
void Binary::writeDirect( T const & v )
{
    v.binaryDW( *this );
}


template<typename T>
void Binary::readDirect( T & v )
{
    v.binaryDR( *this );
}













template<typename T>
void Binary::writeDirect( List<T> const & vs )
{
    writeDirect( vs._len );
    for( int i = 0; i < vs._len; ++i )
    {
        writeDirect( vs[ i ] );
    }
}


template<typename T>
void Binary::readDirect( List<T> & vs )
{
    int len = 0; 
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
void Binary::write( List<T> const & vs )
{
    write( vs._len );
    for( int i = 0; i < vs._len; ++i )
    {
        write( vs[ i ] );
    }
}


template<typename T>
ReadStatus Binary::read( List<T> & vs )
{
    if( getFreeReadSpace() < sizeof(int) ) return ReadStatus::NotEnoughData;
    int len = 0; 
    readDirect( len );
    vs.clear();
    vs.ensure( len );
    ReadStatus status;
    for( int i = 0; i < len; ++i )
    {
        new ( &vs._buf[ i ] ) T();
        vs._len = i + 1;
        status = read( vs._buf[ i ] );
        if( status != ReadStatus::Success ) return status;
    }
    return ReadStatus::Success;
}




#endif

