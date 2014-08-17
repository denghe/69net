#include "All.h"

Pool::Pool()
    : _itemHeader( nullptr )
    , _itemBufLen( 0 )
    , _pageBufLen( 0 )
{
}

Pool::Pool( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128*/ )
    : _itemHeader( nullptr )
{
    init( itemBufLen, pageBufLen, capacity );
}

void Pool::init( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128*/ )
{
    if( itemBufLen < sizeof( size_t ) ) _itemBufLen = sizeof( size_t );
    else _itemBufLen = (int)Utils::round2n( itemBufLen );
    if( pageBufLen < _itemBufLen ) _pageBufLen = _itemBufLen;
    else if( pageBufLen < 4096 ) _pageBufLen = 4096;
    else _pageBufLen = (int)Utils::round2n( pageBufLen );
    reserve( _pageBufLen / _itemBufLen );
}

Pool::~Pool()
{
    for( int i = 0; i < _pages.size(); ++i ) aligned_free( _pages[ i ] );
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

void Pool::clear()
{
    if( !_pages.size() ) return;
    _itemHeader = _pages[ 0 ];
    char* p = nullptr;
    for( int j = 0; j < _pages.size(); ++j )
    {
        p = (char*)_pages[ j ];
        for( ; p < (char*)_pages[ j ] + _pageBufLen - _itemBufLen; p += _itemBufLen )
        {
            *(void**)p = p + _itemBufLen;
        }
        *(void**)p = _pages[ j + 1 ];   // finger to next page's first item
    }
    *(void**)p = nullptr;   // clear last finger
}

NOINLINE void Pool::reserve()
{
    auto page = (char*)aligned_alloc( _pageBufLen, _pageBufLen );
    _pages.push( page );
    _itemHeader = page;
    auto p = page;
    for( ; p < page + _pageBufLen - _itemBufLen; p += _itemBufLen )
    {
        *(void**)p = p + _itemBufLen;
    }
    *(void**)p = nullptr;
}

void Pool::reserve( int capacity )
{
    for( int i = 0; i <= capacity / ( _pageBufLen / _itemBufLen ); ++i ) reserve();
}

void* Pool::alloc()
{
    assert( _itemBufLen && _pageBufLen );
    if( !_itemHeader ) reserve();
    auto rtv = _itemHeader;
    _itemHeader = *(void**)_itemHeader;
    return rtv;
}

void Pool::free( void* p )
{
    assert( p );
    *(void**)p = _itemHeader;
    _itemHeader = p;
}

int Pool::itemBufLen() const
{
    return _itemBufLen;
}
