#ifndef __PATHNODE_H__
#define __PATHNODE_H__

#include <type_traits>

template<typename UCT>
struct PathNode
{
    int x = -1, y = -1;
    int pqIdx = -1;
    double g = 0;
    double h = 0;
    double f = 0;
    UCT userContext;

    bool operator<( PathNode const& other )
    {
        return this->f < other.f;
    }

    template<typename T>
    void Assign( int x, int y, T&& userContext )
    {
        this->x = x;
        this->y = y;
        this->userContext = std::forward<T>( userContext );
    }
};

#endif
