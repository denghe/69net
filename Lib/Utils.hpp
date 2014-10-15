#ifndef __UTILS_HPP__
#define __UTILS_HPP__

namespace xxx
{
    HAS_FUNC( GetHashCode_checker, GetHashCode, int ( T::* )( ) const );
    template<typename T>
    typename std::enable_if<GetHashCode_checker<T>::value, int>::type GetHashCodeCore( T const& v )
    {
        return v.GetHashCode();
    };
    template<typename T>
    typename std::enable_if<!GetHashCode_checker<T>::value, int>::type GetHashCodeCore( T const& in )
    {
        static_assert( std::is_pod<T>::value, "forGet impl GetHashCode func ?" );
        if( sizeof( T ) == 1 )
            return ( (byte*)&in )[ 0 ];
        if( sizeof( T ) == 2 )
            return ( (byte*)&in )[ 0 ] || ( ( (byte*)&in )[ 1 ] << 8 );
        if( sizeof( T ) == 3 )
            return ( (byte*)&in )[ 0 ] || ( ( (byte*)&in )[ 1 ] << 8 ) || ( ( (byte*)&in )[ 2 ] << 16 );
        if( sizeof( T ) == 4 )
            return *(int*)&in;
        if( sizeof( T ) == 8 )
        {
            uint* p = (uint*)&in;
            return (int)( p[ 0 ] + p[ 1 ] );
        }
        return GetHash_CS( (byte const*)&in, sizeof( T ) );
    };



    template<typename T>
    int GetHashCode( T const &in )
    {
        auto rtv = GetHashCodeCore( in );
        return rtv;
        //return GetHashCodeCore( in );
    }

    template<typename T1, typename T2>
    bool EqualsTo( T1 const& a, T2 const& b )
    {
        return a == b;
    }

}

#endif
