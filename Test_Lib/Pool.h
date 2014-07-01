#ifndef _POOL_H__
#define _POOL_H__

#include "List.h"

template<int itemSize, int pageSize>
class Pool
{
public:
    Pool( int capacity = 1 );
    ~Pool();
    Pool( Pool const& other ) = delete;
    Pool& operator=( Pool const& other ) = delete;
    void* alloc();
    void free( void* p );
    void clear();
    void collect();
    void reserve();     // new 1 page
    int pageCount();
    int itemCount();
private:
    List<void*> _items;
    List<void*> _pages;
};

#include "Pool.hpp"

#endif
