#ifndef _DICT_HPP_
#define _DICT_HPP_

#define DICT_PRIMES_COUNT (sizeof(_primes)/sizeof(_primes[0]))

template <typename TK, typename TV>
Dict<TK, TV>::Dict()
{
    int prime = _primes[ 0 ];
    _nodes.ensure( prime );      // 确保空间足够
    memset( _nodes._buf, 0, sizeof(void*) * prime ); // _len 后面的单元要用来存 已分配但未使用的指针，所以也要初始化
    _buckets.ensure( prime );
    _buckets._len = prime;       // 将桶的空间留出来，并初始化为 0（要用作桶内有没有数据的判断条件，0: 空指针 就是没有）
    memset( _buckets._buf, 0, sizeof(void*) * prime );
    setPrimeIdx( 0 );                   // 存当前质数索引
}

template <typename TK, typename TV>
Dict<TK, TV>::Dict( int capacity )
{
    int prime;
    for( int i = 0; i < DICT_PRIMES_COUNT; ++i )    // 遍历质数表 看看有没有刚好大于等于传入 capacity 的值，有就用之
    {
        prime = _primes[ i ];                       // 如果 for 没找到比 capacity 更大的值，默认将使用最后一个最大质数值
        if( prime >= capacity ) break;              // 找到就跳出
    }
    _nodes.ensure( prime );
    for( int idx = 0; idx < prime; ++idx )
    {
        _nodes._buf[ idx ] = (Node*)malloc( sizeof(Node) ); // 事先创建所有元素容器
    }
    _buckets.ensure( prime );
    _buckets._len = prime;
    memset( _buckets._buf, 0, sizeof(void*) * prime );
    setPrimeIdx( prime );
}

template <typename TK, typename TV>
Dict<TK, TV>::Dict( Dict && o )
{
    _nodes = (List<Node*>&&)o._nodes;
    _buckets = (List<Node*>&&)o._buckets;
    for( int i = _nodes._len; i < _buckets._len; ++i )  // 复制 _nodes._len 到 _buckets._len 这段的已创建但未使用的节点
    {
        if( o._nodes[ i ] ) _nodes[ i ] = o._nodes[ i ];    // 有值就复制，遇到 0 就中断扫描
        else break;
    }
}

template <typename TK, typename TV>
Dict<TK, TV>::~Dict()
{
    for( int i = 0; i < _nodes._len; ++i )  // 释放使用中的节点
    {
        free( _nodes[ i ] );
    }
    for( int i = _nodes._len; i < _buckets._len; ++i )  // 释放已分配但未使用的节点
    {
        if( _nodes[ i ] ) free( _nodes[ i ] );
        else break;
    }
}

template <typename TK, typename TV>
Dict<TK, TV> & Dict<TK, TV>::operator=( Dict && o )
{
    for( int i = 0; i < _nodes._len; ++i )  // 释放使用中的节点
    {
        free( _nodes[ i ] );
    }
    for( int i = _nodes._len; i < _buckets._len; ++i )  // 释放已分配但未使用的节点
    {
        if( _nodes[ i ] ) free( _nodes[ i ] );
        else break;
    }
    _nodes = (List<Node*>&&)o._nodes;       // 这里开始的代码是从 右值复制构造函数里复制过来的
    _buckets = (List<Node*>&&)o._buckets;
    for( int i = _nodes._len; i < _buckets._len; ++i )
    {
        if( o._nodes[ i ] ) _nodes[ i ] = o._nodes[ i ];
        else break;
    }
}

template <typename TK, typename TV>
template <typename TKey, typename TValue>
bool Dict<TK, TV>::add( TKey && k, TValue && v, bool replace /*= false */ )
{
    auto hc = getHashCode( std::forward<TKey>(k) );     // 取 key hashCode
    auto idx = hc % _buckets._len;  // 判断 k 属于哪个桶
    Node *next = nullptr;           // 如果要创建新节点并放入桶中，该值为指向下一节点的指针
    if( auto node = _buckets[ idx ] )  // 如果桶里还有其他节点，则需要逐一比对
    {
LabBegin:               // 如果 hashCode 一致则进一步对比具体 key 值，都一致的话则看是否替换值
        if( node->_hashCode == hc && node->_key == k )
        {
            if( replace )
                node->_value = std::forward<TValue>(v);
            return true;
        }
        if( node->_next )    // 如果还存在下一个节点，用当前节点变量指向，跳转继续扫。
        {
            node = node->_next;
            goto LabBegin;
        }                       // 未发现相同 key 节点，准备走添加流程
        next = _buckets[ idx ];   // 添加的新节点的 next 指向桶的链头
    }
    Node* n;    // 看是从 _nodes  _len 偏移量的后面取还是 new: 判断 _nodes[ _len ] 是否为 nullptr
    if( _nodes[ _nodes._len ] )
        n = _nodes[ _nodes._len ];
    else
        n = (Node*)malloc( sizeof(Node) );
    n->_next = next;
    n->_index = _nodes._len;
    n->_hashCode = hc;
    _nodes[ _nodes._len ] = n;
    _buckets[ idx ] = n;
    new ( &n->_key ) TK( std::forward<TKey>(k) );
    new ( &n->_value ) TV( std::forward<TValue>(v) );
    ++_nodes._len;

    if( _nodes._len == _buckets._len )              // 判断下当前节点数是否已达到桶数，是则扩容
    {
        int prime_idx = getPrimeIdx() + 1;          // 取下一个质数索引
        int prime = _primes[ prime_idx ];           // 取到质数
        _nodes._bufSize = (int)round2n( ( prime ) * sizeof(Node*) );    // 计算扩容尺寸
        _nodes._buf = (decltype(_nodes._buf))realloc( _nodes._buf, _nodes._bufSize );   // 扩容
        memset( _nodes._buf + _nodes._len, 0, (prime - _nodes._len) * sizeof(Node*) );
        _buckets._bufSize = (int)round2n( ( prime ) * sizeof(Node*) );
        _buckets._buf = (decltype(_buckets._buf))realloc( _buckets._buf, _buckets._bufSize );
        memset( _buckets._buf, 0, prime * sizeof(Node*) );  // 重置桶并准备重新分配
        _buckets._len = prime;                      // 设置桶数
        setPrimeIdx( prime_idx );                   // 把当前质数索引写入空闲单元
        for( int i = 0; i < _nodes._len; ++i )      // 将所有已使用的节点重分配到新桶
        {
            n = _nodes[ i ];
            idx = n->_hashCode % prime;             // 余数取模即为桶索引
            n->_next = _buckets[ idx ];             // 相同桶的节点形成链
            _buckets[ idx ] = n;
        }
    }
    return false;
}

