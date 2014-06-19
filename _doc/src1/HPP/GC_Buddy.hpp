#ifndef GC_BUDDY_HPP
#define GC_BUDDY_HPP
namespace Lib3 {
namespace GC {


INLINE Buddy::~Buddy() {
    if (_isRef == false)
        delete [] _buffer;
    else
        delete [] _nodes;
}



#ifdef __DEBUG
INLINE void * Buddy::Allocate(UInt32 const &len, SByte const * const &fn, Int32 const &ln) {
    // todo: fn, ln
    for (UInt32 j = 0; j < _maxLevel; ++j)
        if (len <= (_blockSize << j))
            return AllocateLevel(j);
    return Null;
}

INLINE void Buddy::Free(void * const &p, SByte const * const &fn, Int32 const &ln) {
    // todo: fn, ln
    var idx = (UInt32)(((Byte*)p - _buffer) / _blockSize);
    FreeIndex(idx);
}
#else
INLINE void * Buddy::Allocate(UInt32 const &len) {
    for (UInt32 j = 0; j < _maxLevel; ++j)
        if (len <= (_blockSize << j))
            return AllocateLevel(j);
    return Null;
}

INLINE void Buddy::Free(void * const &p) {
    var idx = (UInt32)(((Byte*)p - _buffer) / _blockSize);
    FreeIndex(idx);
}
#endif







    

INLINE Buddy::Buddy(UInt32 const &blockSize, UInt32 const &blockLength, UInt32 const &maxLevel, Byte * const &buf) :
    _blockSize(blockSize), 
    _blockLength(blockLength),
    _bufferSize(blockSize * blockLength),
    _maxLevel(maxLevel) {
    Node *nodes;
    if (buf) {
        _buffer          = buf;
        _isRef           = true;
        var buf          = new Byte[sizeof(Node*) * maxLevel + sizeof(UInt32) * maxLevel + sizeof(Node) * blockLength * 2];  // todo: new size_t
        _nodes           = new (buf) Node*[maxLevel];
        _headNodesIndex  = new (buf + sizeof(Node*) * maxLevel) UInt32[maxLevel];
        nodes            = new (buf + sizeof(Node*) * maxLevel + sizeof(UInt32) * maxLevel) Node[blockLength * 2];
    }
    else {
        _buffer          = new Byte[_bufferSize + sizeof(Node*) * maxLevel + sizeof(UInt32) * maxLevel + sizeof(Node) * blockLength * 2];// todo: new size_t
        _isRef           = false;
        _nodes           = new (_buffer + _bufferSize) Node*[maxLevel];
        _headNodesIndex  = new (_buffer + _bufferSize + sizeof(Node*) * maxLevel) UInt32[maxLevel];
        nodes            = new (_buffer + _bufferSize + sizeof(Node*) * maxLevel + sizeof(UInt32) * maxLevel) Node[blockLength * 2];
    }
    // 暂时只令最高层有数据可分配。其他层只初始化索引但不链接。
    // 这意味着刚开始请求小尺寸时将拆分数次（也就是为分配大尺寸而优化）
    var idx = 0;
    for (UInt32 j = 0; j < maxLevel; ++j) {
        var len = blockLength >> j;
        _nodes[j] = nodes + idx;
        idx += len;
        for (UInt32 i = 0; i < (len); ++i) {
            var &node = _nodes[j][i];
            node.Index = i;
            node.Allocated = true;
        }
        _headNodesIndex[j] = (UInt32)-1;
    }
    {
        UInt32 j = maxLevel - 1;
        var len = blockLength >> j;
        var nodes = _nodes[j];
        for (UInt32 i = 0; i < len; ++i) {
            var &node = nodes[i];
            node.PrevNodeIndex = i - 1;
            node.NextNodeIndex = i + 1;
            node.Allocated = false;
        }
        nodes[0].PrevNodeIndex = (UInt32)-1;
        nodes[len - 1].NextNodeIndex = (UInt32)-1;
        _headNodesIndex[j] = 0;
    }
}


INLINE void * Buddy::AllocateLevel(UInt32 const &level) {
    assert ( level < _maxLevel );
    if (_headNodesIndex[level] == (UInt32)-1) {
        var idx = AllocateLevelSplit(level + 1);
        if (idx == (UInt32)-1)
            return Null;
        var &node = _nodes[level][idx];
        node.NextNodeIndex = (UInt32)-1;                            // 保存一半
        node.Allocated = false;
        _headNodesIndex[level] = idx;
        ++idx;                                                      // 返回一半
        var &node2 = _nodes[level][idx];
        node2.Allocated = true;
        _nodes[0][idx << level].Level = level;
        return _buffer + idx * (_blockSize << level);
    }
    var idx = _headNodesIndex[level];
    var &n = _nodes[level][idx];
    _headNodesIndex[level] = n.NextNodeIndex;
    n.Allocated = true;
    _nodes[0][idx << level].Level = level;
    return _buffer + idx * (_blockSize << level);
}

INLINE UInt32 Buddy::AllocateLevelSplit(UInt32 const &level) {
    assert ( level < _maxLevel );
    if (level >= _maxLevel)                                          // 打断递归
        return (UInt32)-1;
    if (_headNodesIndex[level] == (UInt32)-1) {
        var idx = AllocateLevelSplit(level + 1);
        if (idx == (UInt32)-1)
            return (UInt32)-1;
        var &node = _nodes[level][idx];
        node.NextNodeIndex = (UInt32)-1;                            // 将其中一半加入本层分配链
        node.Allocated = false;
        _headNodesIndex[level] = idx;
        ++idx;                                                      // 设另一半已被使用并返回
        _nodes[level][idx].Allocated = true;
        return idx << 1;
    }
    var idx = _headNodesIndex[level];
    var &n = _nodes[level][idx];
    n.Allocated = true;
    _headNodesIndex[level] = n.NextNodeIndex;
    return idx << 1;
}


INLINE void Buddy::FreeIndex(UInt32 const &idx) {
    var idx_ = idx;
    var level = (UInt32)_nodes[0][idx_].Level;
    assert ( level < _maxLevel );
    idx_ >>= level;
    var nodes = _nodes[level];
    var &node = nodes[idx_];
    assert ( node.Allocated == true );                              // 检测是否重复 free
    if (level < _maxLevel - 1) {
        if (idx_ & 1) {
            var &node2 = nodes[idx_ - 1];
            if (node2.Allocated == false) {
                node2.Allocated = true;
                //node2.Level = level + 1;
                _nodes[0][(idx_ - 1) << level].Level = level + 1;
                FreeIndex((idx_ - 1) << level);
                if (_headNodesIndex[level] == idx_ - 1)
                    _headNodesIndex[level] = node2.NextNodeIndex;
                else
                    nodes[node2.PrevNodeIndex].NextNodeIndex = node2.NextNodeIndex;
                return;
            }
        } else {
            var &node2 = nodes[idx_ + 1];
            if (node2.Allocated == false) {
                node2.Allocated = true;
                _nodes[0][idx_ << level].Level = level + 1;
                FreeIndex(idx_ << level);
                if (_headNodesIndex[level] == idx_ + 1)
                    _headNodesIndex[level] = node2.NextNodeIndex;
                else
                    nodes[node2.PrevNodeIndex].NextNodeIndex = node2.NextNodeIndex;
                return;
            }
        }
    }
    node.Allocated = false;
    node.NextNodeIndex = _headNodesIndex[level];
    if (_headNodesIndex[level] != (UInt32)-1)
        _nodes[level][_headNodesIndex[level]].PrevNodeIndex = idx_;
    _headNodesIndex[level] = idx_;
}





INLINE void Buddy::Dump() {
    Cout << "\nBlockSize    = " << _blockSize
            << "\tBlockLength  = " << _blockLength
            << "\tMaxLevel     = " << _maxLevel
            << "\nBufferSize   = " << _bufferSize;
    //UInt32 numfree = 0;
    //BitArray flag;
    //for (UInt32 j = 0; j < MaxLevel; ++j)
    //    for (UInt32 i = 0; i < (BlockLength >> j); ++i)
    //        if (!Nodes[j][i].Allocated)
    //            for (UInt32 k = 0; k < (1u << j); ++k)
    //                flag.SetTrue((i << j) + k);
    //for (UInt32 i = 0; i < BlockLength; ++i)
    //    if (flag.Get(i))
    //        ++numfree;
    //Cout << "\tFreeBlocks   = " << numfree
    //        << "\tAllocBlocks  = " << (BlockLength - numfree) << "\n";


    //for (UInt32 j = 0; j < MaxLevel; ++j) {
    //    for (UInt32 i = 0; i < (BlockLength >> j); ++i)
    //        Cout << (Nodes[j][i].Allocated ? 'A' : '_');
    //    Cout << '\n';
    //}
}






}}
#endif