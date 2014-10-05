#ifndef __CDGRID_H__
#define __CDGRID_H__

namespace xxx
{
    // CD = collision detection
    // 当前只支持矩形 AABB( 如果一个显示对象较为复杂，多注册几个Item即可 )

    struct CdPoint
    {
        int x, y;
    };
    struct CdSize
    {
        int w, h;
    };

    struct CdCell;
    struct CdItem
    {
        CdSize radius;                  // 宽高
        CdPoint pos;                    // 坐标（锚点位于左下角）
        int groupId;                    // 0 组号将和其他组号发生碰撞，相同组号不发生
        uint layerMask;                 // maskA & maskB 不为 0 的发生碰撞
        uint flag;                      // 自增流水号，用于去重
        void* userData;
        List<CdCell*> cells;            // 占据了哪些格子
    };

    struct CdCell
    {
        CdPoint index;                  // x: columnIndex, y: rowIndex
        Set<CdItem*> items;             // 格内有哪些 item
    };

    // 格子阵列( 分割显示区域，综控 )
    struct CdGrid
    {
        Set<CdItem*> items;
        List<CdItem*> freeItems;
        List<CdCell> cells;

        CdSize cellRadius;
        CdSize cellDiameter;
        int rowCount = 0;
        int columnCount = 0;
        CdSize gridDiameter;


        uint autoFlag = 0;  // 自增流水号，用于去重( 进某函数时 +1。当再次等于0时，所有 cell 和 item 的 flag 设成 0, 用 1 )

        ~CdGrid();

        void Init( CdSize _gridDiameter, int _rowCount, int _columnCount ); // 按一个总尺寸划分格子并 Clear
        void Reserve( int capacity );                                       // 于 freeItems 中预创建 capacity 个 items
        CdItem* CreateItem();                                               // 申请内存( 优先从 freeItems 拿 )，并返回
        void DestroyItem( CdItem* _item );                                  // 析构 item 并从 items 移动至 freeItems
        void Clear();                                                       // Destroy items, 清 cells 的 items
        void Reindex( CdItem* _item );                                      // 创建索引信息
        void Reindex( CdItem* _item, CdPoint const& _pos );                 // 更新位置及索引信息
        int GetNearItems( List<CdItem*> container, CdItem* _item );         // 填充 _item 所占格子笼罩的 items, 返回个数
        int GetCollisionItems( List<CdItem*> container, CdItem* _item );    // 填充与 _item 碰撞的 items, 返回个数
        int GetCellIndex( CdPoint const& _pos );                            // 判断一个坐标位于哪格( 超出返回 -1 )
        void ResetAllFlag();                                                // 清所有 items, freeItems 的 flag 为 0
    };
}

#endif
