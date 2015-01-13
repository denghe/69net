#ifndef __OPENCLOSEMAP_H__
#define __OPENCLOSEMAP_H__

#include <cassert>
#include <vector>
#include "PathNode.h"

struct OpenCloseMap
{
    OpenCloseMap() = delete;
    OpenCloseMap( OpenCloseMap const& o ) = delete;
    OpenCloseMap& operator= ( OpenCloseMap const& o ) = delete;

    OpenCloseMap( int w, int h )
        : w( w )
        , h( h )
    {
        data.resize( w * h );
    }
    bool Contains( int x, int y ) const
    {
        return data[ y * w + x ];
    }

    void Add( int x, int y )
    {
        assert( !At( x, y ) );
        data[ y * w + x ] = true;
        ++c;
    }

    void Remove( int x, int y )
    {
        assert( At( x, y ) );
        data[ y * w + x ] = false;
        --c;
    }

    void Clear()
    {
        data.clear();
        data.resize( w * h );
        c = 0;
    }

    //private:
    int w = 0, h = 0, c = 0;
    std::vector<bool> data;
};


#endif
