#ifndef __OPENCLOSEMAP_H__
#define __OPENCLOSEMAP_H__

#include <cassert>
#include <vector>
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
        //Clear();
        memset( data, 0, w * h * sizeof( T ) );
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


    // 试优化，用一个 vector 来记，以便 Clear 时只清这些
    std::vector<T*> needClears;

    void Add( T v )
    {
        assert( !At( v->x, v->y ) );
        At( v->x, v->y ) = v;
        ++c;
        needClears.push_back( &data[ v->y * w + v->x ] );
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
        if( !c )
        {
            needClears.clear();
            return;
        }

        if( needClears.size() < w * h / 2 )
        {
            for( int i = 0; i < needClears.size(); ++i )
            {
                *needClears[ i ] = nullptr;
            }
        }
        else
        {
            memset( data, 0, w * h * sizeof( T ) );
            //for( int i = 0; i < w * h; ++i )
            //{
            //    data[ i ] = nullptr;
            //}
        }
        needClears.clear();

        c = 0;
    }

    //private:
    int w = 0, h = 0, c = 0;
    T* data = nullptr;
};


#endif
