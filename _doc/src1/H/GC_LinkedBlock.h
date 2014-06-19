#ifndef GC_LINKEDBLOCK_H
#define GC_LINKEDBLOCK_H
namespace Lib3 {
namespace GC {

// 内存块链：当内存不足时将以块操作来请求新的内存（一次请求，多次分配以节省分配时间）
// 只提供内存分配与对齐等功能，不提供 delete 功能 暂不允许大于此尺寸的内存请求行为
// 自动内存对齐原则：1, 2, 4, 8. 即：sizeof 小于这些尺寸的，按这些尺寸对齐; 大于 8 的，按 8 对齐
struct LinkedBlock : public Base {
    // 内存块节点，该节点内存以 new char[sizeof(BlockNode) + 尺寸] 方式来分配, delete [] BlockNode 指针　方式删
    struct BlockNode {
        UInt32          Index;                                          // 已分配索引（同时也代表了该块内存的已分配数据长）
        UInt32          Size;                                           // 尺寸
        BlockNode *     Next;                                           // 指向下一个节点
    };
    LinkedBlock();                                                      // 初始化块的最小尺寸。默认为 128k - 头
    explicit LinkedBlock(UInt32 nodeSize);                              // 初始化块的最小尺寸
    ~LinkedBlock();                                                     // 析构 (将从 _firstNode 开始 delete [] , 直到 Next == Null
    void                Clear();                                        // 令 _currentNode = _firstNode, _buffer, _index, _allocateSize 重置以便复用 (内存块数将保持)

#ifdef __DEBUG
    void *              Allocate(UInt32 const &len, SByte const * const &fn = Null, Int32 const &ln = 0);
    void                Free(void * const &p,       SByte const * const &fn = Null, Int32 const &ln = 0);
#else
    void *              Allocate(UInt32 const &len);
    void                Free(void * const &p);
#endif

private:
    BlockNode *         _firstNode;                                     // 指向起始节点
    BlockNode *         _currentNode;                                   // 指向起始节点
    Byte *              _buffer;                                        // 指向当前内存块的起始地址
    UInt32              _nodeCount;                                     // 节点总数（已分配的内存块数）
    UInt32              _index;                                         // 指向当前内存块的当前索引
    const UInt32        _nodeSize;                                      // 创建新节点时所采用的尺寸

    void                NewNode();                                      // 创建新的内存块节点
};




}}
#endif