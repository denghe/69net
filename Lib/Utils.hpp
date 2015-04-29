#ifndef __UTILS_HPP__
#define __UTILS_HPP__

namespace xxx
{
    HAS_FUNC( HasFunc_GetHashCode, GetHashCode, int ( T::* )( ) const );
    template<typename T>
    typename std::enable_if<HasFunc_GetHashCode<T>::value, int>::type GetHashCodeSwitch( T const& v )
    {
        return v.GetHashCode();
    };
    template<typename T>
    typename std::enable_if<!HasFunc_GetHashCode<T>::value, int>::type GetHashCodeSwitch( T const& v )
    {
        static_assert( std::is_pod<T>::value, "forget impl GetHashCode func ?" );
        if( std::is_pointer<T>::value )
        {
            return (int)(uint)( (size_t)(void*)v / ( sizeof(size_t) * 2 ) );    // 经验数据. 经测试发现 x64 下 vc new 至少是 16 的倍数, x86 至少是 8 的倍数
        }
        if( sizeof( T ) == 8 )
        {
            return (int)std::hash<uint64>()( *(uint64*)&v );
        }
        if( sizeof( T ) == 4 )
        {
            return (int)std::hash<uint32>()( *(uint32*)&v );
        }
        //            return (int)std::hash<T>()( v );
        return GetHash_CS( (byte const*)&v, sizeof( T ) );
    };

    template<typename T>
    int GetHashCode( T const &v )
    {
        return GetHashCodeSwitch( v );
    }

    template<typename T1, typename T2>
    bool EqualsTo( T1 const& a, T2 const& b )
    {
        return a == b;
    }

}

#endif
