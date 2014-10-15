#include "All.h"

namespace xxx
{


    Pool::Pool()
        : _itemHeader( nullptr )
        , _itemBufLen( 0 )
        , _pageBufLen( 0 )
    {
    }

    Pool::Pool( int ItemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128*/ )
        : _itemHeader( nullptr )
    {
        Init( ItemBufLen, pageBufLen, capacity );
    }

    void Pool::Init( int ItemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128*/ )
    {
        if( ItemBufLen < sizeof( size_t ) ) _itemBufLen = sizeof( size_t );
        else _itemBufLen = (int)Round2n( ItemBufLen );
        if( pageBufLen < _itemBufLen ) _pageBufLen = _itemBufLen;
        else if( pageBufLen < 4096 ) _pageBufLen = 4096;
        else _pageBufLen = (int)Round2n( pageBufLen );
        Reserve( _pageBufLen / _itemBufLen );
    }

    Pool::~Pool()
    {
        for( int i = 0; i < _pages.Size(); ++i ) aligned_free( _pages[ i ] );
    }

    Pool::Pool( Pool&& other )
        : _itemBufLen( other._itemBufLen )
        , _pageBufLen( other._pageBufLen )
        , _pages( std::move( other._pages ) )
        , _itemHeader( other._itemHeader )
    {
        other._itemBufLen = 0;
        other._pageBufLen = 0;
        other._itemHeader = nullptr;
    }

    Pool& Pool::operator=( Pool&& other )
    {
        _itemBufLen = other._itemBufLen;
        _pageBufLen = other._pageBufLen;
        _pages = std::move( other._pages );
        _itemHeader = other._itemHeader;
        other._itemHeader = nullptr;
        return *this;
    }

    void Pool::Clear()
    {
        if( !_pages.Size() ) return;
        _itemHeader = _pages[ 0 ];
        char* p = nullptr;
        for( int j = 0; j < _pages.Size(); ++j )
        {
            p = (char*)_pages[ j ];
            for( ; p < (char*)_pages[ j ] + _pageBufLen - _itemBufLen; p += _itemBufLen )
            {
                *(void**)p = p + _itemBufLen;
            }
            *(void**)p = _pages[ j + 1 ];   // finger to next page's first item
        }
        *(void**)p = nullptr;   // Clear last finger
    }

    NOINLINE void Pool::Reserve()
    {
        auto page = (char*)aligned_alloc( _pageBufLen, _pageBufLen );
        _pages.Push( page );
        _itemHeader = page;
        auto p = page;
        for( ; p < page + _pageBufLen - _itemBufLen; p += _itemBufLen )
        {
            *(void**)p = p + _itemBufLen;
        }
        *(void**)p = nullptr;
    }

    void Pool::Reserve( int capacity )
    {
        for( int i = 0; i <= capacity / ( _pageBufLen / _itemBufLen ); ++i ) Reserve();
    }

    void* Pool::Alloc()
    {
        assert( _itemBufLen && _pageBufLen );
        if( !_itemHeader ) Reserve();
        auto rtv = _itemHeader;
        _itemHeader = *(void**)_itemHeader;
        return rtv;
    }

    void Pool::Free( void* p )
    {
        assert( p );
        *(void**)p = _itemHeader;
        _itemHeader = p;
    }

    int Pool::ItemBufLen() const
    {
        return _itemBufLen;
    }

}
