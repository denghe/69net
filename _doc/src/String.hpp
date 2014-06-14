#ifndef _STRING_HPP_
#define _STRING_HPP_

template<int len>
String::String( char const (&s)[ len ] )
{
    _len = len;
    _bufSize = (int)round2n( len );
    _buf = (char*)malloc( _bufSize );
    memcpy( _buf, s, len );
}

template<int len>
String::String( wchar const (&s)[ len ] )
{
    _bufSize = (int)round2n( len * 3 );
    _buf = (char*)malloc( _bufSize );
    _len = (int)Converts::toStringUtf8( _buf, s, len );
}

template<int len>
void String::assign( char const (&s)[ len ] )
{
    clear( len );
    memcpy( _buf, s, len );
    _len = len;
}

template<typename T>
void String::assign( T const & v )
{
    clear();
    write( v );
}

template<typename T>
String & String::operator<<( T const & v )
{
    write( v );
    return *this;
}

template<typename T>
void String::write( T const & v )
{
    v.toString( *this );
}


template<typename T>
String & String::operator=( T const & v )
{
    clear();
    write( v );
    return *this;
}

template<typename T>
String & String::operator+=( T const & v )
{
    write( v );
    return *this;
}

template<typename T>
String & String::operator+( T const & v )
{
    write( v );
    return *this;
}



#endif
