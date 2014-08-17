#ifndef _OLDPOOL_H__
#define _OLDPOOL_H__

class OldPool : Memmoveable
{
public:
    OldPool();     // need init
    void init( int itemBufLen, int pageBufLen = 4096, int capacity = 128, bool attachThis = false );
    OldPool( int itemBufLen, int pageBufLen = 4096, int capacity = 128, bool attachThis = false );
    ~OldPool();
    OldPool( OldPool&& other );
    OldPool& operator=( OldPool&& other );
    OldPool( OldPool const& other ) = delete;
    OldPool& operator=( OldPool const& other ) = delete;
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
