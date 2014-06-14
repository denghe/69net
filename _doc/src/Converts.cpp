#include "Precompile.h"
#include "ftoa.c"

namespace Converts
{



// 取 整数 转换后的 string 长度系列
// todo: if ( n < ...... ) 这种代码理论上讲可以优化成树形, 类似折半查找从而减少 if 次数

int getToStringSize( uint8 v )
{
    if     ( v < 10 ) return 1;
    else if( v < 100 ) return 2;
    else return 3;
}

int getToStringSize( uint16 v )
{
    if     ( v < 10 ) return 1;
    else if( v < 100 ) return 2;
    else if( v < 1000 ) return 3;
    else if( v < 10000 ) return 4;
    else return 5;
}

int getToStringSize( uint32 v )
{
    if     ( v < 10 ) return 1;
    else if( v < 100 ) return 2;
    else if( v < 1000 ) return 3;
    else if( v < 10000 ) return 4;
    else if( v < 100000 ) return 5;
    else if( v < 1000000 ) return 6;
    else if( v < 10000000 ) return 7;
    else if( v < 100000000 ) return 8;
    else if( v < 1000000000 ) return 9;
    else return 10;
}

int getToStringSize( uint64 v )
{
    if     ( v < 10000000000LL ) return getToStringSize((uint32)v);
    else if( v < 100000000000LL ) return 11;
    else if( v < 1000000000000LL ) return 12;
    else if( v < 10000000000000LL ) return 13;
    else if( v < 100000000000000LL ) return 14;
    else if( v < 1000000000000000LL ) return 15;
    else if( v < 10000000000000000LL ) return 16;
    else if( v < 100000000000000000LL ) return 17;
    else if( v < 1000000000000000000LL ) return 18;
    else if( v < (uint64)((uint64)1000000000000000000LL * (uint64)10)) return 19;
    else return 20;
}


int toStringCore( uint8 in, char *out )
{
    if( in == 0 )
    {
        out[ 0 ] = '0';
        return 1;
    }

    int idx, len;
    idx = len = getToStringSize( in );
    size_t v = in, tmp;
    do
    {
#if __X64
        tmp = ( v * 1717986919 ) >> 34;			// v / 10 的高效率版
#else
        tmp = v / 10;
#endif
        out[ --idx ] = (char)( 48 + v - tmp * 10 );
    }
    while(( v = tmp ));
    return len;
}


int toStringCore( uint16 in, char *out )
{
    if( in == 0 )
    {
        out[ 0 ] = '0';
        return 1;
    }

    int idx, len;
    idx = len = getToStringSize( in );
    size_t v = in, tmp;
    do
    {
#if __X64
        tmp = ( v * 1717986919 ) >> 34;			// v / 10 的高效率版
#else
        tmp = v / 10;
#endif
        out[ --idx ] = (char)( 48 + v - tmp * 10 );
    }
    while(( v = tmp ));
    return len;
}



int toStringCore( uint32 in, char *out )
{
    if( in == 0 )
    {
        out[ 0 ] = '0';
        return 1;
    }

    int idx, len;
    idx = len = getToStringSize( in );
    size_t v = in, tmp;
    do
    {
#if __X64
        tmp = ( v * 1717986919 ) >> 34;			// v / 10 的高效率版
#else
        tmp = v / 10;
#endif
        out[ --idx ] = (char)( 48 + v - tmp * 10 );
    }
    while(( v = tmp ));
    return len;
}


int toStringCore( uint64 in, char * out )
{
    if( in == 0 )
    {
        out[ 0 ] = '0';
        return 1;
    }
    if( in <= 0xFFFFFFFFLL ) return toStringCore( (uint32)in, out );
    int len, idx;
    idx = len = getToStringSize( in );
    size_t v = (size_t)in, tmp;
    do
    {
        tmp = v / 10;
        out[ --idx ] = (char)( 48 + v - tmp * 10 );
    }
    while(( v = tmp ));
    return len;
}


int toStringCore( int in, char * out )
{
    if( in < 0 )
    {
        *out = '-';
        return toStringCore( (uint32)-in, out + 1 ) + 1;
    }
    return toStringCore( (uint32)in, out );
}


int toStringCore( int64 in, char * out )
{
    if( in < 0 )
    {
        *out = '-';
        return toStringCore( (uint64)-in, out + 1 ) + 1;
    }
    return toStringCore( (uint64)in, out );
}


int toStringCore( bool in, char * out )
{
    if( in )
    {
        out[0] = 't';
        out[1] = 'r';
        out[2] = 'u';
        out[3] = 'e';
        return 4;
    }
    else
    {
        out[0] = 'f';
        out[1] = 'a';
        out[2] = 'l';
        out[3] = 's';
        out[4] = 'e';
        return 5;
    }
}

int toStringCore( double in, char * out, int maxlen, int precision, bool delEndZeros )
{
    int ret = lftoa( in, out, maxlen, precision );
    if( !delEndZeros ) return ret;
    auto lastPos = out + ret;
    auto dotPos = std::find( out, lastPos, '.' );
    if( dotPos == lastPos ) return ret;
    auto p = lastPos - 1;
    for( ; p > dotPos ; --p ) if( *p != '0' ) break;
    ret -= int(lastPos - 1 - p);
    if( out[ ret - 1 ] == '.' ) --ret;
    return ret;
}

int toStringCore( float in, char * out, int maxlen, int precision, bool delEndZeros )
{
    int ret = ftoa( in, out, maxlen, precision );
    if( !delEndZeros ) return ret;
    auto lastPos = out + ret;
    auto dotPos = std::find( out, lastPos, '.' );
    if( dotPos == lastPos ) return ret;
    auto p = lastPos - 1;
    for( ; p > dotPos ; --p ) if( *p != '0' ) break;
    ret -= int(lastPos - 1 - p);
    if( out[ ret - 1 ] == '.' ) --ret;
    return ret;
}











// string 转为各种长度的 有符号整数. Out 取值范围： int8~64
template <typename OutType>
void toInt( char const * in, OutType & out )
{
    auto in_ = in;
    if( *in_ == '0' ) {
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
    while(( c = *(++in_) )) r = r * 10 + ( c - '0' );
    out = b ? -r : r;
}

// string (不能有减号打头) 转为各种长度的 无符号整数. Out 取值范围： uint8, uint16, uint32, uint64
template <typename OutType>
void toUInt( char const * in, OutType & out )
{
    assert ( in );
    auto in_ = in;
    if( *in_ == '0' ) {
        out = 0;
        return;
    }
    OutType r = *(in_) - '0';
    char c;
    while(( c = *(++in_) )) r = r * 10 + ( c - '0' );
    out = r;
}


void fromStringCore( char const * in, uint8 & out )
{
    toUInt( in, out );
}


void fromStringCore( char const * in, uint16 & out )
{
    toUInt( in, out );
}


void fromStringCore( char const * in, uint32 & out )
{
    toUInt( in, out );
}


void fromStringCore( char const * in, uint64 & out )
{
    toUInt( in, out );
}


void fromStringCore( char const * in, int8 & out )
{
    toInt( in, out );
}


void fromStringCore( char const * in, int16 & out )
{
    toInt( in, out );
}


void fromStringCore( char const * in, int & out )
{
    toInt( in, out );
}


void fromStringCore( char const * in, int64 & out )
{
    toInt( in, out );
}

void fromStringCore( char const * in, double & out )
{
    out = strtod(in, nullptr);
}

void fromStringCore( char const * in, float & out ) 
{
    out = (float)strtod(in, nullptr);
}


void fromStringCore( char const * in, bool & out )
{
    out = (in[0] == '1' || in[0] == 'T' || in[0] == 't');
}







int toString( char * dstBuf, uint8   v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, uint16  v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, uint    v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, uint64  v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, int8    v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, int16   v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, int     v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, int64   v ) { return toStringCore( v, dstBuf ); }
int toString( char * dstBuf, double  v, int maxlen, int precision, bool delEndZeros ) { return toStringCore( v, dstBuf, maxlen, precision, delEndZeros ); }
int toString( char * dstBuf, float   v, int maxlen, int precision, bool delEndZeros ) { return toStringCore( v, dstBuf, maxlen, precision, delEndZeros ); }
int toString( char * dstBuf, bool    v ) { return toStringCore( v, dstBuf ); }


void fromString( uint8   & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( uint16  & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( uint32  & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( uint64  & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( int8    & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( int16   & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( int     & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( int64   & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( double  & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( float   & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }
void fromString( bool    & dstVar, char const * s ) { return fromStringCore( s, dstVar ); }


































uint32 toStringHexCore( uint8 in, char * out )
{
    auto &p = *(BitMask::UInt8*)&in;
    out[ 0 ] = Int2HexTable[ p.Byte0_H ];
    out[ 1 ] = Int2HexTable[ p.Byte0_L ];
    return sizeof(in) * 2;
}


uint32 toStringHexCore( uint16 in, char * out )
{
    auto &p = *(BitMask::UInt16*)&in;
    out[ 0 ] = Int2HexTable[ p.Byte1_H ];
    out[ 1 ] = Int2HexTable[ p.Byte1_L ];
    out[ 2 ] = Int2HexTable[ p.Byte0_H ];
    out[ 3 ] = Int2HexTable[ p.Byte0_L ];
    return sizeof(in) * 2;
}


uint32 toStringHexCore( uint32 in, char * out )
{
    auto &p = *(BitMask::UInt32*)&in;
    out[ 0 ] = Int2HexTable[ p.Byte3_H ];
    out[ 1 ] = Int2HexTable[ p.Byte3_L ];
    out[ 2 ] = Int2HexTable[ p.Byte2_H ];
    out[ 3 ] = Int2HexTable[ p.Byte2_L ];
    out[ 4 ] = Int2HexTable[ p.Byte1_H ];
    out[ 5 ] = Int2HexTable[ p.Byte1_L ];
    out[ 6 ] = Int2HexTable[ p.Byte0_H ];
    out[ 7 ] = Int2HexTable[ p.Byte0_L ];
    return sizeof(in) * 2;
}


uint32 toStringHexCore( uint64 in, char * out )
{
    auto &p = *(BitMask::UInt64*)&in;
    out[ 0  ] = Int2HexTable[ p.Byte7_H ];
    out[ 1  ] = Int2HexTable[ p.Byte7_L ];
    out[ 2  ] = Int2HexTable[ p.Byte6_H ];
    out[ 3  ] = Int2HexTable[ p.Byte6_L ];
    out[ 4  ] = Int2HexTable[ p.Byte5_H ];
    out[ 5  ] = Int2HexTable[ p.Byte5_L ];
    out[ 6  ] = Int2HexTable[ p.Byte4_H ];
    out[ 7  ] = Int2HexTable[ p.Byte4_L ];
    out[ 8  ] = Int2HexTable[ p.Byte3_H ];
    out[ 9  ] = Int2HexTable[ p.Byte3_L ];
    out[ 10 ] = Int2HexTable[ p.Byte2_H ];
    out[ 11 ] = Int2HexTable[ p.Byte2_L ];
    out[ 12 ] = Int2HexTable[ p.Byte1_H ];
    out[ 13 ] = Int2HexTable[ p.Byte1_L ];
    out[ 14 ] = Int2HexTable[ p.Byte0_H ];
    out[ 15 ] = Int2HexTable[ p.Byte0_L ];
    return sizeof(in) * 2;
}




void fromStringHexCore( char const * in, uint8 & out )
{
    auto &p = *(BitMask::UInt8*)&out;
    p.Byte0_H = Hex2IntTable[ (int)in[ 0 ] ];
    p.Byte0_L = Hex2IntTable[ (int)in[ 1 ] ];
}


void fromStringHexCore( char const * in, uint16 & out )
{
    auto &p = *(BitMask::UInt16*)&out;
    p.Byte1_H = Hex2IntTable[ (int)in[ 0 ] ];
    p.Byte1_L = Hex2IntTable[ (int)in[ 1 ] ];
    p.Byte0_H = Hex2IntTable[ (int)in[ 2 ] ];
    p.Byte0_L = Hex2IntTable[ (int)in[ 3 ] ];
}


void fromStringHexCore( char const * in, uint32 & out )
{
    auto &p = *(BitMask::UInt32*)&out;
    p.Byte3_H = Hex2IntTable[ (int)in[ 0 ] ];
    p.Byte3_L = Hex2IntTable[ (int)in[ 1 ] ];
    p.Byte2_H = Hex2IntTable[ (int)in[ 2 ] ];
    p.Byte2_L = Hex2IntTable[ (int)in[ 3 ] ];
    p.Byte1_H = Hex2IntTable[ (int)in[ 4 ] ];
    p.Byte1_L = Hex2IntTable[ (int)in[ 5 ] ];
    p.Byte0_H = Hex2IntTable[ (int)in[ 6 ] ];
    p.Byte0_L = Hex2IntTable[ (int)in[ 7 ] ];
}


void fromStringHexCore( char const * in, uint64 & out )
{
    auto &p = *(BitMask::UInt64*)&out;
    p.Byte7_H = Hex2IntTable[ (int)in[ 0  ] ];
    p.Byte7_L = Hex2IntTable[ (int)in[ 1  ] ];
    p.Byte6_H = Hex2IntTable[ (int)in[ 2  ] ];
    p.Byte6_L = Hex2IntTable[ (int)in[ 3  ] ];
    p.Byte5_H = Hex2IntTable[ (int)in[ 4  ] ];
    p.Byte5_L = Hex2IntTable[ (int)in[ 5  ] ];
    p.Byte4_H = Hex2IntTable[ (int)in[ 6  ] ];
    p.Byte4_L = Hex2IntTable[ (int)in[ 7  ] ];
    p.Byte3_H = Hex2IntTable[ (int)in[ 8  ] ];
    p.Byte3_L = Hex2IntTable[ (int)in[ 9  ] ];
    p.Byte2_H = Hex2IntTable[ (int)in[ 10 ] ];
    p.Byte2_L = Hex2IntTable[ (int)in[ 11 ] ];
    p.Byte1_H = Hex2IntTable[ (int)in[ 12 ] ];
    p.Byte1_L = Hex2IntTable[ (int)in[ 13 ] ];
    p.Byte0_H = Hex2IntTable[ (int)in[ 14 ] ];
    p.Byte0_L = Hex2IntTable[ (int)in[ 15 ] ];
}










int toStringHex( char * dstBuf, uint8   v ) { return toStringHexCore( v, dstBuf );            }
int toStringHex( char * dstBuf, uint16  v ) { return toStringHexCore( v, dstBuf );            }
int toStringHex( char * dstBuf, uint    v ) { return toStringHexCore( v, dstBuf );            }
int toStringHex( char * dstBuf, uint64  v ) { return toStringHexCore( v, dstBuf );            }
int toStringHex( char * dstBuf, int8    v ) { return toStringHexCore( (uint8)v, dstBuf );     }
int toStringHex( char * dstBuf, int16   v ) { return toStringHexCore( (uint16)v, dstBuf );    }
int toStringHex( char * dstBuf, int     v ) { return toStringHexCore( (uint32)v, dstBuf );    }
int toStringHex( char * dstBuf, int64   v ) { return toStringHexCore( (uint64)v, dstBuf );    }
int toStringHex( char * dstBuf, double  v ) { return toStringHexCore( *(uint64*)&v, dstBuf ); }
int toStringHex( char * dstBuf, float   v ) { return toStringHexCore( *(uint32*)&v, dstBuf ); }
int toStringHex( char * dstBuf, bool    v ) { return toStringHexCore( *(uint8*)&v, dstBuf );  }


void fromStringHex( uint8   & dstVar, char const * s ) { return fromStringHexCore( s, dstVar );             }
void fromStringHex( uint16  & dstVar, char const * s ) { return fromStringHexCore( s, dstVar );             }
void fromStringHex( uint32  & dstVar, char const * s ) { return fromStringHexCore( s, dstVar );             }
void fromStringHex( uint64  & dstVar, char const * s ) { return fromStringHexCore( s, dstVar );             }
void fromStringHex( int8    & dstVar, char const * s ) { return fromStringHexCore( s, *(uint8*)&dstVar );   }
void fromStringHex( int16   & dstVar, char const * s ) { return fromStringHexCore( s, *(uint16*)&dstVar );  }
void fromStringHex( int     & dstVar, char const * s ) { return fromStringHexCore( s, *(uint*)&dstVar );    }
void fromStringHex( int64   & dstVar, char const * s ) { return fromStringHexCore( s, *(uint64*)&dstVar );  }
void fromStringHex( double  & dstVar, char const * s ) { return fromStringHexCore( s, *(uint64*)&dstVar );  }
void fromStringHex( float   & dstVar, char const * s ) { return fromStringHexCore( s, *(uint*)&dstVar );    }
void fromStringHex( bool    & dstVar, char const * s ) { return fromStringHexCore( s, *(uint8*)&dstVar );   }











int toStringUtf8( char * dstBuf, wchar_t const * s )
{
    assert( s && dstBuf );
    auto s_ = s;
    int idx = 0;
    while( auto c = *s_++ )
    {
        if( c < 0x80 ) dstBuf[ idx++ ] = (uint8)c;              // 0xxx xxxx
        else if( c < 0x800 )
        {                                                       // 110x xxxx 10xx xxxx
            dstBuf[ idx + 0 ] = ( ( c >> 6 ) & 0x1F ) | 0xC0;   // 1f: 0001 1111    c0: 1100 0000
            dstBuf[ idx + 1 ] = ( c & 0x3F ) | 0x80;            // 3f: 0011 1111    80: 1000 0000
            idx += 2;
        }
        else
        {                                                       // 1110 xxxx 10xx xxxx 10xx xxxx
            dstBuf[ idx + 0 ] = ( ( c >> 12 ) & 0x0F ) | 0xE0;  // 0f: 0000 1111    e0: 1110 0000
            dstBuf[ idx + 1 ] = ( ( c >> 6 ) & 0x3F ) | 0x80;   // 3f: 0011 1111    80: 1000 0000
            dstBuf[ idx + 2 ] = ( c & 0x3F ) | 0x80;            // 3f: 0011 1111    80: 1000 0000
            idx += 3;
        }
    }
    return idx;
}

int toStringUtf8( char * dstBuf, wchar_t const * s, int len )
{
    assert( s && dstBuf );
    int idx = 0, i = 0;
    do
    {
        wchar_t c = s[ i++ ];
        if( c < 0x80 ) dstBuf[ idx++ ] = (uint8)c;
        else if( c < 0x800 )
        {
            dstBuf[ idx + 0 ] = ( ( c >> 6 ) & 0x1F ) | 0xC0;
            dstBuf[ idx + 1 ] = ( c & 0x3F ) | 0x80;
            idx += 2;
        }
        else
        {
            dstBuf[ idx + 0 ] = ( ( c >> 12 ) & 0x0F ) | 0xE0;
            dstBuf[ idx + 1 ] = ( ( c >> 6 ) & 0x3F ) | 0x80;
            dstBuf[ idx + 2 ] = ( c & 0x3F ) | 0x80;
            idx += 3;
        }
    } while( i < len );
    return idx;
}

int toStringUtf8( char * dstBuf, wchar_t const & s )
{
    assert( dstBuf );
    int idx = 0;
    if( s < 0x80 ) dstBuf[ idx++ ] = (uint8)s;
    else if( s < 0x800 )
    {
        dstBuf[ idx + 0 ] = ( ( s >> 6 ) & 0x1F ) | 0xC0;
        dstBuf[ idx + 1 ] = ( s & 0x3F ) | 0x80;
        idx += 2;
    }
    else
    {
        dstBuf[ idx + 0 ] = ( ( s >> 12 ) & 0x0F ) | 0xE0;
        dstBuf[ idx + 1 ] = ( ( s >> 6 ) & 0x3F ) | 0x80;
        dstBuf[ idx + 2 ] = ( s & 0x3F ) | 0x80;
        idx += 3;
    }
    return idx;
}

int toStringUnicode( wchar_t * dstBuf, char const * s, int len )
{
    assert( s && dstBuf );
    auto outWStr_ = dstBuf;
    auto bak = dstBuf;
    size_t i = 0;
    do
    {
        uint8 c = s[ i++ ];
        if( c < 0x80u ) *outWStr_++ = c;
        else if( ( c & 0xE0 ) == 0xE0 )
        {
            *outWStr_++ = ( ( c & 0x0F ) << 12 ) | ( ( ( s[ i ] ) & 0x3F ) << 6 ) | ( ( s[ i + 1 ] ) & 0x3F );
            i += 2;
        }
        else if( ( c & 0xC0u ) == 0xC0u )
        {
            *outWStr_++ = ( ( c & 0x1F ) << 6 ) | ( s[ i++ ] & 0x3F );
        }
    } while( i < len );
    return (int)( outWStr_ - bak );
}












}
