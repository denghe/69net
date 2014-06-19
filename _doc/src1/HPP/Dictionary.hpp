#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
namespace Lib3 {



// 初始化默认长度Buffer (供构造函数调用)
template <typename TKey, typename TValue>
void Dictionary<TKey, TValue>::Init() {
    _buffer              = _originalBuffer;
    _primeIndex          = Dictionary_OriginalPrimeIndex;
    _length              = Dictionary_OriginalNodeLength;
    _hashNodeHeadMap     = (Node**)(_buffer + sizeof(Node) * Dictionary_OriginalNodeLength);
    memset(_hashNodeHeadMap, 0, sizeof(Node*) * Dictionary_OriginalNodeLength);     // 初始化映射表
    var p = (Node*)_buffer;                                              // 初始化 FreeNodeHead 链表
    for (UInt32 i = 0; i < Dictionary_OriginalNodeLength; ++i)
        p[i].Next       = p + i + 1;                                    // 前一个的 Next 指向下一个
    p[Dictionary_OriginalNodeLength - 1].Next     = NULL;                          // 最后一个指向 NULL
    _freeNodeHead        = p;                                            // 指向链表头
    _usedNodeHead        = NULL;                                         // 已分配链表默认为 NULL
    _count               = 0;
}

// 初始化指定长度Buffer (供构造函数调用)
template <typename TKey, typename TValue>
INLINE void Dictionary<TKey, TValue>::Init(UInt32 const &len) {
    Init();                                                             // 先初始化自带 Buffer ，下面的代码再扩容
    Resize(len);
}

// 这个 Resize 只适用于无数据态 (给 Init(len) 和 operator= 用)
template <typename TKey, typename TValue>
void Dictionary<TKey, TValue>::Resize(UInt32 const &len) {
    UInt32 len_ = len;
    for (UInt32 i = _primeIndex + 1; i < 25; ++i) {                      // 从质数表中找出最接近 len 且大于 len 的质数
        if (len_ <= Math::Table_DictPrimes[i]) {
            len_ = Math::Table_DictPrimes[i];                            // 赋予 len
            _primeIndex  = i;                                            // 存下质数索引
            break;
        }
    }
    var idxAreaNodeLen  = sizeof(Node*) * _length / sizeof(Node);        // 计算 HashNodeHeadMap 内存区域能存放多少个节点
    var newLen          = len_ - _length - idxAreaNodeLen;                // 计算需要创建多大的新 Buffer (对于接近双倍成长的情况来讲应该不存在 newLen 为0或负的情况
    if (idxAreaNodeLen) {                                                // 初始化 HashNodeHeadMap 内存区域并加入链表 (如果有的话)
        var n = (Node*)_hashNodeHeadMap;
        for (UInt32 i = 0; i < idxAreaNodeLen; ++i)
            n[i].Next = n + i + 1;
        n[idxAreaNodeLen - 1].Next = _freeNodeHead;
        _freeNodeHead = n;
    }
    var bak_buf         = _buffer;
    _buffer              = new Byte[sizeof(Byte*) + sizeof(Node) * newLen + sizeof(Node*) * len_];    // 创建新缓存
    _length              = len_;
    _hashNodeHeadMap     = (Node**)(_buffer + sizeof(Byte*) + sizeof(Node) * newLen);
    *(size_t*)_buffer    = (size_t)bak_buf;                              // Buffer 最前面放的是指向下一个内存块的指针
    memset(_hashNodeHeadMap, 0, sizeof(Node*) * len_);                   // 初始化映射表
    var p = (Node*)(_buffer + sizeof(Byte*));                            // 初始化 FreeNodeHead 链表
    for (UInt32 i = 0; i < newLen; ++i)
        p[i].Next       = p + i + 1;
    p[newLen - 1].Next  = _freeNodeHead;
    _freeNodeHead        = p;
}

// 取得新的内存尺寸, 请求1个内存块，链接，扩展 FreeNodeHead 链表，同步 Length, PrimeIndex, 重新计算 Hash映射关系 到新的 HashNodeHeadMap
template <typename TKey, typename TValue>
void Dictionary<TKey, TValue>::Resize() {
    var newtotallen     = Math::Table_DictPrimes[++_primeIndex];
    var idxAreaNodeLen  = sizeof(Node*) * _length / sizeof(Node);
    var newlen          = newtotallen - _length - idxAreaNodeLen;
    var buf             = new Byte[sizeof(Byte*) + sizeof(Node) * newlen + sizeof(Node*) * newtotallen];
    *(size_t*)buf       = (size_t)_buffer;
    _buffer              = buf;
    _length              = newtotallen;
    if (idxAreaNodeLen) {
        var n = (Node*)_hashNodeHeadMap;
        for (UInt32 i = 0; i < idxAreaNodeLen; ++i)
            n[i].Next = n + i + 1;
        n[idxAreaNodeLen - 1].Next = _freeNodeHead;
        _freeNodeHead = n;
    }
    var p               = (Node*)(buf + sizeof(Byte*));
    for (UInt32 i = 0; i < newlen; ++i)
        p[i].Next       = p + i + 1;
    p[newlen - 1].Next  = _freeNodeHead;
    _freeNodeHead        = p;

    var newmap          = (Node**)(p + newlen);
    memset(newmap, 0, sizeof(Node*) * newtotallen);
    _hashNodeHeadMap     = newmap;

    var node            = _usedNodeHead;
Lab1:
    node->Next          = NULL;
    var idx             = node->HashCode % newtotallen;
    if (var n = newmap[idx]) {
        for (; n->Next != NULL; n = n->Next) {};                        // 定位到最后一个节点
        n->Next = node;
    }
    else
        newmap[idx]     = node;
    if ((node = node->UsedNext))
        goto Lab1;
}


template <typename TKey, typename TValue>
INLINE Dictionary<TKey, TValue>::Dictionary() {                                // 至少创建含有 Dictionary_OriginalNodeLength 个元素存放空间的 Buffer
    Init();
}

template <typename TKey, typename TValue>
INLINE Dictionary<TKey, TValue>::Dictionary(UInt32 const &len) {
    assert ( sizeof(Byte*) + sizeof(Node) * len + sizeof(Node*) * len <= 0xFFFFFFFFu ); // x64下最多允许一次 new 出 4g 的内存空间
    if (len <= Dictionary_OriginalNodeLength)
        Init();
    Init(len);
}

template <typename TKey, typename TValue>
Dictionary<TKey, TValue>::~Dictionary() {
    if (var p = _usedNodeHead) {                                         // 依次回收已使用的节点( 调 key & value 的析构 )
Lab2:
        p->Key.~TKey();
        p->Value.~TValue();
        if ((p = p->UsedNext))
            goto Lab2;
    }
    var p = _buffer;                                                     // 依次删除所有 Buffer
Lab1:
    if (p != _originalBuffer) {
        var tmp = *(Byte**)p;                                           // 取该内存段头部的地址
        delete [] p;
        p = tmp;
        goto Lab1;
    }
}

// 不清内存, 只是调已有数据的析构, 并将已有数据的节点挪到 FreeNodeHead 链
template <typename TKey, typename TValue>
void Dictionary<TKey, TValue>::Clear() {
    if (var p = _usedNodeHead) {
        var fnh = _freeNodeHead;
Lab1:
        p->Key.~TKey();
        p->Value.~TValue();
        p->Next = fnh;
        fnh = p;
        if ((p = p->UsedNext))
            goto Lab1;

        _freeNodeHead = fnh;
        _usedNodeHead = NULL;
        _count        = 0;
        memset(_hashNodeHeadMap, 0, sizeof(Node*) * _length);
    }
}

template <typename TKey, typename TValue>
INLINE Dictionary<TKey, TValue>::Dictionary(Dictionary<TKey, TValue> const &dict) {
    Init();
    operator=(dict);
}

template <typename TKey, typename TValue>
Dictionary<TKey, TValue> & Dictionary<TKey, TValue>::operator=(Dictionary<TKey, TValue> const &dict) {
    Clear();
    if (dict._count == 0)
        return *this;
    else {
        _count = dict._count;
        if (dict._count > _length)
            Resize(dict._count);
        if (var p = dict._usedNodeHead) {
Lab2:                                                                   // 下面的代码复制的 Add 里的, Hash 计算变为取节点属性, 键值变为取节点属性, 去掉了空间不足, Replace 检测
            var num = p->HashCode;
            var index = num % _length;
            var node = _hashNodeHeadMap[index];
            if (node) {
Lab1:
                if (node->Next) {
                    node = node->Next;
                    goto Lab1;
                }                                                        
                var n                   = _freeNodeHead;      
                _freeNodeHead            = _freeNodeHead->Next;
                node->Next              = n;                 
                n->Next                 = NULL;              
                n->UsedPrev             = NULL;              
                n->UsedNext             = _usedNodeHead;      
                _usedNodeHead->UsedPrev  = n;                 
                _usedNodeHead            = n;                 
                n->HashCode             = num;        
                new (&n->Key) TKey(p->Key);                  
                new (&n->Value) TValue(p->Value);
            }
            else {
                var n                   = _freeNodeHead;
                _freeNodeHead            = _freeNodeHead->Next;
                n->Next                 = NULL;
                n->UsedPrev             = NULL;
                n->UsedNext             = _usedNodeHead;
                if (_usedNodeHead)                            
                    _usedNodeHead->UsedPrev  = n;
                _usedNodeHead            = n;
                n->HashCode             = num;
                new (&n->Key) TKey(p->Key);
                new (&n->Value) TValue(p->Value);
                _hashNodeHeadMap[index]  = n;                 
            }

            if ((p = p->UsedNext))                                        // 继续添加下一个节点
                goto Lab2;
        }
        return *this;
    }
}

template <typename TKey, typename TValue>
TValue * Dictionary<TKey, TValue>::operator[](TKey const &key) const {
    var code            = Math::GetHashCode(key);
    var idx             = code % _length;
    var node            = _hashNodeHeadMap[idx];
    if (node == NULL)
        return NULL;
Lab1:
    if (node->HashCode == code && node->Key == key)
        return &node->Value;
    if (node->Next) {
        node = node->Next;
        goto Lab1;
    }
    return NULL;
}



// 添加 k & v 到字典。replace 为 true 时, 替换已存在。返回 key 是否已存在
template <typename TKey, typename TValue>
bool Dictionary<TKey, TValue>::Add(TKey const &key, TValue const &value, Boolean const &replace) {
    var num = Math::GetHashCode(key);                                   // 算哈希
    var index = num % _length;                                           // 得到映射入口
    var node = _hashNodeHeadMap[index];                                  // 得到节点(链表头)
    if (node) {                                                         // 如果节点链表已存在, 扫描链表, 判断是否已存在，是否替换并替换
Lab1:
        if ((node->HashCode == num) && (node->Key == key)) {
            if (replace == false)
                return true;
            node->Value = value;
            return false;
        }
        if (node->Next) {
            node = node->Next;
            goto Lab1;
        }                                                               // 扫描完毕, node 指向链表最后一个节点
        if (_freeNodeHead == NULL)                                       // 如果没有空节点则 Resize
            Resize();
        var n                   = _freeNodeHead;                         // 取空闲节点头
        _freeNodeHead            = _freeNodeHead->Next;                   // 令头指向下一个空闲节点
        node->Next              = n;                                    // 接入链表尾
        n->Next                 = NULL;                                 // 下一个同头节点为空
        n->UsedPrev             = NULL;                                 // 上一个已分配节点为空
        n->UsedNext             = _usedNodeHead;                         // 下一个已分配节点指到当前已分配节点头
        _usedNodeHead->UsedPrev  = n;                                    // 当前已分配节点链头的上一个节点指向该节点
        _usedNodeHead            = n;                                    // 令该节点成为链头
        n->HashCode             = num;                                  // 保存 hash 值
        new (&n->Key) TKey(key);                                        // 复制键值数据到节点
        new (&n->Value) TValue(value);
        ++_count;
        return false;
    }
    else {
        if (_freeNodeHead == NULL)
            Resize();
        var n                   = _freeNodeHead;
        _freeNodeHead            = _freeNodeHead->Next;
        n->Next                 = NULL;
        n->UsedPrev             = NULL;
        n->UsedNext             = _usedNodeHead;
        if (_usedNodeHead)                                               // 判断下当前已分配节点链头是不是空的。不空才链接
            _usedNodeHead->UsedPrev  = n;
        _usedNodeHead            = n;
        n->HashCode             = num;
        new (&n->Key) TKey(key);
        new (&n->Value) TValue(value);
        _hashNodeHeadMap[index]  = n;                                    // 登记到 HashNodeHeadMap
        ++_count;
        return false;
    }
}
// 添加　函数的　右值版
template <typename TKey, typename TValue>
bool Dictionary<TKey, TValue>::Add(TKey &&key, TValue &&value, Boolean const &replace) {
    var num = Math::GetHashCode(key);                   
    var index = num % _length;                                    
    var node = _hashNodeHeadMap[index];                           
    if (node) {                                                  
Lab1:
        if ((node->HashCode == num) && (node->Key == key)) {
            if (replace == false)
                return true;
            node->Value = value;
            return false;
        }
        if (node->Next) {
            node = node->Next;
            goto Lab1;
        }                                                        
        if (_freeNodeHead == NULL)                                
            Resize();
        var n                   = _freeNodeHead;                  
        _freeNodeHead            = _freeNodeHead->Next;            
        node->Next              = n;                             
        n->Next                 = NULL;                          
        n->UsedPrev             = NULL;                          
        n->UsedNext             = _usedNodeHead;                  
        _usedNodeHead->UsedPrev  = n;                             
        _usedNodeHead            = n;                             
        n->HashCode             = num;
        new (&n->Key) TKey((TKey &&)key);                               // 调用右值版复制构造
        new (&n->Value) TValue((TValue &&)value);
        ++_count;
        return false;
    }
    else {
        if (_freeNodeHead == NULL)
            Resize();
        var n                   = _freeNodeHead;
        _freeNodeHead            = _freeNodeHead->Next;
        n->Next                 = NULL;
        n->UsedPrev             = NULL;
        n->UsedNext             = _usedNodeHead;
        if (_usedNodeHead)
            _usedNodeHead->UsedPrev  = n;
        _usedNodeHead            = n;
        n->HashCode             = num;
        new (&n->Key) TKey((TKey &&)key);
        new (&n->Value) TValue((TValue &&)value);
        _hashNodeHeadMap[index]  = n;
        ++_count;
        return false;
    }
}
// 添加　函数的　右值版
template <typename TKey, typename TValue>
bool Dictionary<TKey, TValue>::Add(TKey const &key, TValue &&value, Boolean const &replace) {
    var num = Math::GetHashCode(key);                   
    var index = num % _length;                                    
    var node = _hashNodeHeadMap[index];                           
    if (node) {                                                  
Lab1:
        if ((node->HashCode == num) && (node->Key == key)) {
            if (replace == false)
                return true;
            node->Value = value;
            return false;
        }
        if (node->Next) {
            node = node->Next;
            goto Lab1;
        }                                                        
        if (_freeNodeHead == NULL)                                
            Resize();
        var n                   = _freeNodeHead;                  
        _freeNodeHead            = _freeNodeHead->Next;            
        node->Next              = n;                             
        n->Next                 = NULL;                          
        n->UsedPrev             = NULL;                          
        n->UsedNext             = _usedNodeHead;                  
        _usedNodeHead->UsedPrev  = n;                             
        _usedNodeHead            = n;                             
        n->HashCode             = num;
        new (&n->Key) TKey(key);
        new (&n->Value) TValue((TValue &&)value);
        ++_count;
        return false;
    }
    else {
        if (_freeNodeHead == NULL)
            Resize();
        var n                   = _freeNodeHead;
        _freeNodeHead            = _freeNodeHead->Next;
        n->Next                 = NULL;
        n->UsedPrev             = NULL;
        n->UsedNext             = _usedNodeHead;
        if (_usedNodeHead)
            _usedNodeHead->UsedPrev  = n;
        _usedNodeHead            = n;
        n->HashCode             = num;
        new (&n->Key) TKey(key);
        new (&n->Value) TValue((TValue &&)value);
        _hashNodeHeadMap[index]  = n;
        ++_count;
        return false;
    }
}
// 添加　函数的　右值版
template <typename TKey, typename TValue>
bool Dictionary<TKey, TValue>::Add(TKey &&key, TValue const &value, Boolean const &replace) {
    var num = Math::GetHashCode(key);                   
    var index = num % _length;                                    
    var node = _hashNodeHeadMap[index];                           
    if (node) {                                                  
Lab1:
        if ((node->HashCode == num) && (node->Key == key)) {
            if (replace == false)
                return true;
            node->Value = value;
            return false;
        }
        if (node->Next) {
            node = node->Next;
            goto Lab1;
        }                                                        
        if (_freeNodeHead == NULL)                                
            Resize();
        var n                   = _freeNodeHead;                  
        _freeNodeHead            = _freeNodeHead->Next;            
        node->Next              = n;                             
        n->Next                 = NULL;                          
        n->UsedPrev             = NULL;                          
        n->UsedNext             = _usedNodeHead;                  
        _usedNodeHead->UsedPrev  = n;                             
        _usedNodeHead            = n;                             
        n->HashCode             = num;
        new (&n->Key) TKey((TKey &&)key);                               // 调用右值版复制构造
        new (&n->Value) TValue(value);
        ++_count;
        return false;
    }
    else {
        if (_freeNodeHead == NULL)
            Resize();
        var n                   = _freeNodeHead;
        _freeNodeHead            = _freeNodeHead->Next;
        n->Next                 = NULL;
        n->UsedPrev             = NULL;
        n->UsedNext             = _usedNodeHead;
        if (_usedNodeHead)
            _usedNodeHead->UsedPrev  = n;
        _usedNodeHead            = n;
        n->HashCode             = num;
        new (&n->Key) TKey((TKey &&)key);
        new (&n->Value) TValue(value);
        _hashNodeHeadMap[index]  = n;
        ++_count;
        return false;
    }
}

// 从字典中移除 key 项, 返回该项是否存在
template <typename TKey, typename TValue>
Boolean Dictionary<TKey, TValue>::Remove(TKey const &key) {
    var code            = Math::GetHashCode(key);
    var idx             = code % _length;
    var node            = _hashNodeHeadMap[idx];
    if (node == NULL)
        return false;
                                                                        // 扫链表，判断 HashCode 和 key 是否同时相等
    Node *prevnode      = NULL;                                         // 存上一个节点, 以便修改其 Next 指向下下节点
Lab1:
    if (node->HashCode == code && node->Key == key) {                   // 如果找到　则将该节点从当前链中移除
        if (prevnode)
            prevnode->Next = node->Next;
        else
            _hashNodeHeadMap[idx] = NULL;
        if (_usedNodeHead == node) {                                     // 令 node 从 UsedNodeHead 链移除
            _usedNodeHead = node->UsedNext;
            _usedNodeHead->UsedPrev = NULL;
        }
        else {
            node->UsedPrev->UsedNext = node->UsedNext;
            node->UsedNext->UsedPrev = node->UsedPrev;
        }
        node->Next = _freeNodeHead;                                      // 将 node 并到 FreeNodeHead
        _freeNodeHead = node;
            
        node->Key.~TKey();                                              // 析构数据
        node->Value.~TValue();

        --_count;
        return true;
    }
    if (node->Next) {
        prevnode = node;
        node = node->Next;
        goto Lab1;
    }
    return false;
}

// 根据键来取值，返回是否存在该项 (存在则复制 Value 到 outValue)
template <typename TKey, typename TValue>
Boolean Dictionary<TKey, TValue>::Get(TKey const &key, TValue &outValue) const {
    var code            = Math::GetHashCode(key);
    var idx             = code % _length;
    var node            = _hashNodeHeadMap[idx];
    if (node == NULL)
        return false;
Lab1:
    if (node->HashCode == code && node->Key == key) {
        outValue = node->Value;
        return true;
    }
    if (node->Next) {
        node = node->Next;
        goto Lab1;
    }
    return false;
}

// 从字典中移除 key 项, 返回该项是否存在 (存在则复制 Value 到 outValue)
template <typename TKey, typename TValue>
Boolean Dictionary<TKey, TValue>::GetAndRemove(TKey const &key, TValue &outValue) {
    var code            = Math::GetHashCode(key);
    var idx             = code % _length;
    var node            = _hashNodeHeadMap[idx];
    if (node == NULL)
        return false;
    Node *prevnode      = NULL;
Lab1:
    if (node->HashCode == code && node->Key == key) {
        if (prevnode)
            prevnode->Next = node->Next;
        else
            _hashNodeHeadMap[idx] = NULL;
        if (_usedNodeHead == node) {
            _usedNodeHead = node->UsedNext;
            _usedNodeHead->UsedPrev = NULL;
        }
        else {
            node->UsedPrev->UsedNext = node->UsedNext;
            node->UsedNext->UsedPrev = node->UsedPrev;
        }
        node->Next = _freeNodeHead;
        _freeNodeHead = node;
            
        outValue = node->Value;                                         // 复制值到输出变量

        node->Key.~TKey();
        node->Value.~TValue();

        --_count;
        return true;
    }
    if (node->Next) {
        prevnode = node;
        node = node->Next;
        goto Lab1;
    }
    return false;
}

// 判断是否包含某 key
template <typename TKey, typename TValue>
Boolean Dictionary<TKey, TValue>::ContainsKey(TKey const &key) const {
    var code            = Math::GetHashCode(key);
    var idx             = code % _length;
    var node            = _hashNodeHeadMap[idx];
    if (node == NULL)
        return false;
Lab1:
    if (node->HashCode == code && node->Key == key)
        return true;
    if (node->Next) {
        node = node->Next;
        goto Lab1;
    }
    return false;
}

template <typename TKey, typename TValue>
INLINE typename Lib3::Dictionary<TKey, TValue>::Node * const & Dictionary<TKey, TValue>::UsedNodeHead() const {
    return _usedNodeHead;
}

template <typename TKey, typename TValue>
INLINE UInt32 const & Dictionary<TKey, TValue>::Count() const {
    return _count;
}





}
#endif
