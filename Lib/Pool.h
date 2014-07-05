#ifndef _POOL_H__
#define _POOL_H__

class Pool
{
public:
    Pool();     // need init
    void init( int itemSize, int pageSize = 4096, int capacity = 128 );
    Pool( int itemSize, int pageSize = 4096, int capacity = 128 );
    ~Pool();
    Pool( Pool const& other ) = delete;
    Pool& operator=( Pool const& other ) = delete;
    void* alloc();
    void free( void* p );
    void clear();
    void collect();
    void reserve();     // new 1 page
    void reserve( int capacity );
    int size() const;
    int pageCount() const;
    int pageBufLen() const;
    int itemBufLen() const;
private:
    List<void*> _items;
    List<void*> _pages;
    int _itemSize;
    int _pageSize;
};

#endif
