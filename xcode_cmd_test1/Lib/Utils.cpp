#include "All.h"
#include "dtoa_milo.h"

namespace xxx
{
    int GetToStringMaxLength( uint8   v ) { return 3; }
    int GetToStringMaxLength( uint16  v ) { return 5; }
    int GetToStringMaxLength( uint    v ) { return 10; }
    int GetToStringMaxLength( uint64  v ) { return 19; }
    int GetToStringMaxLength( int8    v ) { return 4; }
    int GetToStringMaxLength( int16   v ) { return 6; }
    int GetToStringMaxLength( int     v ) { return 11; }
    int GetToStringMaxLength( int64   v ) { return 20; }
    int GetToStringMaxLength( double  v ) { return 19; }
    int GetToStringMaxLength( float   v ) { return 10; }
    int GetToStringMaxLength( bool    v ) { return 5; }
    int GetToStringMaxLength( char    v ) { return 1; }
    int GetToStringMaxLength( std::string const & v ) { return (int)v.size(); }
    int GetToStringMaxLength( char const* v ) { return (int)strlen( v ); }
    int GetToStringMaxLength( String const & v ) { return v.Size(); }









    //
    //
    //    // 取 整数 转换后的 string 长度系列
    //    // todo: if ( n < ...... ) 这种代码理论上讲可以优化成树形, 类似折半查找从而减少 if 次数
    //
    //    int GetToStringSize( uint8 v )
    //    {
    //        if( v < 10 ) return 1;
    //        else if( v < 100 ) return 2;
    //        else return 3;
    //    }
    //
    //    int GetToStringSize( uint16 v )
    //    {
    //        if( v < 10 ) return 1;
    //        else if( v < 100 ) return 2;
    //        else if( v < 1000 ) return 3;
    //        else if( v < 10000 ) return 4;
    //        else return 5;
    //    }
    //
    //    int GetToStringSize( uint32 v )
    //    {
    //        if( v < 10 ) return 1;
    //        else if( v < 100 ) return 2;
    //        else if( v < 1000 ) return 3;
    //        else if( v < 10000 ) return 4;
    //        else if( v < 100000 ) return 5;
    //        else if( v < 1000000 ) return 6;
    //        else if( v < 10000000 ) return 7;
    //        else if( v < 100000000 ) return 8;
    //        else if( v < 1000000000 ) return 9;
    //        else return 10;
    //    }
    //
    //    int GetToStringSize( uint64 v )
    //    {
    //        if( v < 10000000000LL )
    //        {
    //            if( v >= 1000000000 ) return 10;
    //            return GetToStringSize( (uint32)v );
    //        }
    //        else if( v < 100000000000LL ) return 11;
    //        else if( v < 1000000000000LL ) return 12;
    //        else if( v < 10000000000000LL ) return 13;
    //        else if( v < 100000000000000LL ) return 14;
    //        else if( v < 1000000000000000LL ) return 15;
    //        else if( v < 10000000000000000LL ) return 16;
    //        else if( v < 100000000000000000LL ) return 17;
    //        else if( v < 1000000000000000000LL ) return 18;
    //        else if( v < (uint64)( (uint64)1000000000000000000LL * (uint64)10 ) ) return 19;
    //        else return 20;
    //    }
    //
    //
    //    int ToStringCore( uint8 in, char *out )
    //    {
    //        if( in == 0 )
    //        {
    //            out[ 0 ] = '0';
    //            return 1;
    //        }
    //
    //        int idx, len;
    //        idx = len = GetToStringSize( in );
    //        size_t v = in, tmp;
    //        do
    //        {
    //#if __X64
    //            tmp = ( v * 1717986919 ) >> 34;			// v / 10 的高效率版
    //#else
    //            tmp = v / 10;
    //#endif
    //            out[ --idx ] = (char)( 48 + v - tmp * 10 );
    //        } while( ( v = tmp ) );
    //        return len;
    //    }
    //
    //
    //    int ToStringCore( uint16 in, char *out )
    //    {
    //        if( in == 0 )
    //        {
    //            out[ 0 ] = '0';
    //            return 1;
    //        }
    //
    //        int idx, len;
    //        idx = len = GetToStringSize( in );
    //        size_t v = in, tmp;
    //        do
    //        {
    //#if __X64
    //            tmp = ( v * 1717986919 ) >> 34;			// v / 10 的高效率版
    //#else
    //            tmp = v / 10;
    //#endif
    //            out[ --idx ] = (char)( 48 + v - tmp * 10 );
    //        } while( ( v = tmp ) );
    //        return len;
    //    }
    //
    //
    //
    //    int ToStringCore( uint32 in, char *out )
    //    {
    //        if( in == 0 )
    //        {
    //            out[ 0 ] = '0';
    //            return 1;
    //        }
    //
    //        int idx, len;
    //        idx = len = GetToStringSize( in );
    //        size_t v = in, tmp;
    //        do
    //        {
    //#if __X64
    //            tmp = ( v * 1717986919 ) >> 34;			// v / 10 的高效率版
    //#else
    //            tmp = v / 10;
    //#endif
    //            out[ --idx ] = (char)( 48 + v - tmp * 10 );
    //        } while( ( v = tmp ) );
    //        return len;
    //    }
    //
    //
    //    int ToStringCore( uint64 in, char * out )
    //    {
    //        if( in == 0 )
    //        {
    //            out[ 0 ] = '0';
    //            return 1;
    //        }
    //        if( in <= 0xFFFFFFFFLL ) return ToStringCore( (uint32)in, out );
    //        int len, idx;
    //        idx = len = GetToStringSize( in );
    //        size_t v = (size_t)in, tmp;
    //        do
    //        {
    //            tmp = v / 10;
    //            out[ --idx ] = (char)( 48 + v - tmp * 10 );
    //        } while( ( v = tmp ) );
    //        return len;
    //    }
    //
    //
    //    int ToStringCore( int in, char * out )
    //    {
    //        if( in < 0 )
    //        {
    //            *out = '-';
    //            return ToStringCore( (uint32)-in, out + 1 ) + 1;
    //        }
    //        return ToStringCore( (uint32)in, out );
    //    }
    //
    //
    //    int ToStringCore( int64 in, char * out )
    //    {
    //        if( in < 0 )
    //        {
    //            *out = '-';
    //            return ToStringCore( (uint64)-in, out + 1 ) + 1;
    //        }
    //        return ToStringCore( (uint64)in, out );
    //    }
    //


