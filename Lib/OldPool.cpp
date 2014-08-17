#include "All.h"


OldPool::OldPool( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool attachThis /*= false*/ )
{
    init( itemBufLen, pageBufLen, capacity, attachThis );
}

OldPool::OldPool()
    : _itemBufLen( 0 )
    , _pageBufLen( 0 )
    , _attachThis( false )
{
}

OldPool::OldPool( OldPool&& other )
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

OldPool& OldPool::operator=( OldPool&& other )
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

void OldPool::init( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool attachThis /*= false*/ )
{
    assert( itemBufLen <= pageBufLen && itemBufLen > 0 && capacity > 0 );
    _attachThis = attachThis;
    _itemBufLen = itemBufLen; //(int)Utils::round2n( itemSize );
    if( pageBufLen < 4096 )
        _pageBufLen = 4096;
    else
        _pageBufLen = (int)Utils::round2n( pageBufLen );
    _items.reserve( capacity );
    reserve( capacity );
}

OldPool::~OldPool()
{
    for( int i = 0; i < _pages.size(); ++i ) aligned_free( _pages[ i ] );
}


void* OldPool::alloc()
{
    if( _items.size() == 0 ) reserve();
    return _items.top_pop();
}


void OldPool::free( void* p )
{
    _items.push( p );
}


void OldPool::clear()
{
    for( int j = 0; j < _pages.size(); ++j )
    {
        auto p = (char*)_pages[ j ];
        for( int i = 0; i < _pageBufLen; i += _itemBufLen ) _items.push( p + i );
    }
}


void OldPool::compress()
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


void OldPool::reserve()
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
            *(OldPool**)( a + _itemBufLen - sizeof( OldPool* ) ) = this;
        }
    }
}

void OldPool::reserve( int capacity )
{
    while( _items.size() < capacity ) reserve();
}

int OldPool::pageCount() const
{
    return _pages.size();
}

int OldPool::size() const
{
    return _items.size();
}

int OldPool::pageBufLen() const
{
    return _pageBufLen;
}

int OldPool::itemBufLen() const
{
    return _itemBufLen;
}

bool OldPool::attachThis() const
{
    return _attachThis;
}

