#ifndef GC_LINKEDBLOCK_HPP
#define GC_LINKEDBLOCK_HPP
namespace Lib3 {
namespace GC {



#ifdef __DEBUG
INLINE void * LinkedBlock::Allocate(UInt32 const &len, SByte const * const &fn, Int32 const &ln) {
    // todo: 记录 filename, line number
    assert ( len <= _nodeSize );
    var newidx = (_index + 7) & 0xFFFFFFF8u;                            // 得到新的内存对齐的 idx
    if (newidx > _nodeSize - _index)                                    // 如果在内存对齐后会超出当前块的剩余空间，就创建新块
        NewNode();
    else _index = newidx;                                               // 调整起始内存地址到对齐
    var rtv = _buffer + _index;                                         // 得到返回指针
    _index += len;                                                      // 移动索引
    return rtv;
}
INLINE void LinkedBlock::Free(void * const &p, SByte const * const &fn, Int32 const &ln) {
    // todo: 记录 filename, line number
}
#else
INLINE void * LinkedBlock::Allocate(UInt32 const &len) {
    var newidx = (_index + 7) & 0xFFFFFFF8u;                            // 得到新的内存对齐的 idx
    if (newidx > _nodeSize - _index)                                    // 如果在内存对齐后会超出当前块的剩余空间，就创建新块
        NewNode();
    else _index = newidx;                                               // 调整起始内存地址到对齐
    var rtv = _buffer + _index;                                         // 得到返回指针
    _index += len;                                                      // 移动索引
    return rtv;
}
INLINE void LinkedBlock::Free(void * const &p) {
}
#endif


// 创建新内存块（在第１块内存块已存在的情况下被调用）
INLINE void LinkedBlock::NewNode() {
    if (_currentNode->Next) {                                           // 如果下个节点已经存在
        _currentNode->Index = _index;                                   // 存数据长度
        _currentNode        = _currentNode->Next;                       // 当前节点指向下个节点
        _currentNode->Index = 0;                                        // 置数据长度为 0
        _index              = 0;
        return;
    }
    var tmp = _currentNode;
    _currentNode        = (BlockNode*)new size_t[ ((sizeof(BlockNode) + _nodeSize) + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1) ];
    _currentNode->Next  = Null;
    _buffer             = ((Byte*)_currentNode) + sizeof(BlockNode);
    tmp->Size           = _nodeSize;
    tmp->Index          = _index;
    tmp->Next           = _currentNode;
    _index              = 0;
    ++_nodeCount;
}

INLINE LinkedBlock::LinkedBlock() :
    _nodeSize(131072 - sizeof(BlockNode)) {
    _firstNode          = (BlockNode*)new size_t[ ((sizeof(BlockNode) + _nodeSize) + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1) ];
    _firstNode->Next    = Null;
    _buffer             = ((Byte*)_firstNode) + sizeof(BlockNode);
    _currentNode        = _firstNode;
    _index              = 0;
    _nodeCount          = 1;
}

INLINE LinkedBlock::LinkedBlock(UInt32 nodeSize) : 
    _nodeSize(nodeSize) {
    _firstNode          = (BlockNode*)new size_t[ ((sizeof(BlockNode) + nodeSize) + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1) ];
    _firstNode->Next    = Null;
    _buffer             = ((Byte*)_firstNode) + sizeof(BlockNode);
    _currentNode        = _firstNode;
    _index              = 0;
    _nodeCount          = 1;
}

// 析构 (将从 _firstNode 开始 delete [] , 直到 Next == Null)
INLINE LinkedBlock::~LinkedBlock() {
    BlockNode *node = _firstNode, *next;
    do {
        next = node->Next;
        delete [] node;
    }
    while ((node = next));
}

INLINE void LinkedBlock::Clear() {
    _currentNode   = _firstNode;
    _buffer        = ((Byte*)_firstNode) + sizeof(BlockNode);
    _index         = 0;
}




}}
#endif