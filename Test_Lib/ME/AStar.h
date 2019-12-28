#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <cmath>
#include <cassert>
#include <vector>
#include "PriorityQueue.h"
#include "OpenCloseMap.h"
#include "Map.h"
#include "PathNode.h"

template<typename T>
struct AStar
{
    typedef PathNode<T*>    PNT;
    typedef PathNode<T*>*   PPNT;

    std::vector<PPNT>       searchResults;              // Search 函数的执行结果
    Map<PNT>                pathNodeMap;                // 通过原地图数据填充，扩展出计算字段
    PriorityQueue<PPNT>     orderedOpenSet;
    Map<PPNT>               cameFrom;
    OpenCloseMap            closedSet;
    OpenCloseMap            openSet;


    AStar( Map<T>& m )
        : pathNodeMap( m.w, m.h )
        , cameFrom( m.w, m.h )
        , closedSet( m.w, m.h )
        , openSet( m.w, m.h )
    {
        for( int x = 0; x < m.w; x++ )
        {
            for( int y = 0; y < m.h; y++ )
            {
                pathNodeMap.At( x, y ).Assign( x, y, &m.At( x, y ) );
            }
        }
    }

    bool Search( int aX, int aY, int bX, int bY )
    {
        auto startNode = &pathNodeMap.At( aX, aY );
        auto endNode = &pathNodeMap.At( bX, bY );

        if( startNode == endNode )
        {
            searchResults.clear();
            return true;
        }

        closedSet.Clear();
        openSet.Clear();
        orderedOpenSet.Clear();

        if( searchResults.size() )
        {
            for( size_t i = 0; i < searchResults.size(); ++i )
            {
                auto n = searchResults[ i ];
                cameFrom.At( n->x, n->y ) = nullptr;
            }
            searchResults.clear();
        }
        else cameFrom.Clear();

        startNode->g = 0;
        startNode->h = Heuristic( startNode, endNode );
        startNode->f = startNode->h;

        openSet.Add( startNode->x, startNode->y );
        orderedOpenSet.Push( startNode );

        std::vector<PPNT> neighbors;
        neighbors.resize( 8 );

        while( openSet.c )
        {
            auto x = orderedOpenSet.Pop();
            if( x == endNode )
            {
                ReconstructPath( cameFrom.At( endNode->x, endNode->y ) );
                searchResults.push_back( endNode );
                return true;
            }

            openSet.Remove( x->x, x->y );
            closedSet.Add( x->x, x->y );

            FillNeighbors( x, neighbors );

            for( auto y : neighbors )
            {
                if( !x->userContext->IsWalkable( *y->userContext )
                    || closedSet.Contains( y->x, y->y ) ) continue;

                bool better, added = false;

                auto score = pathNodeMap.At( x->x, x->y ).g + NeighborDistance( x, y );

                if( !openSet.Contains( y->x, y->y ) )
                {
                    openSet.Add( y->x, y->y );
                    better = true;
                    added = true;
                }
                else if( score < pathNodeMap.At( y->x, y->y ).g )
                {
                    better = true;
                }
                else
                {
                    better = false;
                }

                if( better )
                {
                    cameFrom.At( y->x, y->y ) = x;

                    auto& n = pathNodeMap.At( y->x, y->y );
                    n.g = score;
                    n.h = Heuristic( y, endNode );
                    n.f = n.g + n.h;

                    if( added ) orderedOpenSet.Push( y );
                    else orderedOpenSet.Update( y );
                }
            }
        }

        return false;
    }

protected:

    void FillNeighbors( PPNT o, std::vector<PPNT>& neighbors )
    {
        int x = o->x, y = o->y;
        neighbors[ 0 ] = &pathNodeMap.At( x - 1, y - 1 );
        neighbors[ 1 ] = &pathNodeMap.At( x, y - 1 );
        neighbors[ 2 ] = &pathNodeMap.At( x + 1, y - 1 );
        neighbors[ 3 ] = &pathNodeMap.At( x - 1, y );
        neighbors[ 4 ] = &pathNodeMap.At( x + 1, y );
        neighbors[ 5 ] = &pathNodeMap.At( x - 1, y + 1 );
        neighbors[ 6 ] = &pathNodeMap.At( x, y + 1 );
        neighbors[ 7 ] = &pathNodeMap.At( x + 1, y + 1 );

        // todo: custom neighbors like teleport door ?
    }


    void ReconstructPath( PPNT n )
    {
        searchResults.clear();
        auto p = cameFrom.At( n->x, n->y );
        searchResults.push_back( p );
        while( p = cameFrom.At( p->x, p->y ) )
        {
            searchResults.push_back( p );
        }
    }

    const float sqrt_2 = sqrtf( 2 );
    float Heuristic( PPNT a, PPNT b )
    {
        return sqrtf( float(( a->x - b->x ) * ( a->x - b->x ) + ( a->y - b->y ) * ( a->y - b->y )) );
    }
    float NeighborDistance( PPNT a, PPNT b )
    {
        int diffX = abs( a->x - b->x );
        int diffY = abs( a->y - b->y );

        switch( diffX + diffY )
        {
        case 1: return 1;
        case 2: return sqrt_2;
        case 0: return 0;
        default:
            assert( false );
            return 0;
        }
    }

};


#endif
