#include "All.h"


Pool::Pool( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool attackPointer /*= false*/ )
{
    init( itemBufLen, pageBufLen, capacity, attackPointer );
}

Pool::Pool()
    : _itemBufLen( 0 )
    , _pageBufLen( 0 )
    , _attackPointer( false )
{
}

Pool::Pool( Pool&& other )
    : _itemBufLen( other._itemBufLen )
    , _pageBufLen( other._pageBufLen )
    , _attackPointer( other._attackPointer )
{
    _items = std::move( other._items );
    _pages = std::move( other._pages );
    other._itemBufLen = 0;
    other._pageBufLen = 0;
    other._attackPointer = false;
}

Pool& Pool::operator=( Pool&& other )
{
    _items = std::move( other._items );
    _pages = std::move( other._pages );
    _itemBufLen = other._itemBufLen;
    _pageBufLen = other._pageBufLen;
    _attackPointer = other._attackPointer;
    other._itemBufLen = 0;
    other._pageBufLen = 0;
    other._attackPointer = false;
    return *this;
}

void Pool::init( int itemBufLen, int pageBufLen /*= 4096*/, int capacity /*= 128 */, bool attackPointer /*= false*/ )
{
    assert( itemBufLen <= pageBufLen && itemBufLen > 0 && capacity >= 0 );
    _attackPointer = attackPointer;
    _itemBufLen = itemBufLen; //(int)Utils::round2n( itemSize );
    if( pageBufLen < 4096 )
        _pageBufLen = 4096;
    else
        _pageBufLen = (int)Utils::round2n( pageBufLen );
    reserve( capacity );
}

Pool::~Pool()
{
    for( int i = 0; i < _pages.size(); ++i ) delete[] _pages[ i ];
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


void Pool::collect()
{
    auto getPage = [ &]( void* item ) ->void*
    {
        for( int i = 0; i < _pages.size(); ++i )
        {
            auto ps = (size_t)_pages[ i ];
            auto pe = (size_t)_pages[ i ] + _pageBufLen;
            auto v = (size_t)item;
            if( ps <= v && v < pe ) return _pages[ i ];
        }
        return 0;
    };

    std::unordered_map<void*, int> dict;    // todo: replace to Dict
    for( int i = 0; i < _items.size(); ++i )
    {
        auto page = getPage( _items[ i ] );
        dict[ page ] += 1;
    }
    for( int i = 0; i < _pages.size(); ++i )
    {
        if( dict[ _pages[ i ] ] == _pageBufLen / _itemBufLen )
        {
            auto ps = (size_t)_pages[ i ];
            auto pe = (size_t)_pages[ i ] + _pageBufLen;
            for( int j = 0; j < _items.size(); ++j )
            {
                auto v = (size_t)_items[ j ];
                if( ps <= v && v < pe )
                {
                    _items[ j-- ] = _items.top();
                    _items.pop();
                }
            }
            delete[] _pages[ i ];
            _pages[ i-- ] = _pages.top();
            _pages.pop();
        }
    }
}


void Pool::reserve()
{
    auto p = new char[ _pageBufLen ];        // maybe need align 8/16
    _pages.push( p );
    if( !_attackPointer )
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

bool Pool::attackPointer() const
{
    return _attackPointer;
}

