#include "All.h"

FastPool::FastPool()
    : _itemHeader( nullptr )
    , _itemBufLen( 0 )
    , _pageBufLen( 0 )
{
}

FastPool::FastPool( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128*/ )
    : _itemHeader( nullptr )
{
    init( itemBufLen, pageBufLen, capacity );
}

void FastPool::init( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128*/ )
{
    if( itemBufLen < sizeof( size_t ) ) _itemBufLen = sizeof( size_t );
    else _itemBufLen = (int)Utils::round2n( itemBufLen );
    if( pageBufLen < 4096 ) _pageBufLen = 4096;
    else _pageBufLen = (int)Utils::round2n( pageBufLen );
    _pages.reserve( capacity / ( _pageBufLen / _itemBufLen ) );
    reserve( capacity );
}

FastPool::~FastPool()
{
    for( int i = 0; i < _pages.size(); ++i ) aligned_free( _pages[ i ] );
}

FastPool::FastPool( FastPool&& other )
    : _itemBufLen( other._itemBufLen )
    , _pageBufLen( other._pageBufLen )
    , _pages( std::move( other._pages ) )
    , _itemHeader( other._itemHeader )
{
    other._itemBufLen = 0;
    other._pageBufLen = 0;
    other._itemHeader = nullptr;
}

FastPool& FastPool::operator=( FastPool&& other )
{
    _itemBufLen = other._itemBufLen;
    _pageBufLen = other._pageBufLen;
    _pages = std::move( other._pages );
    _itemHeader = other._itemHeader;
    other._itemHeader = nullptr;
    return *this;
}

void FastPool::clear()
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

NOINLINE void FastPool::reserve()
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

void FastPool::reserve( int capacity )
{
    for( int i = 0; i < capacity / ( _pageBufLen / _itemBufLen ); ++i ) reserve();
}

void* FastPool::alloc()
{
    assert( _itemBufLen && _pageBufLen );
    if( !_itemHeader ) reserve();
    auto rtv = _itemHeader;
    _itemHeader = *(void**)_itemHeader;
    return rtv;
}

void FastPool::free( void* p )
{
    assert( p );
    *(void**)p = _itemHeader;
    _itemHeader = p;
}
