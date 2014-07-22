#ifndef _STRING_HPP__
#define _STRING_HPP__


template<typename ...TS>
void String::append( TS const & ...vs )
{
    int maxLen = Utils::getFillMaxLength( vs... ) + 1;
    if( _dataLen + maxLen > _bufLen )
        reserve( _dataLen + maxLen );
    _dataLen += Utils::fill( _buf + _dataLen, vs... );
}

template<typename ...TS>
String String::make( TS const & ...vs )
{
    String rtv;
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
String String::toString( T const& v )
{
    String s( Utils::getToStringMaxLength( v ) );
    s.append( v );
    return s;
}

template<typename T>
String String::toHexString( T const& v )
{
    String s( 17 );
    s.appendHex( v );
    return s;
}



#endif