    // the code is copy from following URL:
    // https://github.com/miloyip/itoa-benchmark/tree/master/src

    const char gDigitsLut[ 200 ] = {
        '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9',
        '1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9',
        '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
        '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9',
        '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9',
        '5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
        '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9',
        '7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9',
        '8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
        '9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9'
    };

    // Branching for different cases (forward)
    // Use lookup table of two digits

    int u32toa_branchlut( uint32_t value, char* buffer )
    {
        auto bak = buffer;
        if( value < 10000 )
        {
            const uint32_t d1 = ( value / 100 ) << 1;
            const uint32_t d2 = ( value % 100 ) << 1;

            if( value >= 1000 )
                *buffer++ = gDigitsLut[ d1 ];
            if( value >= 100 )
                *buffer++ = gDigitsLut[ d1 + 1 ];
            if( value >= 10 )
                *buffer++ = gDigitsLut[ d2 ];
            *buffer++ = gDigitsLut[ d2 + 1 ];
        }
        else if( value < 100000000 )
        {
            // value = bbbbcccc
            const uint32_t b = value / 10000;
            const uint32_t c = value % 10000;

            const uint32_t d1 = ( b / 100 ) << 1;
            const uint32_t d2 = ( b % 100 ) << 1;

            const uint32_t d3 = ( c / 100 ) << 1;
            const uint32_t d4 = ( c % 100 ) << 1;

            if( value >= 10000000 )
                *buffer++ = gDigitsLut[ d1 ];
            if( value >= 1000000 )
                *buffer++ = gDigitsLut[ d1 + 1 ];
            if( value >= 100000 )
                *buffer++ = gDigitsLut[ d2 ];
            *buffer++ = gDigitsLut[ d2 + 1 ];

            *buffer++ = gDigitsLut[ d3 ];
            *buffer++ = gDigitsLut[ d3 + 1 ];
            *buffer++ = gDigitsLut[ d4 ];
            *buffer++ = gDigitsLut[ d4 + 1 ];
        }
        else
        {
            // value = aabbbbcccc in decimal

            const uint32_t a = value / 100000000; // 1 to 42
            value %= 100000000;

            if( a >= 10 )
            {
                const unsigned i = a << 1;
                *buffer++ = gDigitsLut[ i ];
                *buffer++ = gDigitsLut[ i + 1 ];
            }
            else
                *buffer++ = '0' + static_cast<char>( a );

            const uint32_t b = value / 10000; // 0 to 9999
            const uint32_t c = value % 10000; // 0 to 9999

            const uint32_t d1 = ( b / 100 ) << 1;
            const uint32_t d2 = ( b % 100 ) << 1;

            const uint32_t d3 = ( c / 100 ) << 1;
            const uint32_t d4 = ( c % 100 ) << 1;

            *buffer++ = gDigitsLut[ d1 ];
            *buffer++ = gDigitsLut[ d1 + 1 ];
            *buffer++ = gDigitsLut[ d2 ];
            *buffer++ = gDigitsLut[ d2 + 1 ];
            *buffer++ = gDigitsLut[ d3 ];
            *buffer++ = gDigitsLut[ d3 + 1 ];
            *buffer++ = gDigitsLut[ d4 ];
            *buffer++ = gDigitsLut[ d4 + 1 ];
        }
        //*buffer++ = '\0';
        return int( buffer - bak );
    }

