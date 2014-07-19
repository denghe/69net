#ifndef _POOL_H__
#define _POOL_H__

class Pool : Memmoveable
{
public:
    Pool();     // need init
    void init( int itemBufLen, int pageBufLen = 4096, int capacity = 128, bool attachThis = false );
    Pool( int itemBufLen, int pageBufLen = 4096, int capacity = 128, bool attachThis = false );
    ~Pool();
    Pool( Pool&& other );
    Pool& operator=( Pool&& other );
    Pool( Pool const& other ) = delete;
    Pool& operator=( Pool const& other ) = delete;
    void* alloc();
    void free( void* p );
    void clear();
    void compress();    // free all no use page & item
    void reserve();     // new 1 page
    void reserve( int capacity );
    int size() const;
    int pageCount() const;
    int pageBufLen() const;
    int itemBufLen() const;
    bool attachThis() const;
private:
    List<void*> _items;
    List<void*> _pages;
    int _itemBufLen;
    int _pageBufLen;
    bool _attachThis;
};

#endif
