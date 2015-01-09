#ifndef __OPENCLOSEMAP_H__
#define __OPENCLOSEMAP_H__

#include <cassert>
#include "PathNode.h"

template<typename UCT>
struct OpenCloseMap
{
    typedef PathNode<UCT>* NT;

    OpenCloseMap() = delete;
    OpenCloseMap( OpenCloseMap const& o ) = delete;
    OpenCloseMap& operator= ( OpenCloseMap const& o ) = delete;

    OpenCloseMap( int w, int h )
        : w( w )
        , h( h )
        , data( new NT[ w * h ] )
    {
        Clear();
    }
    ~OpenCloseMap()
    {
        delete[] data;
        data = nullptr;
    }

    NT& At( int x, int y )
    {
        return data[ y * w + x ];
    }

    void Add( NT v )
    {
        assert( !At( v->x, v->y ) );
        At( v->x, v->y ) = v;
        ++c;
    }

    void Remove( NT v )
    {
        assert( At( v->x, v->y ) == v );
        At( v->x, v->y ) = nullptr;
        --c;
    }

    bool Contains( NT v )
    {
        auto o = At( v->x, v->y );
        if( o == nullptr ) return false;
        assert( o == v );
        return true;
    }

    void Clear()
    {
        memset( data, 0, w * h * sizeof( NT ) );
        c = 0;
    }

//private:
    int w = 0, h = 0, c = 0;
    NT* data = nullptr;
};


#endif
