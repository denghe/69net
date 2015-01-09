#ifndef __PRIORITYQUEUE_H__
#define __PRIORITYQUEUE_H__

#include <vector>


// the T must be a pointer type and have a "int pqIdx" field

template<typename T>
struct PriorityQueue
{
    int Push( T item )
    {
        int p = data.size(), p2;
        item->pqIdx = p;
        data.push_back( item );
        do
        {
            if( p == 0 ) break;
            p2 = ( p - 1 ) / 2;
            if( data[ p ]->f < data[ p2 ]->f )
            {
                Swap( p, p2 );
                p = p2;
            }
            else break;
        } while( true );
        return p;
    }

    inline T Pop()
    {
        auto result = data[ 0 ];
        int p = 0, p1, p2, pn;

        data[ 0 ] = data[ data.size() - 1 ];
        data[ 0 ]->pqIdx = 0;
        data.pop_back();

        do
        {
            pn = p;
            p1 = 2 * p + 1;
            p2 = 2 * p + 2;
            if( data.size() > p1 && data[ p ]->f > data[ p1 ]->f ) p = p1;
            if( data.size() > p2 && data[ p ]->f > data[ p2 ]->f ) p = p2;
            if( p == pn ) break;
            Swap( p, pn );
        } while( true );

        result->pqIdx = -1;
        return result;
    }

    inline void Update( T item )
    {
        int count = data.size();
        while( ( item->pqIdx - 1 >= 0 ) && ( data[ item->pqIdx - 1 ]->f > data[ item->pqIdx ]->f ) )
        {
            Swap( item->pqIdx - 1, item->pqIdx );
        }
        while( ( item->pqIdx + 1 < count ) && ( data[ item->pqIdx + 1 ]->f < data[ item->pqIdx ]->f ) )
        {
            Swap( item->pqIdx + 1, item->pqIdx );
        }
    }

    inline void Clear()
    {
        data.clear();
    }

private:
    void Swap( int srcIdx, int dstIdx )
    {
        auto h = data[ srcIdx ];
        data[ srcIdx ] = data[ dstIdx ];
        data[ dstIdx ] = h;

        data[ srcIdx ]->pqIdx = srcIdx;
        data[ dstIdx ]->pqIdx = dstIdx;
    }

    std::vector<T> data;
};


#endif
