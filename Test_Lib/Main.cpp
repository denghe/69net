#include "Lib/All.h"
using namespace std;




class CirclePool
{
public:
    explicit CirclePool( int pageSize = 4096, int capacity = 2 );
    ~CirclePool();
    void insert( char const* buf, int len );
    void pop( int len );
    void copyTo( char* buf, int len );
    void clear();
    int size();
    bool empty();
private:
    struct Page
    {
        Page*   next;
        int     len;
    };
    Page *      _header;
    Page *      _tail;
    int         _headerLen;
    int         _tailLen;
    int         _len;
    Pool        _pool;
};

CirclePool::CirclePool( int blockSize /* = 4096 */, int capacity /* = 2 */ )
{
    _pool.init( sizeof( Page ), sizeof( Page ) * capacity, 2 );
}





/*

#include "Precompile.h"

ListBuffer::ListBuffer( int block_min_size )
: _header( 0 )
, _tail( 0 )
, _header_ptr( 0 )
, _tail_ptr( 0 )
, _size( 0 )
{
    if( block_min_size < (int)NetConfigs::ListBufferMinBlockSize )
    {
        _block_min_size = round2n( (int)NetConfigs::ListBufferMinBlockSize );
    }
    else
    {
        _block_min_size = round2n( block_min_size );
    }
}

ListBuffer::~ListBuffer()
{
    Block *b_to_free;
    while( _header )
    {
        b_to_free = _header;
        _header = _header->next;
        free_block( b_to_free );
    }
}

void ListBuffer::push( const char *buf, int len )
{
    if( len == 0 ) return;

    // allocate first block if the list is empty
    if( !_tail )
    {
        append_block( len );
        _header = _tail;
        _header_ptr = 0;
    }

    // copy data to blocks
    int avail = _tail->size - _tail_ptr;
    int left = len;
    if( avail == 0 )
    {
        append_block( len );
        avail = _tail->size;
    }

    if( left >= avail )
    {
        memcpy( _tail->data() + _tail_ptr, buf, avail );

        buf += avail;
        left -= avail;
        _tail_ptr += avail;

        if( left > 0 ) append_block( left );
    }

    if( left > 0 )
    {
        memcpy( _tail->data() + _tail_ptr, buf, left );
        _tail_ptr += left;
    }

    _size += len;
}

int ListBuffer::copy( char *buf, int len )
{
    if( len > _size ) len = _size;
    if( len == 0 ) return 0;

    Block *b = _header;
    int avail = b->size - _header_ptr;
    char *p = b->data() + _header_ptr;
    int left = len;
    while( left > avail )
    {
        memcpy( buf, p, avail );

        buf += avail;
        left -= avail;

        b = b->next;
        avail = b->size;
        p = b->data();
    }

    if( left > 0 )
    {
        memcpy( buf, p, left );
    }

    return len;
}

int ListBuffer::pop( int len )
{
    if( len > _size ) len = _size;
    if( len == 0 ) return 0;

    int avail = _header->size - _header_ptr;
    int left = len;
    Block *b_to_free;
    while( left >= avail )
    {
        b_to_free = _header;
        _header = _header->next;
        free_block( b_to_free );

        left -= avail;

        if( !_header ) break;
        avail = _header->size;
    }

    if( left > 0 ) _header_ptr += left;

    if( !_header )
    {
        _tail = 0;
        _tail_ptr = 0;
    }

    _size -= len;

    return len;
}

void ListBuffer::append_block( int len )
{
    // adjust length for allocating
    int to_alloc = len + sizeof(int)+sizeof( Block * );

    // alloc block
    to_alloc = round2n( to_alloc );
    if( to_alloc < _block_min_size ) to_alloc = _block_min_size;

    Block *block = (Block *)malloc( to_alloc );

    block->size = len;
    block->next = 0;

    // append
    if( _tail ) _tail->next = block;

    _tail = block;
    _tail_ptr = 0;
}

void ListBuffer::free_block( Block *block )
{
    free( block );
    _header_ptr = 0;
}

int ListBuffer::size() const
{
    return _size;
}

void ListBuffer::clear()
{
    pop( _size );
}

int ListBuffer::empty() const
{
    return _size == 0;
}

char * ListBuffer::Block::data() const
{
    return (char *)&next + sizeof( Block * );
}

*/










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

    friend LRUCache<KT, VT>;
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
