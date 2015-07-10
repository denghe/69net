#pragma once

namespace xxx
{


    template<typename KT, typename VT>
    template<typename PKT, typename PVT>
    LRUCacheItem<KT, VT>::LRUCacheItem( PKT &&k, PVT &&v )
        : next( nullptr )
        , prev( nullptr )
        , key( std::forward<PKT>( k ) )
        , value( std::forward<PVT>( v ) )
    {
    }

    template<typename KT, typename VT>
    LRUCacheItem<KT, VT>::LRUCacheItem( LRUCacheItem const &o )
        : next( nullptr )
        , prev( nullptr )
        , key( o.key )
        , value( o.value )
    {
    }

    template<typename KT, typename VT>
    LRUCacheItem<KT, VT>::LRUCacheItem( LRUCacheItem &&o )
        : next( nullptr )
        , prev( nullptr )
        , key( std::move( o.key ) )
        , value( std::move( o.value ) )
    {
    }

    template<typename KT, typename VT>
    LRUCacheItem<KT, VT>& LRUCacheItem<KT, VT>::operator=( LRUCacheItem const &o )
    {
        next = nullptr;
        prev = nullptr;
        key = o.key;
        value = o.value;
        return *this;
    }

    template<typename KT, typename VT>
    LRUCacheItem<KT, VT>& LRUCacheItem<KT, VT>::operator=( LRUCacheItem &&o )
    {
        next = nullptr;
        prev = nullptr;
        key = std::move( o.key );
        value = std::move( o.value );
        return *this;
    }


    template<typename KT, typename VT>
    LRUCacheItem<KT, VT>::LRUCacheItem( LRUCacheItem *_next, LRUCacheItem *_prev )
        : next( _next )
        , prev( _prev )
    {
    }

    template<typename KT, typename VT>
    void LRUCacheItem<KT, VT>::Link( LRUCacheItem &_head )
    {
        next = &_head;
        prev = _head.prev;
        _head.prev->next = this;
        _head.prev = this;
    }

    template<typename KT, typename VT>
    void LRUCacheItem<KT, VT>::Unlink()
    {
        prev->next = next;
        next->prev = prev;
    }

    template<typename KT, typename VT>
    void LRUCacheItem<KT, VT>::MoveTo( LRUCacheItem &_head )
    {
        Unlink();
        this->next = &_head;
        this->prev = _head.prev;
        _head.prev->next = this;
        _head.prev = this;
    }







    template<typename KT, typename VT>
    LRUCache<KT, VT>::LRUCache( int _limit/* = 100*/ )
        : limit( _limit )
        , head( &head, &head )
    {
        data.Reserve( _limit + 1 );
    }

    template<typename KT, typename VT>
    template<typename PKT, typename PVT>
    std::pair<VT*, bool> LRUCache<KT, VT>::Insert( PKT &&k, PVT &&v, bool override )
    {
        std::pair<VT*, bool> result;
        auto r = data.Insert( std::forward<PKT>( k ), IT( k, std::forward<PVT>( v ) ), override );
        if( r.second )
        {
            r.first->value.Link( head );
            if( (int)data.Size() > limit )
            {
                Evict();
            }
        }
        result.first = &r.first->value.value;
        result.second = r.second;
        return result;
    }

    template<typename KT, typename VT>
    VT* LRUCache<KT, VT>::Find( KT const& k )
    {
        auto p = data.Find( k );
        if( !p ) return nullptr;
        p->value.MoveTo( head );
        return &p->value.value;
    }

    template<typename KT, typename VT>
    void LRUCache<KT, VT>::Clear()
    {
        data.Clear();
    }

    template<typename KT, typename VT>
    void LRUCache<KT, VT>::Dump()
    {
        for( int i = 0; i < data.Size(); ++i )
        {
            std::cout << data.Data()[ i ]->key << std::endl;
        }
        std::cout << std::endl;
    }

    template<typename KT, typename VT>
    void LRUCache<KT, VT>::Evict()
    {
        auto lastItem = head.next;
        lastItem->Unlink();
        data.Erase( lastItem->key );
    }


}
