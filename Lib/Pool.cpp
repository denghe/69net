#include "All.h"


Pool::Pool( int itemSize, int pageSize, int capacity )
{
    init( itemSize, pageSize, capacity );
}

Pool::Pool()
    :_itemSize( 0 )
    , _pageSize( 0 )
{
}

void Pool::init( int itemSize, int pageSize /*= 4096*/, int capacity /*= 128 */ )
{
    assert( itemSize <= pageSize && itemSize > 0 && capacity >= 0 );
    _itemSize = itemSize; //(int)Utils::round2n( itemSize );
    if( pageSize < 4096 )
        _pageSize = 4096;
    else
        _pageSize = (int)Utils::round2n( pageSize );
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
        for( int i = 0; i < _pageSize; i += _itemSize ) _items.push( p + i );
    }
}


void Pool::collect()
{
    auto getPage = [ &]( void* item ) ->void*
    {
        for( int i = 0; i < _pages.size(); ++i )
        {
            auto ps = (size_t)_pages[ i ];
            auto pe = (size_t)_pages[ i ] + _pageSize;
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
        //auto it = dict.find( page );
        //if( it != dict.end() )
        //{
        //    ++it->second;
        //}
        //else dict[ page ] = 1;
    }
    for( int i = 0; i < _pages.size(); ++i )
    {
        if( dict[ _pages[ i ] ] == _pageSize / _itemSize )
        {
            auto ps = (size_t)_pages[ i ];
            auto pe = (size_t)_pages[ i ] + _pageSize;
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
    auto p = new char[ _pageSize ];        // maybe need align 8/16
    _pages.push( p );
    for( int i = 0; i < _pageSize / _itemSize; ++i ) _items.push( p + i * _itemSize );
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
    return _pageSize;
}

int Pool::itemBufLen() const
{
    return _itemSize;
}

