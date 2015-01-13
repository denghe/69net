#ifndef __MAP_H__
#define __MAP_H__

#include <vector>

template<typename T>
struct Map
{
    Map() = delete;
    Map( Map const& o ) = delete;
    Map& operator= ( Map const& o ) = delete;

    Map( int w, int h )
        : w( w )
        , h( h )
    {
        data.resize( w * h );
    }
    T& At( int x, int y )
    {
        assert( x >= 0 && y >= 0 && x < w && y < h );
        return data[ y * w + x ];
    }
    void Clear()
    {
        memset( data.data(), 0, w * h * sizeof( T ) );
    }

//private:
    int w = 0, h = 0;
    std::vector<T> data;
};


#endif
