#ifndef _BUFFERUTILS_HPP__
#define _BUFFERUTILS_HPP__

namespace xxx
{
    namespace BufferUtils
    {

        HAS_FUNC( GetSize_checker, GetWriteBufferSize, int ( T::* )( ) const );
        template<typename T>
        typename std::enable_if<GetSize_checker<T>::value, int>::type GetSizeCore( T const& v )
        {
            return v.GetWriteBufferSize();
        };
        template<typename T>
        typename std::enable_if<!GetSize_checker<T>::value, int>::type GetSizeCore( T const& v )
        {
            static_assert( std::is_pod<T>::value, "forGet impl GetWriteBufferSize func ?" );
            return sizeof( T );
        };

        template<typename T>
        int GetSize( T const& v )
        {
            return GetSizeCore( v );
        }

        template<int len>
        int GetSize( char const( &v )[ len ] )
        {
            return len - 1 + sizeof( int );
        }


        template<typename T, int len>
        typename std::enable_if<GetSize_checker<T>::value, int>::type GetArraySizeCore( T const( &v )[ len ] )
        {
            int siz = 0;
            for( int i = 0; i < len; ++i )
            {
                siz += v[ i ].GetWriteBufferSize();
            }
            return siz;
        };
        template<typename T, int len>
        typename std::enable_if<!GetSize_checker<T>::value, int>::type GetArraySizeCore( T const( &v )[ len ] )
        {
            if( std::is_pod<T>::value )
            {
                return sizeof( T ) * len;
            }
            else
            {
                int siz = 0;
                for( int i = 0; i < len; ++i )
                {
                    siz += GetSize( v[ i ] );
                }
                return siz;
            }
        };
        template<typename T, int len>
        int GetSize( T const( &v )[ len ] )
        {
            return GetArraySizeCore( v );
        }


        template<typename T>
        void GetSizesCore( int& len, T const& v )
        {
            len += GetSize( v );
        }

        template<typename T, typename ...TS>
        void GetSizesCore( int& len, T const & v, TS const & ...vs )
        {
            GetSizesCore( len, v );
            GetSizesCore( len, vs... );
        }

        template<typename ...TS>
        int GetSizes( TS const & ...vs )
        {
            int len = 0;
            GetSizesCore( len, vs... );
            return len;
        }




        template<typename T>
        void Write( char* dest, T const& src )
        {
#ifdef __IA
            *(T*)dest = src;
#else
            auto p = (char*)&src;
            if( sizeof( T ) >= 1 )
            {
                dest[ 0 ] = p[ 0 ];
            }
            if( sizeof( T ) >= 2 )
            {
                dest[ 1 ] = p[ 1 ];
            }
            if( sizeof( T ) >= 4 )
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

        template<typename T, int len>
        void Write( char* dest, T const( &src )[ len ] )
        {
            for( int i = 0; i < len; ++i )
            {
                Write( dest + sizeof( T ) * i, src[ i ] );
            }
        }





        template<typename T>
        void Read( T& dest, char const* src )
        {
#ifdef __IA
            dest = *(T*)src;
#else
            auto p = (char*)&dest;
            if( sizeof( T ) >= 1 )
            {
                p[ 0 ] = src[ 0 ];
            }
            if( sizeof( T ) >= 2 )
            {
                p[ 1 ] = src[ 1 ];
            }
            if( sizeof( T ) >= 4 )
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

        template<typename T, int len>
        void Read( T( &dest )[ len ], char const* src )
        {
            for( int i = 0; i < len; ++i )
            {
                Read( dest[ i ], src + sizeof( T ) * i );
            }
        }



    }
}

#endif
