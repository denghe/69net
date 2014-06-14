#ifndef _HASHCODE_H_
#define _HASHCODE_H_

inline uint getHashCode( char const * buf, int len )
{
    uint seed = 131, hash = 0;
    for( int i = 0; i < len; ++i )
        hash = hash * seed + (uint8)buf[ i ];
    return hash;
}
inline uint getHashCode( String const & in )
{
    if ( in._len == 0 ) return 0;
    return getHashCode( in._buf, in._len );
}
inline uint getHashCode( Binary const & in )
{
    if ( in._len == 0 ) return 0;
    return getHashCode( in._buf, in._len );
}
template<typename T>
uint getHashCode( T const & in )
{
    if (sizeof(T) == 1) return ((uint8*)&in)[0];
    if (sizeof(T) == 2) return ((uint8*)&in)[0] || ( ((uint8*)&in)[1] << 8 );
    if (sizeof(T) == 3) return ((uint8*)&in)[0] || ( ((uint8*)&in)[1] << 8 ) || ( ((uint8*)&in)[2] << 16 );
    if (sizeof(T) == 4) return *(uint*)&in;
    return getHashCode( (char*)&in, sizeof(T) );     // ４字节以上，循环计算
}

#endif
