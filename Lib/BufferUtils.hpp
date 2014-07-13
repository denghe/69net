#ifndef _BUFFERUTILS_HPP__
#define _BUFFERUTILS_HPP__

namespace BufferUtils
{

    HAS_FUNC( getSize_checker, getBufferSize, int ( T::* )( ) const );
    template<typename T>
    typename std::enable_if<getSize_checker<T>::value, int>::type getSizeCore( T const& v )
    {
        return v.getBufferSize();
    };
    template<typename T>
    typename std::enable_if<!getSize_checker<T>::value, int>::type getSizeCore( T const& v )
    {
        return sizeof( T );
    };

    template<typename T>
    int getSize( T const& v )
    {
        return getSizeCore( v );
    }

    template<int len>
    int getSize( char const( &v )[ len ] )
    {
        return len - 1 + sizeof( int );
    }



    template<typename T>
    void getSizesCore( int& len, T const& v )
    {
        len += getSize( v );
    }

    template<typename T, typename ...TS>
    void getSizesCore( int& len, T const & v, TS const & ...vs )
    {
        getSizesCore( len, v );
        getSizesCore( len, vs... );
    }

    template<typename ...TS>
    int BufferUtils::getSizes( TS const & ...vs )
    {
        int len = 0;
        getSizesCore( len, vs... );
        return len;
    }




    template<typename T>
    void write( char* dest, T const& src )
    {
#ifdef __IA
        *(T*)dest = src;
#else
        auto p = (char*)&src;
        if( sizeof( T ) == 1 )
        {
            dest[ 0 ] = p[ 0 ];
        }
        if( sizeof( T ) == 2 )
        {
            dest[ 1 ] = p[ 1 ];
        }
        if( sizeof( T ) == 4 )
        {
            dest[ 2 ] = p[ 2 ];
            dest[ 3 ] = p[ 3 ];
        }
        if( sizeof( T ) == 8 )
        {
            dest[ 4 ] = p[ 4 ];
            dest[ 5 ] = p[ 5 ];
            dest[ 6 ] = p[ 6 ];
            dest[ 7 ] = p[ 7 ];
        }
#endif
    }




    template<typename T>
    void read( T& dest, char const* src )
    {
#ifdef __IA
        dest = *(T*)src;
#else
        auto p = (char*)&dest;
        if( sizeof( T ) == 1 )
        {
            p[ 0 ] = src[ 0 ];
        }
        if( sizeof( T ) == 2 )
        {
            p[ 1 ] = src[ 1 ];
        }
        if( sizeof( T ) == 4 )
        {
            p[ 2 ] = src[ 2 ];
            p[ 3 ] = src[ 3 ];
        }
        if( sizeof( T ) == 8 )
        {
            p[ 4 ] = src[ 4 ];
            p[ 5 ] = src[ 5 ];
            p[ 6 ] = src[ 6 ];
            p[ 7 ] = src[ 7 ];
        }
#endif
    }

}

#endif
