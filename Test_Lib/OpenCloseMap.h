#ifndef __OPENCLOSEMAP_H__
#define __OPENCLOSEMAP_H__

#include <cassert>
#include "PathNode.h"

template<typename T>
struct OpenCloseMap
{
    OpenCloseMap() = delete;
    OpenCloseMap( OpenCloseMap const& o ) = delete;
    OpenCloseMap& operator= ( OpenCloseMap const& o ) = delete;

    OpenCloseMap( int w, int h )
        : w( w )
        , h( h )
        , data( new T[ w * h ] )
    {
        Clear();
    }
    ~OpenCloseMap()
    {
        delete[] data;
        data = nullptr;
    }

    T& At( int x, int y )
    {
        return data[ y * w + x ];
    }

    void Add( T v )
    {
        assert( !At( v->x, v->y ) );
        At( v->x, v->y ) = v;
        ++c;
    }

    void Remove( T v )
    {
        assert( At( v->x, v->y ) == v );
        At( v->x, v->y ) = nullptr;
        --c;
    }

    bool Contains( T v )
    {
        auto o = At( v->x, v->y );
        if( o == nullptr ) return false;
        assert( o == v );
        return true;
    }

    void Clear()
    {
        memset( data, 0, w * h * sizeof( T ) );
        c = 0;
    }

//private:
    int w = 0, h = 0, c = 0;
    T* data = nullptr;
};


#endif
