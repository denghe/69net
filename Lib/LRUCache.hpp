#ifndef _LRUCACHE_HPP__
#define _LRUCACHE_HPP__

template<typename KT, typename VT>
template<typename PKT, typename PVT>
LRUCacheItem<KT, VT>::LRUCacheItem( PKT && key, PVT && value )
    : _next( nullptr )
    , _prev( nullptr )
    , _key( std::forward<PKT>( key ) )
    , _value( std::forward<PVT>( value ) )
{
}

template<typename KT, typename VT>
LRUCacheItem<KT, VT>::LRUCacheItem( LRUCacheItem const & other )
    : _next( nullptr )
    , _prev( nullptr )
    , _key( other._key )
    , _value( other._value )
{
}

template<typename KT, typename VT>
LRUCacheItem<KT, VT>::LRUCacheItem( LRUCacheItem && other )
    : _next( nullptr )
    , _prev( nullptr )
    , _key( std::move( other._key ) )
    , _value( std::move( other._value ) )
{
}

template<typename KT, typename VT>
LRUCacheItem<KT, VT>& LRUCacheItem<KT, VT>::operator=( LRUCacheItem const & other )
{
    _next = nullptr;
    _prev = nullptr;
    _key = other._key;
    _value = other._value;
    return *this;
}

template<typename KT, typename VT>
LRUCacheItem<KT, VT>& LRUCacheItem<KT, VT>::operator=( LRUCacheItem && other )
{
    _next = nullptr;
    _prev = nullptr;
    _key = std::move( other._key );
    _value = std::move( other._value );
    return *this;
}


template<typename KT, typename VT>
LRUCacheItem<KT, VT>::LRUCacheItem( LRUCacheItem * next, LRUCacheItem * prev )
    : _next( next )
    , _prev( prev )
{
}

template<typename KT, typename VT>
void LRUCacheItem<KT, VT>::link( LRUCacheItem & head )
{
    _next = &head;
    _prev = head._prev;
    head._prev->_next = this;
    head._prev = this;
}

template<typename KT, typename VT>
void LRUCacheItem<KT, VT>::unlink()
{
    _prev->_next = _next;
    _next->_prev = _prev;
}

template<typename KT, typename VT>
void LRUCacheItem<KT, VT>::moveTo( LRUCacheItem & head )
{
    unlink();
    this->_next = &head;
    this->_prev = head._prev;
    head._prev->_next = this;
    head._prev = this;
}







template<typename KT, typename VT>
LRUCache<KT, VT>::LRUCache( int limit/* = 100*/ )
    : _limit( limit )
    , _head( &_head, &_head )
{
    _data.reserve( limit + 1 );
}

template<typename KT, typename VT>
template<typename PKT, typename PVT>
std::pair<VT*, bool> LRUCache<KT, VT>::insert( PKT && key, PVT && value, bool override )
{
    std::pair<VT*, bool> result;
    auto r = _data.insert( std::forward<PKT>( key ), IT( key, std::forward<PVT>( value ) ), override );
    if( r.second )
    {
        r.first->value.link( _head );
        if( (int)_data.size() > _limit )
        {
            evict();
        }
    }
    result.first = &r.first->value._value;
    result.second = r.second;
    return result;
}

template<typename KT, typename VT>
VT* LRUCache<KT, VT>::find( KT const & key )
{
    auto p = _data.find( key );
    if( !p ) return nullptr;
    p->value.moveTo( _head );
    return &p->value._value;
}

template<typename KT, typename VT>
void LRUCache<KT, VT>::clear()
{
    _data.clear();
}

template<typename KT, typename VT>
void LRUCache<KT, VT>::dump()
{
    for( int i = 0; i < _data.size(); ++i )
    {
        std::cout << _data.data()[ i ]->key << std::endl;
    }
    std::cout << std::endl;
}

template<typename KT, typename VT>
void LRUCache<KT, VT>::evict()
{
    auto lastItem = _head._next;
    lastItem->unlink();
    _data.erase( lastItem->_key );
}


#endif
