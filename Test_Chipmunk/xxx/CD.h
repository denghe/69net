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
        CdSize radius;                  // 半径
        CdPoint pos;                    // 中心点坐标
        int groupId;                    // 0 组号将和其他组号发生碰撞，相同组号不发生
        uint layerMask;                 // maskA & maskB 不为 0 的发生碰撞
        uint flag;                      // 自增流水号，用于去重
        void* userData;
        List<CdCell*> cells;            // 占据了哪些格子
    };

    struct CdCell
    {
        CdPoint index;                  // x: columnIndex, y: rowIndex
        Hash<CdItem*> items;            // 格内有哪些 item
    };

    // 格子阵列( 分割显示区域，综控 )
    struct CdGrid
    {
        Hash<CdItem*> items;
        List<CdItem*> freeItems;
        List<CdCell> cells;

        CdSize gridDiameter;
        CdSize cellRadius;
        CdSize cellDiameter;
        int rowCount;
        int columnCount;

        uint autoFlag;  // 自增流水号，用于去重( 进某函数时 +1。当再次等于0时，所有 cell 和 item 的 flag 设成 0, 用 1 )
        void IncreaseFlag();                                                // ++autoFlag; 循环后重置所有并跳过 0

        CdGrid();
        ~CdGrid();

        void Init( CdSize _gridDiameter, int _rowCount, int _columnCount ); // 按一个总尺寸划分格子并 Clear
        void Reserve( int _capacity );                                      // 于 freeItems 中预创建 capacity 个 items
        CdItem* CreateItem();                                               // 申请内存( 优先从 freeItems 拿 )，并返回
        void DestroyItem( CdItem* _item );                                  // 析构 item 并从 items 移动至 freeItems
        void Clear();                                                       // Destroy items, 清 cells 的 items
        void Index( CdItem* _item );                                          // 创建索引信息
        void Update( CdItem* _item, CdPoint const& _pos );                  // 更新位置及索引信息
        int GetNearItems( List<CdItem*>& _container, CdItem* _item );       // 填充 _item 所占格子笼罩的 items, 返回个数
        int GetCollisionItems( List<CdItem*>& _container, CdItem* _item );  // 填充与 _item 碰撞的 items, 返回个数
        CdItem* GetCollisionItem( CdItem* _item );                          // 返回首个检测到的 与 _item 碰撞的 item
        int GetItems( List<CdItem*>& _container, CdPoint const& _pos );     // 填充覆盖着某坐标的 items，返回个数
        CdItem* GetItem( CdPoint const& _pos );                             // 返回覆盖着某坐标的，第1个扫到的 item
        static bool CheckCollision( CdItem* _a, CdItem* _b );               // 检查两个 item 是否相交
        static bool CheckCollision( CdItem* _a, CdPoint _pos );             // 检查 pos 是否位于 item 内
        // todo: 找离目标区域 最近的 item ? 得到范围内由近到远排列的 items list?
    };
}

#endif
