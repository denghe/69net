#ifndef __UTILS_H__
#define __UTILS_H__

class String;

namespace Utils
{
    // 取长度系列，值为 toString 后的最大长. 10 进制。
    int getToStringMaxLength( uint8   v );
    int getToStringMaxLength( uint16  v );
    int getToStringMaxLength( uint    v );
    int getToStringMaxLength( uint64  v );
    int getToStringMaxLength( int8    v );
    int getToStringMaxLength( int16   v );
    int getToStringMaxLength( int     v );
    int getToStringMaxLength( int64   v );
    int getToStringMaxLength( double  v );
    int getToStringMaxLength( float   v );
    int getToStringMaxLength( bool    v );
    int getToStringMaxLength( char const* v );
    int getToStringMaxLength( String const & v );
    int getToStringMaxLength( std::string const & v );

    template<typename T>
    void getFillMaxLengthCore( int & len, T const & v )
    {
        len += getToStringMaxLength( v );
    }

    template<typename T, typename ...TS>
    void getFillMaxLengthCore( int & len, T const & v, TS const & ...vs )
    {
        getFillMaxLengthCore( len, v );
        getFillMaxLengthCore( len, vs... );
    }

    template<typename ...TS>
    int getFillMaxLength( TS const & ...vs )
    {
        int len = 0;
        getFillMaxLengthCore( len, vs... );
        return len;
    }






    // 将值类型转换为字符串 返回转换后的串长 (主要为 Append, Write 等函数服务)
    int toString( char * dstBuf, uint8   v );
    int toString( char * dstBuf, uint16  v );
    int toString( char * dstBuf, uint    v );
    int toString( char * dstBuf, uint64  v );
    int toString( char * dstBuf, int8    v );
    int toString( char * dstBuf, int16   v );
    int toString( char * dstBuf, int     v );
    int toString( char * dstBuf, int64   v );
    int toString( char * dstBuf, double  v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
    int toString( char * dstBuf, float   v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
    int toString( char * dstBuf, bool    v );

    // 一些便于写模板的补充
    int toString( char * dstBuf, char const* v );
    int toString( char * dstBuf, std::string const & v );
    int toString( char * dstBuf, String const & v );
    //template<int len>
    //int toString( char * dstBuf, char const ( &v )[ len ] )
    //{
    //    memcpy( dstBuf, v, len - 1 );
    //    return len - 1;
    //}

    

    template<typename T>
    void fillCore( char * & buf, int & offset, T const & v )
    {
        offset += toString( buf + offset, v );
    }

    template<typename T, typename ...TS>
    void fillCore( char * & buf, int & offset, T const & v, TS const & ...vs )
    {
        fillCore( buf, offset, v );
        fillCore( buf, offset, vs... );
    }

    // fill 主用于高速填充 char * buffer 拼字串, 但须提前预估足长度. 可 assert 返回值 < len
    template<typename ...TS>
    int fill( char * buf, TS const & ...vs )
    {
        int offset = 0;
        fillCore( buf, offset, vs... );
        buf[ offset ] = '\0';
        return offset;
    }



    // HEX 版
    //

    int toHexString( char * dstBuf, uint8   v );
    int toHexString( char * dstBuf, uint16  v );
    int toHexString( char * dstBuf, uint    v );
    int toHexString( char * dstBuf, uint64  v );
    // todo: byte[], string to hex
    int toHexString( char * dstBuf, int8   v );
    int toHexString( char * dstBuf, int16  v );
    int toHexString( char * dstBuf, int    v );
    int toHexString( char * dstBuf, int64  v );


    template<typename T>
    void fillHexCore( char * & buf, int & offset, T const & v )
    {
        offset += toHexString( buf + offset, v );
    }

    template<typename T, typename ...TS>
    void fillHexCore( char * & buf, int & offset, T const & v, TS const & ...vs )
    {
        fillHexCore( buf, offset, v );
        fillHexCore( buf, offset, vs... );
    }

    template<typename ...TS>
    int fillHex( char * buf, TS const & ...vs )
    {
        int offset = 0;
        fillHexCore( buf, offset, vs... );
        buf[ offset ] = '\0';
        return offset;
    }











    // 字串转值（值后面需要以 0 为结尾且不能有乱七八糟的 空格, TAB 等排版符）
    void fromString( uint8   & dstVar, char const * s );
    void fromString( uint16  & dstVar, char const * s );
    void fromString( uint32  & dstVar, char const * s );
    void fromString( uint64  & dstVar, char const * s );
    void fromString( int8    & dstVar, char const * s );
    void fromString( int16   & dstVar, char const * s );
    void fromString( int     & dstVar, char const * s );
    void fromString( int64   & dstVar, char const * s );
    void fromString( double  & dstVar, char const * s );
    void fromString( float   & dstVar, char const * s );
    void fromString( bool    & dstVar, char const * s );






    // calc lead zero （数 2 进制数高位头上的 0 的个数
#ifdef __GNUC__
    INLINE int clz( size_t x )
    {
        return __builtin_clz( x );
    }
#elif defined(__IA) && defined(_MSC_VER)
#include <intrin.h>
    INLINE int clz( size_t x )
    {
        unsigned long r = 0;
#ifdef __X64
        _BitScanReverse64( &r, x );
        return 63 - r;
#else
        _BitScanReverse( &r, x );
        return 31 - r;
#endif
    }
#else
#ifdef __X64
#error not support
#endif
    INLINE int popcnt( size_t x )
    {
        x -= ( ( x >> 1 ) & 0x55555555 );
        x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
        x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
        x += ( x >> 8 );
        x += ( x >> 16 );
        return x & 0x0000003f;
    }
    //INLINE int ctz( size_t x ) {
    //    return popcnt((x & -x) - 1);
    //}
    INLINE int clz( size_t x )
    {
        x |= ( x >> 1 );
        x |= ( x >> 2 );
        x |= ( x >> 4 );
        x |= ( x >> 8 );
        x |= ( x >> 16 );
        return 32 - popcnt( x );
    }
#endif


    // 返回刚好大于 x 的 2^n 的值用于内存分配
    // 如果 x 本身 2^n 系 则返回原值
    INLINE size_t round2n( size_t len )
    {
#ifdef __X64
        int bits = 63 - clz( len );
#else
        int bits = 31 - clz( len );
#endif
        size_t rtv = size_t( 1 ) << bits;
        if( rtv == len ) return len;
        return rtv << 1;
    }

    // 算一个数是 2 的几次方
    INLINE size_t calc2n( size_t len )
    {
#ifdef __X64
        return 63 - clz( len );
#else
        return 31 - clz( len );
#endif
    }






    template<typename T>
    bool isValueType()    // 整数，浮点，枚举，指针
    {
        return std::is_enum<T>::value || std::is_integral<T>::value || std::is_pointer<T>::value || std::is_floating_point<T>::value;
    }

}


#endif
