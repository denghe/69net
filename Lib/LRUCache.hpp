#ifndef _LRUCACHE_HPP__
#define _LRUCACHE_HPP__

namespace xxx
{


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
    void LRUCacheItem<KT, VT>::Link( LRUCacheItem & head )
    {
        _next = &head;
        _prev = head._prev;
        head._prev->_next = this;
        head._prev = this;
    }

    template<typename KT, typename VT>
    void LRUCacheItem<KT, VT>::Unlink()
    {
        _prev->_next = _next;
        _next->_prev = _prev;
    }

    template<typename KT, typename VT>
    void LRUCacheItem<KT, VT>::MoveTo( LRUCacheItem & head )
    {
        Unlink();
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
        _data.Reserve( limit + 1 );
    }

    template<typename KT, typename VT>
    template<typename PKT, typename PVT>
    std::pair<VT*, bool> LRUCache<KT, VT>::Insert( PKT && key, PVT && value, bool override )
    {
        std::pair<VT*, bool> result;
        auto r = _data.Insert( std::forward<PKT>( key ), IT( key, std::forward<PVT>( value ) ), override );
        if( r.second )
        {
            r.first->value.Link( _head );
            if( (int)_data.Size() > _limit )
            {
                Evict();
            }
        }
        result.first = &r.first->value._value;
        result.second = r.second;
        return result;
    }

    template<typename KT, typename VT>
    VT* LRUCache<KT, VT>::Find( KT const & key )
    {
        auto p = _data.Find( key );
        if( !p ) return nullptr;
        p->value.MoveTo( _head );
        return &p->value._value;
    }

    template<typename KT, typename VT>
    void LRUCache<KT, VT>::Clear()
    {
        _data.Clear();
    }

    template<typename KT, typename VT>
    void LRUCache<KT, VT>::Dump()
    {
        for( int i = 0; i < _data.Size(); ++i )
        {
            std::cout << _data.Data()[ i ]->key << std::endl;
        }
        std::cout << std::endl;
    }

    template<typename KT, typename VT>
    void LRUCache<KT, VT>::Evict()
    {
        auto lastItem = _head._next;
        lastItem->Unlink();
        _data.Erase( lastItem->_key );
    }


}

#endif
