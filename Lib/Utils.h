#ifndef __UTILS_H__
#define __UTILS_H__

namespace xxx
{

    // 取长度系列，值为 ToString 后的最大长. 10 进制。
    int GetToStringMaxLength( uint8   v );
    int GetToStringMaxLength( uint16  v );
    int GetToStringMaxLength( uint    v );
    int GetToStringMaxLength( uint64  v );
#if __LINUX
    int GetToStringMaxLength( size_t   v );
#endif
    int GetToStringMaxLength( int8    v );
    int GetToStringMaxLength( int16   v );
    int GetToStringMaxLength( int     v );
    int GetToStringMaxLength( int64   v );
    int GetToStringMaxLength( double  v );
    int GetToStringMaxLength( float   v );
    int GetToStringMaxLength( bool    v );
    int GetToStringMaxLength( char    v );
    int GetToStringMaxLength( std::string const & v );
    int GetToStringMaxLength( char const* v );
    int GetToStringMaxLength( String const & v );







    // 将值类型转换为字符串 返回转换后的串长 (主要为 Append, Write 等函数服务)
    int ToString( char * dstBuf, uint8   v );
    int ToString( char * dstBuf, uint16  v );
    int ToString( char * dstBuf, uint    v );
    int ToString( char * dstBuf, uint64  v );
#if __LINUX
    int ToString( char * dstBuf, size_t  v );
#endif
    int ToString( char * dstBuf, int8    v );
    int ToString( char * dstBuf, int16   v );
    int ToString( char * dstBuf, int     v );
    int ToString( char * dstBuf, int64   v );
    int ToString( char * dstBuf, double  v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
    int ToString( char * dstBuf, float   v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
    int ToString( char * dstBuf, bool    v );
    int ToString( char * dstBuf, char    v );

    // 一些便于写模板的补充
    int ToString( char * dstBuf, char const* v );
    int ToString( char * dstBuf, std::string const & v );
    int ToString( char * dstBuf, String const & v );
    //template<int len>
    //int ToString( char * dstBuf, char const ( &v )[ len ] )
    //{
    //    memcpy( dstBuf, v, len - 1 );
    //    return len - 1;
    //}






    // HEX 版
    //

    int ToHexString( char * dstBuf, uint8   v );
    int ToHexString( char * dstBuf, uint16  v );
    int ToHexString( char * dstBuf, uint    v );
    int ToHexString( char * dstBuf, uint64  v );
    // todo: byte[], string to hex
    int ToHexString( char * dstBuf, char   v );
    int ToHexString( char * dstBuf, int8   v );
    int ToHexString( char * dstBuf, int16  v );
    int ToHexString( char * dstBuf, int    v );
    int ToHexString( char * dstBuf, int64  v );











    // 字串转值（值后面需要以 0 为结尾且不能有乱七八糟的 空格, TAB 等排版符）
    void FromString( uint8   & dstVar, char const * s );
    void FromString( uint16  & dstVar, char const * s );
    void FromString( uint32  & dstVar, char const * s );
    void FromString( uint64  & dstVar, char const * s );
    void FromString( int8    & dstVar, char const * s );
    void FromString( int16   & dstVar, char const * s );
    void FromString( int     & dstVar, char const * s );
    void FromString( int64   & dstVar, char const * s );
    void FromString( double  & dstVar, char const * s );
    void FromString( float   & dstVar, char const * s );
    void FromString( bool    & dstVar, char const * s );






    // calc lead zero （数 2 进制数高位头上的 0 的个数
#ifdef __GNUC__
    INLINE int Clz( size_t x )
    {
#ifdef __X64
        return __builtin_clzl( x );
#else
        return __builtin_clz( x );
#endif
    }
#elif defined(__IA) && defined(_MSC_VER)
#include <intrin.h>
    INLINE int Clz( size_t x )
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
    INLINE int Popcnt( size_t x )
    {
        x -= ( ( x >> 1 ) & 0x55555555 );
        x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
        x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
        x += ( x >> 8 );
        x += ( x >> 16 );
        return x & 0x0000003f;
    }
    //INLINE int ctz( size_t x ) {
    //    return Popcnt((x & -x) - 1);
    //}
    INLINE int Clz( size_t x )
    {
        x |= ( x >> 1 );
        x |= ( x >> 2 );
        x |= ( x >> 4 );
        x |= ( x >> 8 );
        x |= ( x >> 16 );
        return 32 - Popcnt( x );
    }
#endif


    // 返回刚好大于 x 的 2^n 的值用于内存分配
    // 如果 x 本身 2^n 系 则返回原值
    INLINE size_t Round2n( size_t len )
    {
#ifdef __X64
        int bits = 63 - Clz( len );
#else
        int bits = 31 - Clz( len );
#endif
        size_t rtv = size_t( 1 ) << bits;
        if( rtv == len ) return len;
        return rtv << 1;
    }

    // 算一个数是 2 的几次方
    INLINE size_t Calc2n( size_t len )
    {
#ifdef __X64
        return 63 - Clz( len );
#else
        return 31 - Clz( len );
#endif
    }








    int GetHash_CS( byte const* buf, int len );             // if arm, the buf must be align of 4(32bit)/8(64bit)
    int GetHash_Lua( byte const* buf, int len );            // if len <=4, will be faster than CS

    int GetHashCode( std::string const & in );
    template<typename T>
    int GetHashCode( T const &in );


    // 转指针比较为指针指向的内容比较
    template<typename T1, typename T2>
    bool EqualsTo( T1 const& a, T2 const& b );
    bool EqualsTo( String* const& a, String* const& b );



    // 得到刚好小于 n 的质数 主用于内存分配
    int GetPrime( int n );







    template<typename T, typename... Args>
    struct MaxSize
    {
        enum
        {
            value = sizeof( T ) > MaxSize<Args...>::value
            ? sizeof( T )
            : MaxSize<Args...>::value
        };
    };
    template<typename T>
    struct MaxSize < T >
    {
        enum { value = sizeof( T ) };
    };



}


#endif
