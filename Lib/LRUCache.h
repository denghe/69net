#pragma once

namespace xxx
{

    template<typename KT, typename VT>
    class LRUCache;

    template<typename KT, typename VT>
    class LRUCacheItem
    {
    public:
        template<typename PKT, typename PVT>
        LRUCacheItem( PKT&& k, PVT&& v );
        LRUCacheItem( LRUCacheItem const& o );
        LRUCacheItem( LRUCacheItem&& o );
        LRUCacheItem& operator=( LRUCacheItem const& o );
        LRUCacheItem& operator=( LRUCacheItem&& o );

        friend LRUCache < KT, VT > ;
    private:
        LRUCacheItem( LRUCacheItem* _next, LRUCacheItem* _prev );
        void Link( LRUCacheItem& _head );
        void Unlink();
        void MoveTo( LRUCacheItem& _head );

        LRUCacheItem*   next;
        LRUCacheItem*   prev;
        KT              key;
        VT              value;
    };


    template<typename KT, typename VT>
    class LRUCache
    {
    public:
        typedef LRUCacheItem<KT, VT> IT;
        LRUCache( int _limit = 100 );                                                        // _limit 为长度限定. 超限将移除
        template<typename PKT, typename PVT>
        std::pair<VT*, bool> Insert( PKT&& k, PVT&& v, bool override = true );        // 返回指向 data 中存储 value 的地址 和  true( add成功 ), false( 已存在 )
        VT* Find( KT const& k );                                                          // 相当于 Dict Find, 顺便提升排名, 防移除
        void Clear();
        void Dump();

    private:
        void Evict();                                                                       // 移除最后一个

        int                     limit;
        IT                      head;
        Dict<KT, IT>            data;
    };

}
