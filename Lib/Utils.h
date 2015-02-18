#ifndef __UTILS_H__
#define __UTILS_H__

namespace xxx
{

    // ȡ����ϵ�У�ֵΪ ToString ������. 10 ���ơ�
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







    // ��ֵ����ת��Ϊ�ַ��� ����ת����Ĵ��� (��ҪΪ Append, Write �Ⱥ�������)
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

    // һЩ����дģ��Ĳ���
    int ToString( char * dstBuf, char const* v );
    int ToString( char * dstBuf, std::string const & v );
    int ToString( char * dstBuf, String const & v );
    //template<int len>
    //int ToString( char * dstBuf, char const ( &v )[ len ] )
    //{
    //    memcpy( dstBuf, v, len - 1 );
    //    return len - 1;
    //}






    // HEX ��
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











    // �ִ�תֵ��ֵ������Ҫ�� 0 Ϊ��β�Ҳ��������߰���� �ո�, TAB ���Ű����
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






    // calc lead zero ���� 2 ��������λͷ�ϵ� 0 �ĸ���
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


    // ���ظպô��� x �� 2^n ��ֵ�����ڴ����
    // ��� x ���� 2^n ϵ �򷵻�ԭֵ
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

    // ��һ������ 2 �ļ��η�
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


    // תָ��Ƚ�Ϊָ��ָ������ݱȽ�
    template<typename T1, typename T2>
    bool EqualsTo( T1 const& a, T2 const& b );
    bool EqualsTo( String* const& a, String* const& b );



    // �õ��պ�С�� n ������ �������ڴ����
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
