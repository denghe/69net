#include "All.h"

namespace xxx
{
    OldPool::OldPool( int _itemBufLen, int _pageBufLen /*= 4096*/, int capacity /*= 128 */, bool AttachThis /*= false*/ )
    {
        Init( _itemBufLen, _pageBufLen, capacity, AttachThis );
    }

    OldPool::OldPool()
        : itemBufLen( 0 )
        , pageBufLen( 0 )
        , attachThis( false )
    {
    }

    OldPool::OldPool( OldPool&& o )
        : itemBufLen( o.itemBufLen )
        , pageBufLen( o.pageBufLen )
        , attachThis( o.attachThis )
    {
        items = std::move( o.items );
        pages = std::move( o.pages );
        o.itemBufLen = 0;
        o.pageBufLen = 0;
        o.attachThis = false;
    }

    OldPool& OldPool::operator=( OldPool&& o )
    {
        items = std::move( o.items );
        pages = std::move( o.pages );
        itemBufLen = o.itemBufLen;
        pageBufLen = o.pageBufLen;
        attachThis = o.attachThis;
        o.itemBufLen = 0;
        o.pageBufLen = 0;
        o.attachThis = false;
        return *this;
    }

    void OldPool::Init( int _itemBufLen, int _pageBufLen /*= 4096*/, int capacity /*= 128 */, bool _attachThis /*= false*/ )
    {
        assert( _itemBufLen <= _pageBufLen && _itemBufLen > 0 && capacity > 0 );
        attachThis = _attachThis;
        itemBufLen = _itemBufLen; //(int)Round2n( itemSize );
        if( _pageBufLen < 4096 )
            pageBufLen = 4096;
        else
            pageBufLen = (int)Round2n( _pageBufLen );
        items.Reserve( capacity );
        Reserve( capacity );
    }

    OldPool::~OldPool()
    {
        for( int i = 0; i < pages.Size(); ++i ) aligned_free( pages[ i ] );
    }


    void* OldPool::Alloc()
    {
        if( items.Size() == 0 ) Reserve();
        return items.TopPop();
    }


    void OldPool::Free( void* p )
    {
        items.Push( p );
    }


    void OldPool::Clear()
    {
        for( int j = 0; j < pages.Size(); ++j )
        {
            auto p = (char*)pages[ j ];
            for( int i = 0; i < pageBufLen; i += itemBufLen ) items.Push( p + i );
        }
    }


    void OldPool::Compress()
    {
        auto mask = (size_t)-pageBufLen;
        auto numItemsPerPage = pageBufLen / itemBufLen;
        Dict<size_t, int> dict;
        for( int i = 0; i < items.Size(); ++i )
        {
            dict[ (size_t)items[ i ] & mask ] += 1;
        }
        for( int i = 0; i < items.Size(); ++i )
        {
            if( dict[ (size_t)items[ i ] & mask ] == numItemsPerPage )
            {
                items.EraseFast( i-- );
            }
        }
        for( int i = 0; i < pages.Size(); ++i )
        {
            if( dict[ (size_t)pages[ i ] ] == numItemsPerPage )
            {
                aligned_free( pages[ i ] );
                pages.EraseFast( i-- );
            }
        }
    }


    void OldPool::Reserve()
    {
        auto p = (char*)aligned_alloc( pageBufLen, pageBufLen );
        pages.Push( p );
        if( !attachThis )
        {
            for( int i = 0; i < pageBufLen / itemBufLen; ++i )
            {
                items.Push( p + i * itemBufLen );
            }
        }
        else
        {
            for( int i = 0; i < pageBufLen / itemBufLen; ++i )
            {
                auto a = p + i * itemBufLen;
                items.Push( a );
                *(OldPool**)( a + itemBufLen - sizeof( OldPool* ) ) = this;
            }
        }
    }

    void OldPool::Reserve( int capacity )
    {
        while( items.Size() < capacity ) Reserve();
    }

    int OldPool::PageCount() const
    {
        return pages.Size();
    }

    int OldPool::Size() const
    {
        return items.Size();
    }

    int OldPool::PageBufLen() const
    {
        return pageBufLen;
    }

    int OldPool::ItemBufLen() const
    {
        return itemBufLen;
    }

    bool OldPool::AttachThis() const
    {
        return attachThis;
    }

}