template <typename TK, typename TV>
void Dict<TK, TV>::clear()
{
    if( _nodes._len > _buckets._len / 10 )           // 经验数字，怀疑少于 1/10 的数据的情况下，单独设 0 比 memset 高效
    {
        memset( _buckets._buf, 0, _buckets._bufSize );
        _nodes._len = 0;
        return;
    }
    for( int i = 0; i < _nodes._len; ++i )
    {
        _buckets[ _nodes[ i ]->_hashCode % _buckets._len ] = nullptr;
    }
    _nodes._len = 0;
}

template <typename TK, typename TV>
bool Dict<TK, TV>::exists( TK const & k ) const
{
    auto hc = getHashCode( k );             // 以下为从 add 复制并减化的代码
    auto idx = hc % _buckets._len;
    if( auto node = _buckets[ idx ] )
    {
LabBegin:
        if( node->_hashCode == hc && node->_key == k ) return true;
        if(( node = node->_next )) goto LabBegin;
    }
    return false;
}

template <typename TK, typename TV>
bool Dict<TK, TV>::tryGet( TK const & k, TV & v ) const
{
    auto hc = getHashCode( k );             // 以下为从 exists 复制的代码并小改
    auto idx = hc % _buckets._len;
    if( auto node = _buckets[ idx ] )
    {
LabBegin:
        if( node->_hashCode == hc && node->_key == k )
        {
            v = node->_value;       // 复制 _value 到 v
            return true;
        }
        if(( node = node->_next )) goto LabBegin;
    }
    return false;
}

template <typename TK, typename TV>
bool Dict<TK, TV>::tryRemove( TK const & k, TV & v )
{
    auto hc = getHashCode( k );             // 以下为从 exists 复制的代码并小改
    auto idx = hc % _buckets._len;
    Node * parent = nullptr;        // 记录 当前节点 的父 以便移除后 令父->next 指向 node->_next
    if( auto node = _buckets[ idx ] )
    {
LabBegin:
        if( node->_hashCode == hc && node->_key == k )
        {
            v = (TV&&)node->_value; // 移动 _value 到 v
            if( parent )            // 开始移除。如果存在父，则令父与下一级节点链起来
            {
                parent->_next = node->_next;
            }
            else
            {
            	_buckets[ idx ] = nullptr;
            }
            if( node->_index < _nodes._len - 1 )   // 当前节点不是节点列表尾，需要交换
            {
                _nodes[ _nodes._len - 1 ]->_index = node->_index;
                std::swap( _nodes[ _nodes._len - 1 ], _nodes[ node->_index ] );
            }
            --_nodes._len;         // 将节点纳入未分配区
            node->_key.~TK();       // 释放 k, v 的内存
            node->_value.~TV();

            return true;
        }
        if( node->_next )
        {
            parent = node;
            node = node->_next;
            goto LabBegin;
        }
    }
    return false;
}

template <typename TK, typename TV>
bool Dict<TK, TV>::tryRemove( TK const & k )
{
    auto hc = getHashCode( k );             // 以下为从 tryRemove( k, v ) 复制的代码并小改
    auto idx = hc % _buckets._len;
    Node * parent = nullptr;
    if( auto node = _buckets[ idx ] )
    {
LabBegin:
        if( node->_hashCode == hc && node->_key == k )
        {
            // 移动 node->_value 到 v 的代码去掉了
            if( parent )
            {
                parent->_next = node->_next;
            }
            else
            {
            	_buckets[ idx ] = nullptr;
            }
            if( node->_index < _nodes._len - 1 )
            {
                _nodes[ _nodes._len - 1 ]->_index = node->_index;
                std::swap( _nodes[ _nodes._len - 1 ], _nodes[ node->_index ] );
            }
            --_nodes._len;
            node->_key.~TK();
            node->_value.~TV();

            return true;
        }
        if( node->_next )
        {
            parent = node;
            node = node->_next;
            goto LabBegin;
        }
    }
    return false;
}

template <typename TK, typename TV>
int Dict<TK, TV>::getPrimeIdx() const
{
    return *(int*)&_buckets._buf[ _buckets._bufSize / sizeof(Node*) - 1 ];
}

template <typename TK, typename TV>
void Dict<TK, TV>::setPrimeIdx( int idx )
{
    *(int*)&_buckets._buf[ _buckets._bufSize / sizeof(Node*) - 1 ] = idx;
}


#endif
