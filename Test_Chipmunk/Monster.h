#ifndef __MONSTER_H__
#define __MONSTER_H__

struct Monster
{
    // logic
    Point pos;
    Point xyInc;
    Size size;

    // display
    Box node;

    // collision
    CdGrid* cditemContainer = nullptr;
    CdItem* cditem = nullptr;

    Monster();

    void Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    virtual bool Update();

    void Destroy();

    static Monster* Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    int idx = 0;
    static List<Monster*> objs;
    static List<Monster*> objPool;

    static void FreeObjs();
};


#endif
