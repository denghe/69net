#ifndef __MONSTER_H__
#define __MONSTER_H__

struct Monster
{
    // logic
    Point pos;
    Point xyInc;
    Size size;

    // display
    Node* nodeContainer = nullptr;
    Box* node = nullptr;

    // collision
    CdGrid* cditemContainer = nullptr;
    CdItem* cditem = nullptr;

    Monster();

    void Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    bool Update();

    void Destroy();

    static Monster* Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    Hash<Monster*>::Node* idx = nullptr;
    static Hash<Monster*> objs;
    static List<Monster*> objPool;

    static void FreeObjs();
};


#endif