    int i32toa_branchlut( int32_t value, char* buffer )
    {
        uint32_t u = static_cast<uint32_t>( value );
        if( value < 0 )
        {
            *buffer++ = '-';
            u = ~u + 1;
            return u32toa_branchlut( u, buffer ) + 1;
        }
        return u32toa_branchlut( u, buffer );
    }

    int u64toa_branchlut( uint64_t value, char* buffer )
    {
        auto bak = buffer;
        if( value < 100000000 )
        {
            uint32_t v = static_cast<uint32_t>( value );
            if( v < 10000 )
            {
                const uint32_t d1 = ( v / 100 ) << 1;
                const uint32_t d2 = ( v % 100 ) << 1;

                if( v >= 1000 )
                    *buffer++ = gDigitsLut[ d1 ];
                if( v >= 100 )
                    *buffer++ = gDigitsLut[ d1 + 1 ];
                if( v >= 10 )
                    *buffer++ = gDigitsLut[ d2 ];
                *buffer++ = gDigitsLut[ d2 + 1 ];
            }
            else
            {
                // value = bbbbcccc
                const uint32_t b = v / 10000;
                const uint32_t c = v % 10000;

                const uint32_t d1 = ( b / 100 ) << 1;
                const uint32_t d2 = ( b % 100 ) << 1;

                const uint32_t d3 = ( c / 100 ) << 1;
                const uint32_t d4 = ( c % 100 ) << 1;

                if( value >= 10000000 )
                    *buffer++ = gDigitsLut[ d1 ];
                if( value >= 1000000 )
                    *buffer++ = gDigitsLut[ d1 + 1 ];
                if( value >= 100000 )
                    *buffer++ = gDigitsLut[ d2 ];
                *buffer++ = gDigitsLut[ d2 + 1 ];

                *buffer++ = gDigitsLut[ d3 ];
                *buffer++ = gDigitsLut[ d3 + 1 ];
                *buffer++ = gDigitsLut[ d4 ];
                *buffer++ = gDigitsLut[ d4 + 1 ];
            }
        }
        else if( value < 10000000000000000 )
        {
            const uint32_t v0 = static_cast<uint32_t>( value / 100000000 );
            const uint32_t v1 = static_cast<uint32_t>( value % 100000000 );

            const uint32_t b0 = v0 / 10000;
            const uint32_t c0 = v0 % 10000;

            const uint32_t d1 = ( b0 / 100 ) << 1;
            const uint32_t d2 = ( b0 % 100 ) << 1;

            const uint32_t d3 = ( c0 / 100 ) << 1;
            const uint32_t d4 = ( c0 % 100 ) << 1;

            const uint32_t b1 = v1 / 10000;
            const uint32_t c1 = v1 % 10000;

            const uint32_t d5 = ( b1 / 100 ) << 1;
            const uint32_t d6 = ( b1 % 100 ) << 1;

            const uint32_t d7 = ( c1 / 100 ) << 1;
            const uint32_t d8 = ( c1 % 100 ) << 1;

            if( value >= 1000000000000000 )
                *buffer++ = gDigitsLut[ d1 ];
            if( value >= 100000000000000 )
                *buffer++ = gDigitsLut[ d1 + 1 ];
            if( value >= 10000000000000 )
                *buffer++ = gDigitsLut[ d2 ];
            if( value >= 1000000000000 )
                *buffer++ = gDigitsLut[ d2 + 1 ];
            if( value >= 100000000000 )
                *buffer++ = gDigitsLut[ d3 ];
            if( value >= 10000000000 )
                *buffer++ = gDigitsLut[ d3 + 1 ];
            if( value >= 1000000000 )
                *buffer++ = gDigitsLut[ d4 ];
            if( value >= 100000000 )
                *buffer++ = gDigitsLut[ d4 + 1 ];

            *buffer++ = gDigitsLut[ d5 ];
            *buffer++ = gDigitsLut[ d5 + 1 ];
            *buffer++ = gDigitsLut[ d6 ];
            *buffer++ = gDigitsLut[ d6 + 1 ];
            *buffer++ = gDigitsLut[ d7 ];
            *buffer++ = gDigitsLut[ d7 + 1 ];
            *buffer++ = gDigitsLut[ d8 ];
            *buffer++ = gDigitsLut[ d8 + 1 ];
        }
        else
        {
            const uint32_t a = static_cast<uint32_t>( value / 10000000000000000 ); // 1 to 1844
            value %= 10000000000000000;

            if( a < 10 )
                *buffer++ = '0' + static_cast<char>( a );
            else if( a < 100 )
            {
                const uint32_t i = a << 1;
                *buffer++ = gDigitsLut[ i ];
                *buffer++ = gDigitsLut[ i + 1 ];
            }
            else if( a < 1000 )
            {
                *buffer++ = '0' + static_cast<char>( a / 100 );

                const uint32_t i = ( a % 100 ) << 1;
                *buffer++ = gDigitsLut[ i ];
                *buffer++ = gDigitsLut[ i + 1 ];
            }
            else
            {
                const uint32_t i = ( a / 100 ) << 1;
                const uint32_t j = ( a % 100 ) << 1;
                *buffer++ = gDigitsLut[ i ];
                *buffer++ = gDigitsLut[ i + 1 ];
                *buffer++ = gDigitsLut[ j ];
                *buffer++ = gDigitsLut[ j + 1 ];
            }

            const uint32_t v0 = static_cast<uint32_t>( value / 100000000 );
            const uint32_t v1 = static_cast<uint32_t>( value % 100000000 );

            const uint32_t b0 = v0 / 10000;
            const uint32_t c0 = v0 % 10000;

            const uint32_t d1 = ( b0 / 100 ) << 1;
            const uint32_t d2 = ( b0 % 100 ) << 1;

            const uint32_t d3 = ( c0 / 100 ) << 1;
            const uint32_t d4 = ( c0 % 100 ) << 1;

            const uint32_t b1 = v1 / 10000;
            const uint32_t c1 = v1 % 10000;

            const uint32_t d5 = ( b1 / 100 ) << 1;
            const uint32_t d6 = ( b1 % 100 ) << 1;

            const uint32_t d7 = ( c1 / 100 ) << 1;
            const uint32_t d8 = ( c1 % 100 ) << 1;

            *buffer++ = gDigitsLut[ d1 ];
            *buffer++ = gDigitsLut[ d1 + 1 ];
            *buffer++ = gDigitsLut[ d2 ];
            *buffer++ = gDigitsLut[ d2 + 1 ];
            *buffer++ = gDigitsLut[ d3 ];
            *buffer++ = gDigitsLut[ d3 + 1 ];
            *buffer++ = gDigitsLut[ d4 ];
            *buffer++ = gDigitsLut[ d4 + 1 ];
            *buffer++ = gDigitsLut[ d5 ];
            *buffer++ = gDigitsLut[ d5 + 1 ];
            *buffer++ = gDigitsLut[ d6 ];
            *buffer++ = gDigitsLut[ d6 + 1 ];
            *buffer++ = gDigitsLut[ d7 ];
            *buffer++ = gDigitsLut[ d7 + 1 ];
            *buffer++ = gDigitsLut[ d8 ];
            *buffer++ = gDigitsLut[ d8 + 1 ];
        }

        //*buffer = '\0';
        return int( buffer - bak );
    }

