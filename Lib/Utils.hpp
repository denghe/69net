#ifndef __UTILS_HPP__
#define __UTILS_HPP__

namespace Utils
{

    template<typename T>
    int getHashCode( T const &in )
    {
        if( sizeof( T ) == 1 )
            return ( (byte*)&in )[ 0 ];
        if( sizeof( T ) == 2 )
            return ( (byte*)&in )[ 0 ] || ( ( (byte*)&in )[ 1 ] << 8 );
        if( sizeof( T ) == 3 )
            return ( (byte*)&in )[ 0 ] || ( ( (byte*)&in )[ 1 ] << 8 ) || ( ( (byte*)&in )[ 2 ] << 16 );
        if( sizeof( T ) == 4 )
            return *(int*)&in;
#if __IA
        return getHash_CS( (byte const*)&in, sizeof( T ) );
#else
        if( in.size() >= 4 && ( (size_t)in.c_str() % sizeof( size_t ) == 0 ) )
            return getHash_CS( (byte const*)&in, sizeof( T ) );
        else
            return getHash_Lua( (byte const*)&in, sizeof( T ) );
#endif
    }

    template<typename T1, typename T2>
    bool equalsTo( T1 const& a, T2 const& b )
    {
        return a == b;
    }

}

#endif
