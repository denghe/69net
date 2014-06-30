#ifndef _POOL_H__
#define _POOL_H__

#include "Stack.h"

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
    void reserve();     // new 1 page
    int pageCount();
    int itemCount();
private:
    Stack<void*> _items;
    Stack<void*> _pages;
};

#include "Pool.hpp"

#endif
