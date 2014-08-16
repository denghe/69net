#ifndef _FASTPOOL_H__
#define _FASTPOOL_H__

class FastPool : Memmoveable
{
public:
    FastPool();     // need init
    void init( int itemBufLen, int pageBufLen = 4096, int capacity = 128 );
    FastPool( int itemBufLen, int pageBufLen = 4096, int capacity = 128 );
    ~FastPool();
    FastPool( FastPool&& other );
    FastPool& operator=( FastPool&& other );
    FastPool( FastPool const& other ) = delete;
    FastPool& operator=( FastPool const& other ) = delete;
    void clear();
    void reserve();     // new 1 page
    void reserve( int capacity );
    void* alloc();
    void free( void* p );
private:
    List<void*> _pages;
    void* _itemHeader;
    int _itemBufLen;
    int _pageBufLen;
};

#endif
