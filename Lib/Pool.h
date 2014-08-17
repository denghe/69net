#ifndef _FASTPOOL_H__
#define _FASTPOOL_H__

class Pool : Memmoveable
{
public:
    Pool();     // need init
    void init( int itemBufLen, int pageBufLen = 4096, int capacity = 128 );
    Pool( int itemBufLen, int pageBufLen = 4096, int capacity = 128 );
    ~Pool();
    Pool( Pool&& other );
    Pool& operator=( Pool&& other );
    Pool( Pool const& other ) = delete;
    Pool& operator=( Pool const& other ) = delete;
    void clear();
    void reserve();     // new 1 page
    void reserve( int capacity );
    void* alloc();
    void free( void* p );
    int itemBufLen() const;
private:
    List<void*> _pages;
    void* _itemHeader;
    int _itemBufLen;
    int _pageBufLen;
};

#endif
