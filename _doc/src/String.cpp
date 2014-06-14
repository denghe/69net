#include "Precompile.h"

String const String::defaultValue;

String::String( int bufSize )
    : _len( 0 )
{
    _bufSize = (int)round2n( bufSize );
    _buf = (char*)malloc( _bufSize );
}

String::String( String const & o )
	: _len( o._len )
    , _bufSize( o._bufSize )
{
    _buf = (char*)malloc( o._bufSize );
    memcpy( _buf, o._buf, o._len );
}

String::String( String && o )
	: _len( o._len )
	, _bufSize( o._bufSize )
    , _buf( o._buf )
{
    o._buf = nullptr;
}

String::String( char const * s )
{
    assert( s );
    _len = (int)strlen( s );
    if( _len )
    {
        _bufSize = (int)round2n( _len );
        _buf = (char*)malloc( _bufSize );
        memcpy( _buf, s, _len );
    }
    else
    {
        _bufSize = DefaultStringBufferSize;
        _buf = (char*)malloc( DefaultStringBufferSize );
    }
}

String::String( wchar const * s )
{
    assert( s );
    _len = (int)wcslen( s );
    if( _len )
    {
        _bufSize = (int)round2n( _len * 3 );
        _buf = (char*)malloc( _bufSize );
#pragma warning(disable:4996)
        _len = Converts::toStringUtf8( _buf, s );
    }
    else
    {
        _bufSize = DefaultStringBufferSize;
        _buf = (char*)malloc( DefaultStringBufferSize );
    }
}


String::String( char const * buf, int len )
{
    _len = len;
    _bufSize = (int)round2n( len );
    _buf = (char*)malloc( _bufSize );
    memcpy( _buf, buf, len );
}

String::String( wchar const * buf, int len )
{
    _bufSize = (int)round2n( len * 3 );
    _buf = (char*)malloc( _bufSize );
    _len = Converts::toStringUtf8( _buf, buf, len );
}

String::~String()
{
    if( _buf ) free( _buf );
}

String & String::operator=( String const & o )
{
    if( _bufSize < o._len )
    {
        _bufSize = (int)round2n( o._len );
        free( _buf );
        _buf = (char*)malloc( _bufSize );
    }
    memcpy( _buf, o._buf, o._len );
    _len = o._len;
    return *this;
}

String & String::operator=( String && o )
{
    std::swap( _buf, o._buf );
    _bufSize = o._bufSize;
    _len = o._len;
    return *this;
}

bool String::operator==( String const & s ) const
{
    if( &s == this ) return true;
    if( s._len != _len ) return false;
    return memcmp( _buf, s._buf, _len ) == 0;
}

bool String::operator!=( String const & s ) const
{
    return !operator==( s );
}


bool String::operator<( String const & s ) const
{
    return !operator>=( s );
}

bool String::operator>( String const & s ) const
{
    return !operator<=( s );
}

bool String::operator<=( String const & s ) const
{
    if( &s == this ) return true;
    int len = _len < s._len ? _len : s._len;
    if( len == 0 ) return _len == 0;
    return memcmp( _buf, s._buf, len ) <= 0;
}

bool String::operator>=( String const & s ) const
{
    if( &s == this ) return true;
    int len = _len < s._len ? _len : s._len;
    if( len == 0 ) return s._len == 0;
    return memcmp( _buf, s._buf, len ) >= 0;
}

void String::assign( char const * buf, int len )
{
    clear( len );
    memcpy( _buf, buf, len );
    _len = len;
}

void String::assign( char const * s )
{
    assert( s );
    assign( s, (int)strlen( s ) );
}

void String::assign( String && s )
{
    operator=( std::forward<String>(s) );
}

void String::clear()
{
    _len = 0;
}

void String::clear( int bufSize )
{
    if( _bufSize < bufSize )
    {
        _bufSize = (int)round2n( bufSize );
        free( _buf );
        _buf = (char*)malloc( _bufSize );
    }
    _len = 0;
}

void String::grow( int bufSize )
{
    if( _bufSize < bufSize )
    {
        _bufSize = (int)round2n( bufSize );
        _buf = (char*)realloc( _buf, _bufSize );
    }
}

void String::ensure( int len )
{
    if( getFreeBufferLen() < len )
    {
        _bufSize = (int)round2n( _len + len );
        _buf = (char*)realloc( _buf, _bufSize );
    }
}

int String::getFreeBufferLen() const
{
    return _bufSize - _len;
}

#define STRINGBUILDER_W( T, L )                     \
void String::write( T v )                           \
{                                                   \
    ensure( L );                                    \
    _len += Converts::toString( _buf + _len, v );   \
}

STRINGBUILDER_W( uint64, 20 )
STRINGBUILDER_W( uint32, 10 )
STRINGBUILDER_W( uint16, 5  )
STRINGBUILDER_W( uint8 , 3  )
STRINGBUILDER_W( int64 , 20 )
STRINGBUILDER_W( int32 , 11 )
STRINGBUILDER_W( int16 , 6  )
STRINGBUILDER_W( int8  , 4  )
STRINGBUILDER_W( float , 16 )
STRINGBUILDER_W( double, 20 )
STRINGBUILDER_W( bool  , 5  )

void String::write( const char * v, int len )
{
    ensure( len );
    memcpy( _buf + _len, v, len );
    _len += len;
}

void String::write( const char * v )
{
    write( v, (int)strlen( v ) );
}

void String::write( String const & v )
{
    write( v._buf, v._len );
}

void String::write( char v )
{
    ensure( 1 );
    _buf[ _len++ ] = v;
}

//void String::write( char * v )
//{
//    write( v, (int)strlen( v ) );
//}


void String::write( wchar const * v, int len )
{
    ensure( len * 3 );
    auto o = Converts::toStringUtf8( _buf + _len, v, len * 3 );
    _len += o;
}

void String::write( wchar const * v )
{
    write( v, (int)wcslen( v ) );
}

//void String::write( wchar * v )
//{
//
//}

void String::write( wchar v )
{
    ensure( 3 );
    _len += Converts::toStringUtf8( _buf + _len, v );
}


char const* String::c_str()
{
    ensure( 1 );
    _buf[ _len ] = 0;
    return _buf;
}

void String::writeHex( uint64 v )
{
    ensure( 16 );
    _len += Converts::toStringHex( _buf + _len, v );
}

void String::writeHex( uint32 v )
{
    ensure( 8 );
    _len += Converts::toStringHex( _buf + _len, v );
}

void String::writeHex( uint16 v )
{
    ensure( 4 );
    _len += Converts::toStringHex( _buf + _len, v );
}

void String::writeHex( uint8 v )
{
    ensure( 2 );
    _len += Converts::toStringHex( _buf + _len, v );
}
