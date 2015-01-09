#ifndef __MAP_H__
#define __MAP_H__

#include <functional>

template<typename T>
struct Map
{
    Map() = delete;
    Map( Map const& o ) = delete;
    Map& operator= ( Map const& o ) = delete;

    Map( int w, int h )
        : w( w )
        , h( h )
        , data( new T[ w * h ] )
    {
    }
    ~Map()
    {
        delete[] data;
        data = nullptr;
    }

    T& At( int x, int y )
    {
        return data[ y * w + x ];
    }

    void All( std::function<void( T& )> handler )
    {
        for( int i = 0; i < w * h; ++i )
        {
            handler( data[ i ] );
        }
    }

//private:
    int w = 0, h = 0;
    T* data = nullptr;
};

#endif