    int i64toa_branchlut( int64_t value, char* buffer )
    {
        uint64_t u = static_cast<uint64_t>( value );
        if( value < 0 )
        {
            *buffer++ = '-';
            u = ~u + 1;
            return u64toa_branchlut( u, buffer ) + 1;
        }
        return u64toa_branchlut( u, buffer );
    }













    int ToStringCore( bool in, char * out )
    {
        if( in )
        {
            out[ 0 ] = 't';
            out[ 1 ] = 'r';
            out[ 2 ] = 'u';
            out[ 3 ] = 'e';
            return 4;
        }
        else
        {
            out[ 0 ] = 'f';
            out[ 1 ] = 'a';
            out[ 2 ] = 'l';
            out[ 3 ] = 's';
            out[ 4 ] = 'e';
            return 5;
        }
    }

    int ToStringCore( double in, char* out )
    {
        return dtoa_milo( in, out );
    }

    int ToStringCore( float in, char* out )
    {
        return dtoa_milo( in, out );
    }

    int ToString( char * dstBuf, uint8   v ) { return u32toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, uint16  v ) { return u32toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, uint    v ) { return u32toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, uint64  v ) { return u64toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, int8    v ) { return i32toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, int16   v ) { return i32toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, int     v ) { return i32toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, int64   v ) { return i64toa_branchlut( v, dstBuf ); }
    int ToString( char * dstBuf, double  v ) { return ToStringCore( v, dstBuf ); }
    int ToString( char * dstBuf, float   v ) { return ToStringCore( v, dstBuf ); }
    int ToString( char * dstBuf, bool    v ) { return ToStringCore( v, dstBuf ); }

