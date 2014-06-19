#ifndef GC_BUDDY_H
#define GC_BUDDY_H
namespace Lib3 {
namespace GC {



// 兄弟算法内存管理器 适用于单线程内部开辟一块大内存来自主分配
// 算法原理简述：将大块内存多次２分，为每种粒度准备状态数组．
// 当请求特定尺寸内存时，到相应数组取元素．不足则找更大粒度２分索要
// 当归还内存时．查找当前粒度是否有２分的另一半．有则合并放入更大粒度数组（继续判断能否合并）
// 块尺寸为 8 的倍数, 块起始地址偏移量为 块尺寸 的倍数以方便定位相应容器
// 参数示例： 512, 65536, 8
// todo: 为调试设计的功能：每次分配／释放内存时，记录日志．也可考虑以 mfc 的 new 重载类似的形式来记录 new 语句所在行列号
// 当程序退出时理论上讲所有内存已归还释放．通过扫描标记区结合检查分配／释放日志配对特性，从而可以辅助查找内存泄漏点
struct Buddy : public Base {
    struct Node {
        UInt32              Index;
        UInt32              PrevNodeIndex;
        UInt32              NextNodeIndex;
        Byte                Level;                                      // 仅供 Nodes[0] 使用，用于记录内存块(节点)分配出去的时候所处的层次
        Boolean             Allocated;                                  // 用于每层分别记录内存块(节点)是否已分配出去, 便于回收时合并
    };

    explicit Buddy(UInt32 const &blockSize, UInt32 const &blockLength, UInt32 const &maxLevel, Byte * const &buf = Null);    // 块尺寸, 多少块, 多少层，Buffer 地址为０则直接 new
    virtual ~Buddy();
    void                    Dump();

#ifdef __DEBUG
    void *                  Allocate(UInt32 const &len, SByte const * const &fn = Null, Int32 const &ln = 0);
    void                    Free(void * const &p,       SByte const * const &fn = Null, Int32 const &ln = 0);
#else
    void *                  Allocate(UInt32 const &len);
    void                    Free(void * const &p);
#endif

private:
    Node **                 _nodes;                                      // 节点数组的数组  todo: 这个可以搞成１维数组　最大总数为最低层节点数 * 2
    UInt32 *                _headNodesIndex;                             // 每层 头节点索引 数组
    Byte *                  _buffer;                                     // 最大 4g
    UInt32 const            _bufferSize;                                 // 存 BlockSize * BlockLength
    UInt32 const            _maxLevel;                                   // 分层数
    UInt32 const            _blockSize;                                  // 512 ? 1024 ? ...
    UInt32 const            _blockLength;                                // 512 ? 1024 ? ...
    Boolean                 _isRef;                                      // 是否为传入的 Buffer (不需要回收)

    void *                  AllocateLevel(UInt32 const &level);         // 从指定层次分配一块内存
    UInt32                  AllocateLevelSplit(UInt32 const &level);    // 向上层请求拆分内存块并得到一个 Index
    void                    FreeLevel(UInt32 const &level, UInt32 const &idx);        // 释放指定层次&索引的内存块
    void                    FreeIndex(UInt32 const &idx);               // 释放指针转换为的索引处的内存块

};





}}
#endif
