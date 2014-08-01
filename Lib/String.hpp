#ifndef _STRING_HPP__
#define _STRING_HPP__


template<int len>
String::String( Pool& p, char const ( &s )[ len ] )
{
    assert( p.attachThis() && p.itemBufLen() > sizeof( Pool* ) && p.itemBufLen() - sizeof( Pool* ) >= len );
    _buf = (char*)p.alloc();
    _bufLen = p.itemBufLen() - sizeof( Pool* );
    _dataLen = len - 1;
    _disposer = &String::disposePoolBuffer;
    memcpy( _buf, s, len );
}


template<typename ...TS>
void String::append( TS const & ...vs )
{
    int maxLen = getFillMaxLength( vs... ) + 1;
    if( _dataLen + maxLen > _bufLen )
        reserve( _dataLen + maxLen );
    _dataLen += fill( _buf + _dataLen, vs... );
}

template<typename ...TS>
String String::make( TS const & ...vs )
{
    String rtv;
    rtv.append( vs... );
    return rtv;
}

template<typename ...TS, int bufLen>
String String::make( char( &buf )[ bufLen ], TS const & ...vs )
{
    String rtv( buf, bufLen, 0 );
    rtv.append( vs... );
    return rtv;
}

template<typename ...TS>
String String::makeFast( TS const & ...vs )
{
    static std::atomic<int> bufIdx = 0;
    static char bufs[ 32 ][ 1024 ];
    String rtv( bufs[ ( bufIdx++ ) & 32 ], 1024, 0 );
    rtv.append( vs... );
    return rtv;
}

template<typename ...TS>
String String::makeFormat( char const* format, TS const & ...vs )
{
    String rtv;
    rtv.appendFormat( format, vs... );
    return rtv;
}

template<typename ...TS>
String String::make( Pool& p, TS const & ...vs )
{
    String rtv( p );
    rtv.append( vs... );
    return rtv;
}

template<typename ...TS>
String String::makeFormat( Pool& p, char const* format, TS const & ...vs )
{
    String rtv( p );
    rtv.appendFormat( format, vs... );
    return rtv;
}




template<typename T>
void String::appendFormatCore( String& s, int& n, T const & v )
{
    if( !n ) s.append( v );
}

template<typename T, typename ...TS>
void String::appendFormatCore( String& s, int& n, T const & v, TS const & ...vs )
{
    if( n-- == 0 )
    {
        s.append( v );
        return;
    }
    appendFormatCore( s, n, vs... );
}

template<typename ...TS>
void String::appendFormat( char const* format, TS const & ...vs )
{
    struct flag { int idx, len; };
    flag flags[ sizeof...( vs ) ];
    memset( flags, 0, sizeof( flag )*sizeof...( vs ) );

    ALIGN8( char numBuf[ 32 ] );
    String numStr( numBuf, 32, 0 );

    int offset = 0, i = 0, n = 0;
    while( auto c = format[ offset ] )
    {
        if( c == '{' )
        {
            c = format[ ++offset ];
            if( c == '{' )
            {
                push( '{' );
            }
            else
            {
                while( c = format[ offset ] )
                {
                    if( c == '}' )
                    {
                        Utils::fromString( i, numBuf );
                        numStr.clear();
                        if( i < 0 || i >= sizeof...( vs ) )
                        {
                            throw std::invalid_argument( "argument out of range." );
                        }
                        if( flags[ i ].len )
                        {
                            reserve( _dataLen + flags[ i ].len );
                            memcpy( _buf + _dataLen, _buf + flags[ i ].idx, flags[ i ].len );
                            _dataLen += flags[ i ].len;
                            _buf[ _dataLen ] = '\0';
                        }
                        else
                        {
                            flags[ i ].idx = _dataLen;
                            n = i;
                            appendFormatCore( *this, n, vs... );
                            flags[ i ].len = _dataLen - flags[ i ].idx;
                        }

                        break;
                    }
                    else
                    {
                        numStr.push( c );
                    }
                    ++offset;
                }
            }
        }
        else
        {
            push( c );
        }
        ++offset;
    }
}



template<typename T>
void String::appendHex( T const& v )
{
    reserve( _dataLen + 17 );
    _dataLen += Utils::toHexString( _buf + _dataLen, v );
    _buf[ _dataLen ] = '\0';
}


template<typename T>
String& String::operator << ( T const& v )
{
    append( v );
    return *this;
}

template<typename T>
String const String::toString( T const& v )
{
    static std::atomic<int> bufIdx = 0;
    static char bufs[ 32 ][ 128 ];
    if( getToStringMaxLength( v ) >= 128 ) return make( v );
    String s( bufs[ ( bufIdx++ ) & 32 ], 128, 0 );
    s.append( v );
    return s;
}

template<typename T>
String String::toHexString( T const& v )
{
    static std::atomic<int> bufIdx = 0;
    static char bufs[ 32 ][ 32 ];
    String s( bufs[ ( bufIdx++ ) & 32 ], 32, 0 );
    s.appendHex( v );
    return s;
}


template<typename T>
void String::getFillMaxLengthCore( int & len, T const & v )
{
    len += Utils::getToStringMaxLength( v );
}

template<typename T, typename ...TS>
void String::getFillMaxLengthCore( int & len, T const & v, TS const & ...vs )
{
    getFillMaxLengthCore( len, v );
    getFillMaxLengthCore( len, vs... );
}

template<typename ...TS>
int String::getFillMaxLength( TS const & ...vs )
{
    int len = 0;
    getFillMaxLengthCore( len, vs... );
    return len;
}

template<typename T>
void String::fillCore( char * & buf, int & offset, T const & v )
{
    offset += Utils::toString( buf + offset, v );
}

template<typename T, typename ...TS>
void String::fillCore( char * & buf, int & offset, T const & v, TS const & ...vs )
{
    fillCore( buf, offset, v );
    fillCore( buf, offset, vs... );
}

template<typename ...TS>
int String::fill( char * buf, TS const & ...vs )
{
    int offset = 0;
    fillCore( buf, offset, vs... );
    buf[ offset ] = '\0';
    return offset;
}

template<typename T>
void String::fillHexCore( char * & buf, int & offset, T const & v )
{
    offset += toHexString( buf + offset, v );
}

template<typename T, typename ...TS>
void String::fillHexCore( char * & buf, int & offset, T const & v, TS const & ...vs )
{
    fillHexCore( buf, offset, v );
    fillHexCore( buf, offset, vs... );
}

template<typename ...TS>
int String::fillHex( char * buf, TS const & ...vs )
{
    int offset = 0;
    fillHexCore( buf, offset, vs... );
    buf[ offset ] = '\0';
    return offset;
}



#endif
