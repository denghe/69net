#include "All.h"

namespace xxx
{


    OldPool::OldPool( int ItemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool AttachThis /*= false*/ )
    {
        Init( ItemBufLen, pageBufLen, capacity, AttachThis );
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

    void OldPool::Init( int ItemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool AttachThis /*= false*/ )
    {
        assert( ItemBufLen <= pageBufLen && ItemBufLen > 0 && capacity > 0 );
        _attachThis = AttachThis;
        _itemBufLen = ItemBufLen; //(int)Round2n( itemSize );
        if( pageBufLen < 4096 )
            _pageBufLen = 4096;
        else
            _pageBufLen = (int)Round2n( pageBufLen );
        _items.Reserve( capacity );
        Reserve( capacity );
    }

    OldPool::~OldPool()
    {
        for( int i = 0; i < _pages.Size(); ++i ) aligned_free( _pages[ i ] );
    }


    void* OldPool::Alloc()
    {
        if( _items.Size() == 0 ) Reserve();
        return _items.TopPop();
    }


    void OldPool::Free( void* p )
    {
        _items.Push( p );
    }


    void OldPool::Clear()
    {
        for( int j = 0; j < _pages.Size(); ++j )
        {
            auto p = (char*)_pages[ j ];
            for( int i = 0; i < _pageBufLen; i += _itemBufLen ) _items.Push( p + i );
        }
    }


    void OldPool::Compress()
    {
        auto mask = (size_t)-_pageBufLen;
        auto numItemsPerPage = _pageBufLen / _itemBufLen;
        Dict<size_t, int> dict;
        for( int i = 0; i < _items.Size(); ++i )
        {
            dict[ (size_t)_items[ i ] & mask ] += 1;
        }
        for( int i = 0; i < _items.Size(); ++i )
        {
            if( dict[ (size_t)_items[ i ] & mask ] == numItemsPerPage )
            {
                _items.EraseFast( i-- );
            }
        }
        for( int i = 0; i < _pages.Size(); ++i )
        {
            if( dict[ (size_t)_pages[ i ] ] == numItemsPerPage )
            {
                aligned_free( _pages[ i ] );
                _pages.EraseFast( i-- );
            }
        }
    }


    void OldPool::Reserve()
    {
        auto p = (char*)aligned_alloc( _pageBufLen, _pageBufLen );
        _pages.Push( p );
        if( !_attachThis )
        {
            for( int i = 0; i < _pageBufLen / _itemBufLen; ++i )
            {
                _items.Push( p + i * _itemBufLen );
            }
        }
        else
        {
            for( int i = 0; i < _pageBufLen / _itemBufLen; ++i )
            {
                auto a = p + i * _itemBufLen;
                _items.Push( a );
                *(OldPool**)( a + _itemBufLen - sizeof( OldPool* ) ) = this;
            }
        }
    }

    void OldPool::Reserve( int capacity )
    {
        while( _items.Size() < capacity ) Reserve();
    }

    int OldPool::pageCount() const
    {
        return _pages.Size();
    }

    int OldPool::Size() const
    {
        return _items.Size();
    }

    int OldPool::pageBufLen() const
    {
        return _pageBufLen;
    }

    int OldPool::ItemBufLen() const
    {
        return _itemBufLen;
    }

    bool OldPool::AttachThis() const
    {
        return _attachThis;
    }

}
