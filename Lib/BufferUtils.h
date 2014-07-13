#ifndef _BUFFERUTILS_H__
#define _BUFFERUTILS_H__

namespace BufferUtils
{

    template<typename T>
    int getSize( T const& v );

    template<int len>
    int getSize( char const( &v )[ len ] );




    template<typename T>
    void getSizesCore( int& len, T const& v );

    template<typename T, typename ...TS>
    void getSizesCore( int& len, T const & v, TS const & ...vs );

    template<typename ...TS>
    int getSizes( TS const & ...vs );




    template<typename T>
    void write( char* dest, T const& src );

    template<typename T>
    void read( T& dest, char const* src );




    void dumpCore( String & s, char const * buf, int len );

    void dump( String & s, char const * buf, int len );

}

#endif
