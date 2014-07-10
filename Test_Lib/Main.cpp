#include "Lib/All.h"
using namespace std;



// 块式扩容的先进先出流( 主用于发数据啥的 )
class LinkedBuffer
{
public:
    explicit LinkedBuffer( Pool& p );
    ~LinkedBuffer();
    void write( char const* buf, int len );
    int read( char* buf, int bufLen );         // 试复制指定长度到 buf 并移动读指针. 返回实际复制的长度
    void copy( char* buf, int len );            // 复制指定长度到 buf, 如果 size 不足会出错
    void clear();
    bool empty();
    int size();                 // 未读之数据长
private:
    struct Page
    {
        Page*   next;
        char    data[ 1 ];
    };
    Page*       _wp;            // 正用于写的页
    Page*       _rp;            // 正用于读的页
    int         _wpLen;         // 正用于写的页的已写长度
    int         _rpLen;         // 正用于读的页的已读长度
    int         _size;          // 正读页剩余长度 + 正写页已写长度 + 中间跨过的页数 * 页size
    Pool&       _pool;
};

LinkedBuffer::LinkedBuffer( Pool& p )
    : _wp( nullptr )
    , _rp( nullptr )
    , _wpLen( 0 )
    , _rpLen( 0 )
    , _size( 0 )
    , _pool( p )
{
}

LinkedBuffer::~LinkedBuffer()
{
    clear();
}

void LinkedBuffer::clear()
{
    Page* p = _wp;
    while( p )
    {
        _pool.free( p );
        p = p->next;
    }
    _wp = _rp = nullptr;
    _wpLen = _rpLen = _size = 0;
}

void LinkedBuffer::write( char const* buf, int len )
{
    assert( buf );
    if( !len ) return;
    _size += len;

    int ps = _pool.pageBufLen() - sizeof( Page* );
    if( !_wp ) _wp = (Page*)_pool.alloc();
    if( len <= ps ) goto CopyLast;

CopyPage:
    memcpy( _wp->data, buf, ps );
    len -= ps;
    buf += ps;
    if( len >= ps )
    {
        _wp->next = (Page*)_pool.alloc();
        _wp = _wp->next;
        goto CopyPage;
    }
CopyLast:
    _wpLen = len;
    _wp->next = nullptr;
    if( len ) memcpy( _wp->data, buf, len );
}

void LinkedBuffer::copy( char* buf, int len )
{
    assert( len <= _size );

}

int LinkedBuffer::read( char* buf, int bufLen )
{
    // ...
    return 0;
}







template<typename KT, typename VT>
class LRUCache;


template<typename KT, typename VT>
class LRUCacheItem
{
public:
    template<typename PKT, typename PVT>
    LRUCacheItem( PKT && key, PVT && value )        // 放入容器之后再 link. 不然 head 的 prev next 指向的地址可能不正确
        : _next( nullptr )                          // 故意令指针为空, 因为复制后( 例如放入容器 ) 需要 link 到链表才有意义
        , _prev( nullptr )
        , _key( forward<PKT>( key ) )
        , _value( forward<PVT>( value ) )
    {
    }

    LRUCacheItem( LRUCacheItem const & other )
        : _next( nullptr )
        , _prev( nullptr )
        , _key( other._key )
        , _value( other._value )
    {
    }

    LRUCacheItem( LRUCacheItem && other )
        : _next( nullptr )
        , _prev( nullptr )
        , _key( move( other._key ) )
        , _value( move( other._value ) )
    {
    }

    LRUCacheItem & operator=( LRUCacheItem const & other )
    {
        _next = nullptr;
        _prev = nullptr;
        _key = other._key;
        _value = other._value;
    }

    LRUCacheItem & operator=( LRUCacheItem && other )
    {
        _next = nullptr;
        _prev = nullptr;
        _key = move( other._key );
        _value = move( other._value );
    }

    friend LRUCache < KT, VT > ;
private:
    LRUCacheItem( LRUCacheItem * next, LRUCacheItem * prev )
        : _next( next )
        , _prev( prev )
        //, _key( KT() )            // 似乎不必初始化
        //, _value( VT() )
    {
    }

    void link( LRUCacheItem & head )
    {
        _next = &head;
        _prev = head._prev;
        head._prev->_next = this;
        head._prev = this;
    }

    void unlink()
    {
        _prev->_next = _next;
        _next->_prev = _prev;
    }

    void moveTo( LRUCacheItem & head )
    {
        unlink();
        this->_next = &head;
        this->_prev = head._prev;
        head._prev->_next = this;
        head._prev = this;
    }

    LRUCacheItem *  _next;
    LRUCacheItem *  _prev;
    KT              _key;
    VT              _value;
};


template<typename KT, typename VT>
class LRUCache
{
public:
    typedef LRUCacheItem<KT, VT> ITEM_T;

    // limit 为长度限定. 超限将移除
    LRUCache( int limit = 100 )
        : _limit( limit )
        , _head( &_head, &_head )
    {
        _data.reserve( limit + 1 );
    }

    // add. 返回指向 _data 中存储 value 的地址 和  true( add成功 ), false( 已存在 )
    template<typename PKT, typename PVT>
    pair<VT*, bool> add( PKT && key, PVT && value, bool override = true )
    {
        pair<VT*, bool> rtv;
        auto it = _data.insert( make_pair( forward<PKT>( key ), ITEM_T( key, forward<PVT>( value ) ) ) );
        if( it.second )
        {
            it.first->second.link( _head );
            if( (int)_data.size() > _limit )
            {
                evict();
            }
            rtv.second = false;
        }
        else if( override ) // 覆盖
        {
            it.first->second._value = forward<PVT>( value );
            rtv.second = true;
        }
        rtv.first = &it.first->second._value;
        return rtv;
    }

    // get. 顺便提升排名, 防移除
    VT * get( KT const & key )
    {
        auto it = _data.find( key );
        if( it == _data.end() )
        {
            return nullptr;
        }
        it->second.moveTo( _head );
        return &it->second._value;
    }

    // clear
    void clear()
    {
        _data.clear();
    }

    // dump
    void dump()
    {
        for( auto &o : _data ) cout << o.first << endl;
        cout << endl;
    }

    // todo: 缺批量移除? 缺 _data 暴露?

private:
    // 移除最后一个
    void evict()
    {
        auto lastItem = _head._next;
        lastItem->unlink();
        _data.erase( lastItem->_key );
    }

    int                         _limit;
    ITEM_T                      _head;
    unordered_map<KT, ITEM_T>   _data;
};






int main()
{

    return 0;
}
