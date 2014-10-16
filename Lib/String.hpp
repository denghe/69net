#ifndef _STRING_HPP__
#define _STRING_HPP__

namespace xxx
{

    template<int len>
    String::String( Pool& p, char const ( &s )[ len ] )
    {
        assert( p.ItemBufLen() - sizeof( Pool* ) >= len );
        buf = (char*)p.Alloc();
        bufLen = p.ItemBufLen();
        dataLen = len - 1;
        disposer = &p;
        memcpy( buf, s, len );
    }


    template<typename ...TS>
    void String::Append( TS const & ...vs )
    {
        int maxLen = GetFillMaxLength( vs... ) + 1;
        if( dataLen + maxLen > bufLen )
            Reserve( dataLen + maxLen );
        dataLen += Fill( buf + dataLen, vs... );
    }

    template<typename ...TS>
    String String::Make( TS const & ...vs )
    {
        String rtv;
        rtv.Append( vs... );
        return rtv;
    }

    template<typename ...TS, int _bufLen>
    String String::Make( char( &_buf )[ _bufLen ], TS const & ...vs )
    {
        String rtv( _buf, _bufLen, 0 );
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
                                Reserve( dataLen + flags[ i ].len );
                                memcpy( buf + dataLen, buf + flags[ i ].idx, flags[ i ].len );
                                dataLen += flags[ i ].len;
                                buf[ dataLen ] = '\0';
                            }
                            else
                            {
                                flags[ i ].idx = dataLen;
                                n = i;
                                AppendFormatCore( *this, n, vs... );
                                flags[ i ].len = dataLen - flags[ i ].idx;
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
        Reserve( dataLen + 17 );
        dataLen += ::xxx::ToHexString( buf + dataLen, v );
        buf[ dataLen ] = '\0';
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
    void String::FillCore( char * & _buf, int & offset, T const & v )
    {
        offset += ::xxx::ToString( _buf + offset, v );
    }

    template<typename T, typename ...TS>
    void String::FillCore( char * & _buf, int & offset, T const & v, TS const & ...vs )
    {
        FillCore( _buf, offset, v );
        FillCore( _buf, offset, vs... );
    }

    template<typename ...TS>
    int String::Fill( char * _buf, TS const & ...vs )
    {
        int offset = 0;
        FillCore( _buf, offset, vs... );
        _buf[ offset ] = '\0';
        return offset;
    }

    template<typename T>
    void String::FillHexCore( char * & _buf, int & offset, T const & v )
    {
        offset += ::xxx::ToHexString( _buf + offset, v );
    }

    template<typename T, typename ...TS>
    void String::FillHexCore( char * & _buf, int & offset, T const & v, TS const & ...vs )
    {
        FillHexCore( _buf, offset, v );
        FillHexCore( _buf, offset, vs... );
    }

    template<typename ...TS>
    int String::FillHex( char * _buf, TS const & ...vs )
    {
        int offset = 0;
        FillHexCore( _buf, offset, vs... );
        _buf[ offset ] = '\0';
        return offset;
    }

}


#endif
