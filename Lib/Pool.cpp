#include "All.h"

namespace xxx
{

    Pool::Pool()
        : itemHeader( nullptr )
        , itemBufLen( 0 )
        , pageBufLen( 0 )
    {
    }

    Pool::Pool( int _itemBufLen, int _pageBufLen /*= 4096*/, int capacity /*= 128*/ )
        : itemHeader( nullptr )
    {
        Init( _itemBufLen, _pageBufLen, capacity );
    }

    void Pool::Init( int _itemBufLen, int _pageBufLen /*= 4096*/, int capacity /*= 128*/ )
    {
        if( _itemBufLen < sizeof( size_t ) ) itemBufLen = sizeof( size_t );
        else itemBufLen = (int)Round2n( _itemBufLen );
        if( _pageBufLen < itemBufLen ) pageBufLen = itemBufLen;
        else if( _pageBufLen < 4096 ) pageBufLen = 4096;
        else pageBufLen = (int)Round2n( _pageBufLen );
        Reserve( pageBufLen / itemBufLen );
    }

    Pool::~Pool()
    {
        for( int i = 0; i < pages.Size(); ++i ) aligned_free( pages[ i ] );
    }

    Pool::Pool( Pool&& o )
        : itemBufLen( o.itemBufLen )
        , pageBufLen( o.pageBufLen )
        , pages( std::move( o.pages ) )
        , itemHeader( o.itemHeader )
    {
        o.itemBufLen = 0;
        o.pageBufLen = 0;
        o.itemHeader = nullptr;
    }

    Pool& Pool::operator=( Pool&& o )
    {
        itemBufLen = o.itemBufLen;
        pageBufLen = o.pageBufLen;
        pages = std::move( o.pages );
        itemHeader = o.itemHeader;
        o.itemHeader = nullptr;
        return *this;
    }

    void Pool::Clear()
    {
        if( !pages.Size() ) return;
        itemHeader = pages[ 0 ];
        char* p = nullptr;
        for( int j = 0; j < pages.Size(); ++j )
        {
            p = (char*)pages[ j ];
            for( ; p < (char*)pages[ j ] + pageBufLen - itemBufLen; p += itemBufLen )
            {
                *(void**)p = p + itemBufLen;
            }
            *(void**)p = pages[ j + 1 ];   // finger to next page's first item
        }
        *(void**)p = nullptr;   // Clear last finger

#if __DEBUG
        allocatedCounter = 0;
#endif

    }

    NOINLINE void Pool::Reserve()
    {
        auto page = (char*)aligned_alloc( pageBufLen, pageBufLen );
        pages.Push( page );
        itemHeader = page;
        auto p = page;
        for( ; p < page + pageBufLen - itemBufLen; p += itemBufLen )
        {
            *(void**)p = p + itemBufLen;
        }
        *(void**)p = nullptr;
    }

    void Pool::Reserve( int capacity )
    {
        for( int i = 0; i <= capacity / ( pageBufLen / itemBufLen ); ++i ) Reserve();
    }

    void* Pool::Alloc()
    {
        assert( itemBufLen && pageBufLen );
        if( !itemHeader ) Reserve();
        auto rtv = itemHeader;
        itemHeader = *(void**)itemHeader;

#if __DEBUG
        ++allocatedCounter;
#endif

        return rtv;
    }

    void Pool::Free( void* p )
    {
        assert( p );
        *(void**)p = itemHeader;
        itemHeader = p;

#if __DEBUG
        --allocatedCounter;
#endif
    }

    int Pool::ItemBufLen() const
    {
        return itemBufLen;
    }

}