    int ToString( char * dstBuf, char const* v )
    {
        auto len = strlen( v );
        memcpy( dstBuf, v, len );
        return (int)len;
    }

    int ToString( char * dstBuf, std::string const & v )
    {
        memcpy( dstBuf, v.c_str(), v.size() );
        return (int)v.size();
    }

    int ToString( char * dstBuf, String const & v )
    {
        memcpy( dstBuf, v.C_str(), v.Size() );
        return v.Size();
    }

    int ToString( char * dstBuf, char v )
    {
        *dstBuf = v;
        return 1;
    }










    static char const Int2HexTable[] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F'
    };

    int ToHexString( char * dstBuf, uint8 v )
    {
        dstBuf[ 0 ] = Int2HexTable[ v >> 4 ];
        dstBuf[ 1 ] = Int2HexTable[ v & 15 ];
        return 2;
    }

    int ToHexString( char * dstBuf, uint16 v )
    {
        ToHexString( dstBuf, uint8( v >> 8 ) );
        ToHexString( dstBuf + 2, (uint8)v );
        return 4;
    }

    int ToHexString( char * dstBuf, uint v )
    {
        ToHexString( dstBuf, uint16( v >> 16 ) );
        ToHexString( dstBuf + 4, (uint16)v );
        return 8;
    }

    int ToHexString( char * dstBuf, uint64 v )
    {
        ToHexString( dstBuf, uint32( v >> 32 ) );
        ToHexString( dstBuf + 8, (uint32)v );
        return 16;
    }


    int ToHexString( char * dstBuf, int8 v )
    {
        return ToHexString( dstBuf, (uint8)v );
    }

    int ToHexString( char * dstBuf, char v )
    {
        return ToHexString( dstBuf, (uint8)v );
    }

    int ToHexString( char * dstBuf, int16 v )
    {
        return ToHexString( dstBuf, (uint16)v );
    }

    int ToHexString( char * dstBuf, int v )
    {
        return ToHexString( dstBuf, (uint32)v );
    }

    int ToHexString( char * dstBuf, int64 v )
    {
        return ToHexString( dstBuf, (uint64)v );
    }





















    // string 转为各种长度的 有符号整数. Out 取值范围： int8~64
    template <typename OutType>
    void toInt( char const * in, OutType & out )
    {
        auto in_ = in;
        if( *in_ == '0' )
        {
            out = 0;
            return;
        }
        bool b;
        if( *in_ == '-' )
        {
            b = true;
            ++in_;
        }
        else b = false;
        OutType r = *in_ - '0';
        char c;
        while( ( c = *( ++in_ ) ) ) r = r * 10 + ( c - '0' );
        out = b ? -r : r;
    }

    // string (不能有减号打头) 转为各种长度的 无符号整数. Out 取值范围： uint8, uint16, uint32, uint64
    template <typename OutType>
    void toUInt( char const * in, OutType & out )
    {
        assert( in );
        auto in_ = in;
        if( *in_ == '0' )
        {
            out = 0;
            return;
        }
        OutType r = *(in_)-'0';
        char c;
        while( ( c = *( ++in_ ) ) ) r = r * 10 + ( c - '0' );
        out = r;
    }


    void FromStringCore( char const * in, uint8 & out )
    {
        toUInt( in, out );
    }


    void FromStringCore( char const * in, uint16 & out )
    {
        toUInt( in, out );
    }


    void FromStringCore( char const * in, uint32 & out )
    {
        toUInt( in, out );
    }


    void FromStringCore( char const * in, uint64 & out )
    {
        toUInt( in, out );
    }


    void FromStringCore( char const * in, int8 & out )
    {
        toInt( in, out );
    }


    void FromStringCore( char const * in, int16 & out )
    {
        toInt( in, out );
    }


    void FromStringCore( char const * in, int & out )
    {
        toInt( in, out );
    }


    void FromStringCore( char const * in, int64 & out )
    {
        toInt( in, out );
    }

    void FromStringCore( char const * in, double & out )
    {
        out = strtod( in, nullptr );
    }

    void FromStringCore( char const * in, float & out )
    {
        out = (float)strtod( in, nullptr );
    }


    void FromStringCore( char const * in, bool & out )
    {
        out = ( in[ 0 ] == '1' || in[ 0 ] == 'T' || in[ 0 ] == 't' );
    }

    void FromString( uint8   & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( uint16  & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( uint32  & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( uint64  & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( int8    & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( int16   & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( int     & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( int64   & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( double  & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( float   & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( bool    & dstVar, char const * s ) { FromStringCore( s, dstVar ); }
    void FromString( String  & dstVar, char const * s ) { dstVar = s; }












    int GetHash_CS( byte const* buf, int len )
    {
        assert( (size_t)buf % 4 == 0 );
        if( !len ) return 0;
        int n2 = 0x15051505, n1 = 0x15051505, mod = len % 8, i = 0;
        for( ; i < len - mod; i += 8 )
        {
            n1 = ( ( ( n1 << 5 ) + n1 ) + ( n1 >> 0x1b ) ) ^ *(int*)( buf + i );
            n2 = ( ( ( n2 << 5 ) + n2 ) + ( n2 >> 0x1b ) ) ^ *(int*)( buf + i + 4 );
        }
        if( mod > 4 )
        {
            n1 = ( ( ( n1 << 5 ) + n1 ) + ( n1 >> 0x1b ) ) ^ *(int*)( buf + i );
            n2 = ( ( ( n2 << 5 ) + n2 ) + ( n2 >> 0x1b ) ) ^ ( *(int*)( buf + i + 4 ) & ( 0xFFFFFFFF >> ( ( 8 - mod ) * 8 ) ) );
        }
        else if( mod )
        {
            n1 = ( ( ( n1 << 5 ) + n1 ) + ( n1 >> 0x1b ) ) ^ ( *(int*)( buf + i ) & ( 0xFFFFFFFF >> ( ( 4 - mod ) * 8 ) ) );
        }
        return n2 + n1 * 0x5d588b65;
    }

    int GetHash_Lua( byte const* buf, int len )
    {
        if( !len ) return 0;
        uint seed = 131, hash = 0;
        for( int i = 0; i < len; ++i )
            hash = hash * seed + (uint8)buf[ i ];
        return (int)hash;
    }



    static const int primes[] =
    {
        1, 2, 3, 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, 131071, 262139, 524287,
        1048573, 2097143, 4194301, 8388593, 16777213, 33554393, 67108859, 134217689, 268435399, 536870909, 1073741789
    };
    int GetPrime( int n )
    {
        return primes[ Calc2n( n ) ];
    }

    int GetHashCode( std::string const & in )
    {
        return GetHash_CS( (byte const*)in.c_str(), (int)in.size() );
    }

    bool EqualsTo( String* const& a, String* const& b )
    {
        return *a == *b;
    }





}
