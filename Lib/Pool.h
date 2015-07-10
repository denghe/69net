#pragma once

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

        int ItemBufLen() const;

        void Clear();
        void Reserve();     // new 1 page
        void Reserve( int capacity );

        void* Alloc();
        void Free( void* p );

        template<typename T>
        void TInit()
        {
            Init( sizeof( T ) );
        }
        template<typename T, typename ...PTS>
        T* TAlloc( PTS&& ...ps )
        {
            assert( itemBufLen >= sizeof( T ) );
            auto p = Alloc();
            return new (p) T( std::forward<PTS>( ps )... );
        }
        template<typename T>
        void TFree( T* p )
        {
            p->T::~T();
            Free( p );
        }

    private:
        List<void*> pages;
        void* itemHeader;
        int itemBufLen;
        int pageBufLen;

#if __DEBUG
    public:
        int allocatedCounter = 0;
#endif
    };

}
