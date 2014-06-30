#ifndef _POOL_H__
#define _POOL_H__

#include "Stack.h"

class Pool
{
public:
    Pool( int pageSize, int capacity );
    ~Pool();
    Pool( Pool const& other ) = delete;
    Pool& operator=( Pool const& other ) = delete;
    void* alloc();
    void free( void* p );
    void clear();
    void collect();
    void reserve();
private:
    Stack<void*> _pointers;
}

#endif
