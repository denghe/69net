#ifndef _STRING_HPP__
#define _STRING_HPP__

namespace xxx
{

    template<int len>
    String::String( Pool& p, char const ( &s )[ len ] )
    {
        assert( p.ItemBufLen() - sizeof( Pool* ) >= len );
        _buf = (char*)p.Alloc();
        _bufLen = p.ItemBufLen();
        _dataLen = len - 1;
        _disposer = &p;
        memcpy( _buf, s, len );
    }


    template<typename ...TS>
    void String::Append( TS const & ...vs )
    {
        int maxLen = GetFillMaxLength( vs... ) + 1;
        if( _dataLen + maxLen > _bufLen )
            Reserve( _dataLen + maxLen );
        _dataLen += Fill( _buf + _dataLen, vs... );
    }

    template<typename ...TS>
    String String::Make( TS const & ...vs )
    {
        String rtv;
        rtv.Append( vs... );
        return rtv;
    }

    template<typename ...TS, int bufLen>
    String String::Make( char( &buf )[ bufLen ], TS const & ...vs )
    {
        String rtv( buf, bufLen, 0 );
        rtv.Append( vs... );
        return rtv;
    }

    template<typename ...TS>
    String String::MakeFast( TS const & ...vs )
    {
        static std::atomic<int> bufIdx( 0 );
        static char bufs[ 32 ][ 1024 ];
        String rtv( bufs[ ( bufIdx++ ) % 32 ], 1024, 0 );
        rtv.Append( vs... );
        return rtv;
    }

    template<typename ...TS>
    String String::MakeFormat( char const* format, TS const & ...vs )
    {
        String rtv;
        rtv.AppendFormat( format, vs... );
        return rtv;
    }

    template<typename ...TS>
    String String::Make( Pool& p, TS const & ...vs )
    {
        String rtv( p );
        rtv.Append( vs... );
        return rtv;
    }

    template<typename ...TS>
    String String::MakeFormat( Pool& p, char const* format, TS const & ...vs )
    {
        String rtv( p );
        rtv.AppendFormat( format, vs... );
        return rtv;
    }




    template<typename T>
    void String::AppendFormatCore( String& s, int& n, T const & v )
    {
        if( !n ) s.Append( v );
    }

    template<typename T, typename ...TS>
    void String::AppendFormatCore( String& s, int& n, T const & v, TS const & ...vs )
    {
        if( n-- == 0 )
        {
            s.Append( v );
            return;
        }
        AppendFormatCore( s, n, vs... );
    }

    template<typename ...TS>
    void String::AppendFormat( char const* format, TS const & ...vs )
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
                    Push( '{' );
                }
                else
                {
                    while( ( c = format[ offset ] ) )
                    {
                        if( c == '}' )
                        {
                            FromString( i, numBuf );
                            numStr.Clear();
                            if( i < 0 || i >= sizeof...( vs ) )
                            {
                                throw std::invalid_argument( "argument out of range." );
                            }
                            if( flags[ i ].len )
                            {
                                Reserve( _dataLen + flags[ i ].len );
                                memcpy( _buf + _dataLen, _buf + flags[ i ].idx, flags[ i ].len );
                                _dataLen += flags[ i ].len;
                                _buf[ _dataLen ] = '\0';
                            }
                            else
                            {
                                flags[ i ].idx = _dataLen;
                                n = i;
                                AppendFormatCore( *this, n, vs... );
                                flags[ i ].len = _dataLen - flags[ i ].idx;
                            }

                            break;
                        }
                        else
                        {
                            numStr.Push( c );
                        }
                        ++offset;
                    }
                }
            }
            else
            {
                Push( c );
            }
            ++offset;
        }
    }



    template<typename T>
    void String::AppendHex( T const& v )
    {
        Reserve( _dataLen + 17 );
        _dataLen += ::xxx::ToHexString( _buf + _dataLen, v );
        _buf[ _dataLen ] = '\0';
    }


    template<typename T>
    String& String::operator << ( T const& v )
    {
        Append( v );
        return *this;
    }

    template<typename T>
    String String::ToString( T const& v )
    {
        return Make( v );
    }


    template<typename T>
    String const String::ToStringFast( T const& v )
    {
        static std::atomic<int> bufIdx( 0 );
        static char bufs[ 32 ][ 128 ];
        if( GetToStringMaxLength( v ) >= 128 ) return Make( v );
        String s( bufs[ ( bufIdx++ ) % 32 ], 128, 0 );
        s.Append( v );
        return s;
    }

    template<typename T>
    String String::ToHexString( T const& v )
    {
        String s;
        s.AppendHex( v );
        return s;
    }

    template<typename T>
    String const String::ToHexStringFast( T const& v )
    {
        static std::atomic<int> bufIdx( 0 );
        static char bufs[ 32 ][ 32 ];
        String s( bufs[ ( bufIdx++ ) % 32 ], 32, 0 );
        s.AppendHex( v );
        return s;
    }


    template<typename T>
    void String::GetFillMaxLengthCore( int & len, T const & v )
    {
        len += GetToStringMaxLength( v );
    }

    template<typename T, typename ...TS>
    void String::GetFillMaxLengthCore( int & len, T const & v, TS const & ...vs )
    {
        GetFillMaxLengthCore( len, v );
        GetFillMaxLengthCore( len, vs... );
    }

    template<typename ...TS>
    int String::GetFillMaxLength( TS const & ...vs )
    {
        int len = 0;
        GetFillMaxLengthCore( len, vs... );
        return len;
    }

    template<typename T>
    void String::FillCore( char * & buf, int & offset, T const & v )
    {
        offset += ::xxx::ToString( buf + offset, v );
    }

    template<typename T, typename ...TS>
    void String::FillCore( char * & buf, int & offset, T const & v, TS const & ...vs )
    {
        FillCore( buf, offset, v );
        FillCore( buf, offset, vs... );
    }

    template<typename ...TS>
    int String::Fill( char * buf, TS const & ...vs )
    {
        int offset = 0;
        FillCore( buf, offset, vs... );
        buf[ offset ] = '\0';
        return offset;
    }

    template<typename T>
    void String::FillHexCore( char * & buf, int & offset, T const & v )
    {
        offset += ::xxx::ToHexString( buf + offset, v );
    }

    template<typename T, typename ...TS>
    void String::FillHexCore( char * & buf, int & offset, T const & v, TS const & ...vs )
    {
        FillHexCore( buf, offset, v );
        FillHexCore( buf, offset, vs... );
    }

    template<typename ...TS>
    int String::FillHex( char * buf, TS const & ...vs )
    {
        int offset = 0;
        FillHexCore( buf, offset, vs... );
        buf[ offset ] = '\0';
        return offset;
    }

}


#endif
