#include "All.h"


Pool::Pool( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool attachThis /*= false*/ )
{
    init( itemBufLen, pageBufLen, capacity, attachThis );
}

Pool::Pool()
    : _itemBufLen( 0 )
    , _pageBufLen( 0 )
    , _attachThis( false )
{
}

Pool::Pool( Pool&& other )
    : _itemBufLen( other._itemBufLen )
    , _pageBufLen( other._pageBufLen )
    , _attachThis( other._attachThis )
{
    _items = std::move( other._items );
    _pages = std::move( other._pages );
    other._itemBufLen = 0;
    other._pageBufLen = 0;
    other._attachThis = false;
}

Pool& Pool::operator=( Pool&& other )
{
    _items = std::move( other._items );
    _pages = std::move( other._pages );
    _itemBufLen = other._itemBufLen;
    _pageBufLen = other._pageBufLen;
    _attachThis = other._attachThis;
    other._itemBufLen = 0;
    other._pageBufLen = 0;
    other._attachThis = false;
    return *this;
}

void Pool::init( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool attachThis /*= false*/ )
{
    assert( itemBufLen <= pageBufLen && itemBufLen > 0 && capacity > 0 );
    _attachThis = attachThis;
    _itemBufLen = itemBufLen; //(int)Utils::round2n( itemSize );
    if( pageBufLen < 4096 )
        _pageBufLen = 4096;
    else
        _pageBufLen = (int)Utils::round2n( pageBufLen );
    reserve( capacity );
}

Pool::~Pool()
{
    for( int i = 0; i < _pages.size(); ++i ) aligned_free( _pages[ i ] );
}


void* Pool::alloc()
{
    if( _items.size() == 0 ) reserve();
    auto rtv = _items.top();
    _items.pop();
    return rtv;
}


void Pool::free( void* p )
{
    _items.push( p );
}


void Pool::clear()
{
    for( int j = 0; j < _pages.size(); ++j )
    {
        auto p = (char*)_pages[ j ];
        for( int i = 0; i < _pageBufLen; i += _itemBufLen ) _items.push( p + i );
    }
}


void Pool::compress()
{
    auto mask = (size_t)-_pageBufLen;
    auto numItemsPerPage = _pageBufLen / _itemBufLen;
    Dict<size_t, int> dict;
    for( int i = 0; i < _items.size(); ++i )
    {
        dict[ (size_t)_items[ i ] & mask ] += 1;
    }
    for( int i = 0; i < _items.size(); ++i )
    {
        if( dict[ (size_t)_items[ i ] & mask ] == numItemsPerPage )
        {
            _items.eraseFast( i-- );
        }
    }
    for( int i = 0; i < _pages.size(); ++i )
    {
        if( dict[ (size_t)_pages[ i ] ] == numItemsPerPage )
        {
            aligned_free( _pages[ i ] );
            _pages.eraseFast( i-- );
        }
    }
}


void Pool::reserve()
{
    auto p = (char*)aligned_alloc( _pageBufLen, _pageBufLen );
    _pages.push( p );
    if( !_attachThis )
    {
        for( int i = 0; i < _pageBufLen / _itemBufLen; ++i )
        {
            _items.push( p + i * _itemBufLen );
        }
    }
    else
    {
        for( int i = 0; i < _pageBufLen / _itemBufLen; ++i )
        {
            auto a = p + i * _itemBufLen;
            _items.push( a );
            *(Pool**)( a + _itemBufLen - sizeof( Pool* ) ) = this;
        }
    }
}

void Pool::reserve( int capacity )
{
    while( _items.size() < capacity ) reserve();
}

int Pool::pageCount() const
{
    return _pages.size();
}

int Pool::size() const
{
    return _items.size();
}

int Pool::pageBufLen() const
{
    return _pageBufLen;
}

int Pool::itemBufLen() const
{
    return _itemBufLen;
}

bool Pool::attachThis() const
{
    return _attachThis;
}

