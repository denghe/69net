#ifndef _BUFFERUTILS_H__
#define _BUFFERUTILS_H__

namespace xxx
{

    namespace BufferUtils
    {

        template<typename T>
        int GetSize( T const& v );

        template<int len>
        int GetSize( char const( &v )[ len ] );

        template<typename T, int len>
        int GetSize( T const( &v )[ len ] );


        template<typename T>
        void GetSizesCore( int& len, T const& v );

        template<typename T, typename ...TS>
        void GetSizesCore( int& len, T const & v, TS const & ...vs );

        template<typename ...TS>
        int GetSizes( TS const & ...vs );




        template<typename T>
        void Write( char* dest, T const& src );

        template<typename T, int len>
        void Write( char* dest, T const( &src )[ len ] );

        template<typename T>
        void Read( T& dest, char const* src );

        template<typename T, int len>
        void Read( T( &dest )[ len ], char const* src );



        void DumpCore( String & s, char const * buf, int len );

        void Dump( String & s, char const * buf, int len );

    }

}

#endif
