#ifndef _POOL_H__
#define _POOL_H__

// todo: 可选择于 item 前面附加 this 指针
class Pool
{
public:
    Pool();     // need init
    void init( int itemBufLen, int pageBufLen = 4096, int capacity = 128, bool attackPointer = false );
    Pool( int itemBufLen, int pageBufLen = 4096, int capacity = 128, bool attackPointer = false );
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
    bool attackPointer() const;
private:
    List<void*> _items;
    List<void*> _pages;
    int _itemBufLen;
    int _pageBufLen;
    bool _attackPointer;
};

#endif
