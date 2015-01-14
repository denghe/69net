#ifndef __OPENCLOSEMAP_H__
#define __OPENCLOSEMAP_H__

#define USE_VECTOR_BOOL

#ifdef USE_VECTOR_BOOL
#include <vector>
#else
#include "Lib/All.h"
#endif

#include "PathNode.h"
#include <cassert>

struct OpenCloseMap
{
    OpenCloseMap() = delete;
    OpenCloseMap( OpenCloseMap const& o ) = delete;
    OpenCloseMap& operator= ( OpenCloseMap const& o ) = delete;

    OpenCloseMap( int w, int h )
        : w( w )
        , h( h )
    {
#ifdef USE_VECTOR_BOOL
        data.resize( w * h );
#else
        data.Resize( w * h, false );
#endif
    }
    bool Contains( int x, int y ) const
    {
        return data[ y * w + x ];
    }

    void Add( int x, int y )
    {
        assert( !Contains( x, y ) );
#ifdef USE_VECTOR_BOOL
        data[ y * w + x ] = true;
#else
        data.SetTrue( y * w + x );
#endif
        ++c;
    }

    void Remove( int x, int y )
    {
        assert( Contains( x, y ) );
#ifdef USE_VECTOR_BOOL
        data[ y * w + x ] = false;
#else
        data.SetFalse( y * w + x );
#endif
        --c;
    }

    void Clear()
    {
#ifdef USE_VECTOR_BOOL
        data.clear();
        data.resize( w * h );
#else
        data.FillFalse();
#endif
        c = 0;
    }

    //private:
    int w = 0, h = 0, c = 0;
#ifdef USE_VECTOR_BOOL
    std::vector<bool> data;
#else
    xxx::List<bool> data;
#endif
};


#endif
