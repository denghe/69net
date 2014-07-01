template<int itemSize, int pageSize>
Pool<itemSize, pageSize>::Pool( int capacity )
{
    for( int i = 0; i < capacity; ++i ) reserve();
}

template<int itemSize, int pageSize>
Pool<itemSize, pageSize>::~Pool()
{
    for( int i = 0; i < _pages.size(); ++i ) delete[] _pages[ i ];
}

template<int itemSize, int pageSize>
void* Pool<itemSize, pageSize>::alloc()
{
    if( _items.size() == 0 ) reserve();
    auto rtv = _items.top();
    _items.pop();
    return rtv;
}

template<int itemSize, int pageSize>
void Pool<itemSize, pageSize>::free( void* p )
{
    _items.push( p );
}

template<int itemSize, int pageSize>
void Pool<itemSize, pageSize>::clear()
{
    for( int j = 0; j < _pages.size(); ++j )
    {
        auto p = (char*)_pages[ j ];
        for( int i = 0; i < pageSize; i += itemSize ) _items.push( p + i );
    }
}

template<int itemSize, int pageSize>
void Pool<itemSize, pageSize>::collect()
{
    auto getPage = [ &]( void* item ) ->void*
    {
        for( int i = 0; i < _pages.size(); ++i )
        {
            auto ps = (size_t)_pages[ i ];
            auto pe = (size_t)_pages[ i ] + pageSize;
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
        if( dict[ _pages[ i ] ] == pageSize / itemSize )
        {
            auto ps = (size_t)_pages[ i ];
            auto pe = (size_t)_pages[ i ] + pageSize;
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

template<int itemSize, int pageSize>
void Pool<itemSize, pageSize>::reserve()
{
    auto p = new char[ pageSize ];        // maybe need align 8/16
    _pages.push( p );
    for( int i = 0; i < pageSize / itemSize; ++i ) _items.push( p + i * itemSize );
}

template<int itemSize, int pageSize>
int Pool<itemSize, pageSize>::pageCount()
{
    return _pages.size();
}

template<int itemSize, int pageSize>
int Pool<itemSize, pageSize>::itemCount()
{
    return _items.size();
}
