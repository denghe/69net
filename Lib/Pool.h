#ifndef _POOL_H__
#define _POOL_H__

namespace xxx
{


    class Pool : Memmoveable
    {
    public:
        Pool();     // need Init
        void Init( int ItemBufLen, int pageBufLen = 4096, int capacity = 128 );
        Pool( int ItemBufLen, int pageBufLen = 4096, int capacity = 128 );
        ~Pool();
        Pool( Pool&& other );
        Pool& operator=( Pool&& other );
        Pool( Pool const& other ) = delete;
        Pool& operator=( Pool const& other ) = delete;
        void Clear();
        void Reserve();     // new 1 page
        void Reserve( int capacity );
        void* Alloc();
        void Free( void* p );
        int ItemBufLen() const;
    private:
        List<void*> _pages;
        void* _itemHeader;
        int _itemBufLen;
        int _pageBufLen;
    };

}

#endif
