#ifndef _POOL_H__
#define _POOL_H__

namespace xxx
{


    class Pool : Memmoveable
    {
    public:
        Pool();     // need Init
        void Init( int _itemBufLen, int _pageBufLen = 4096, int capacity = 128 );
        Pool( int _itemBufLen, int _pageBufLen = 4096, int capacity = 128 );
        ~Pool();
        Pool( Pool&& o );
        Pool& operator=( Pool&& o );
        Pool( Pool const& o ) = delete;
        Pool& operator=( Pool const& o ) = delete;
        void Clear();
        void Reserve();     // new 1 page
        void Reserve( int capacity );
        void* Alloc();
        void Free( void* p );
        int ItemBufLen() const;
    private:
        List<void*> pages;
        void* itemHeader;
        int itemBufLen;
        int pageBufLen;
    };

}

#endif
