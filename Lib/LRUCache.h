#ifndef _LRUCACHE_H__
#define _LRUCACHE_H__

namespace xxx
{

    template<typename KT, typename VT>
    class LRUCache;

    template<typename KT, typename VT>
    class LRUCacheItem
    {
    public:
        template<typename PKT, typename PVT>
        LRUCacheItem( PKT&& key, PVT&& value );
        LRUCacheItem( LRUCacheItem const& other );
        LRUCacheItem( LRUCacheItem&& other );
        LRUCacheItem& operator=( LRUCacheItem const& other );
        LRUCacheItem& operator=( LRUCacheItem&& other );

        friend LRUCache < KT, VT > ;
    private:
        LRUCacheItem( LRUCacheItem* next, LRUCacheItem* prev );
        void Link( LRUCacheItem& head );
        void Unlink();
        void MoveTo( LRUCacheItem& head );

        LRUCacheItem*   _next;
        LRUCacheItem*   _prev;
        KT              _key;
        VT              _value;
    };


    template<typename KT, typename VT>
    class LRUCache
    {
    public:
        typedef LRUCacheItem<KT, VT> IT;
        LRUCache( int limit = 100 );                                                        // limit 为长度限定. 超限将移除
        template<typename PKT, typename PVT>
        std::pair<VT*, bool> Insert( PKT&& key, PVT&& value, bool override = true );        // 返回指向 _data 中存储 value 的地址 和  true( add成功 ), false( 已存在 )
        VT* Find( KT const& key );                                                          // 相当于 Dict Find, 顺便提升排名, 防移除
        void Clear();
        void Dump();

    private:
        void Evict();                                                                       // 移除最后一个

        int                     _limit;
        IT                      _head;
        Dict<KT, IT>            _data;
    };

}

#endif
