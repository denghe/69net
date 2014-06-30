template<int itemSize, int pageSize>
Pool<itemSize, pageSize>::Pool( int capacity )
{
    for( int i = 0; i < capacity; ++i ) reserve();
}

template<int itemSize, int pageSize>
Pool<itemSize, pageSize>::~Pool()
{
    for( int i = 0; i < _pages.count(); ++i ) delete[] _pages[ i ];
}

template<int itemSize, int pageSize>
void* Pool<itemSize, pageSize>::alloc()
{
    if( _items.count() == 0 ) reserve();
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
    for( int j = 0; j < _pages.count(); ++j )
    {
        auto p = (char*)_pages[ j ];
        for( int i = 0; i < pageSize; i += itemSize ) _items.push( p + i );
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
    return _pages.count();
}

template<int itemSize, int pageSize>
int Pool<itemSize, pageSize>::itemCount()
{
    return _items.count();
}
