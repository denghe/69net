#ifndef __BULLET_H__
#define __BULLET_H__

struct Bullet
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

    Bullet();

    void Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    bool Update();

    void Destroy();

    static Bullet* Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    int idx = 0;
    static List<Bullet*> objs;
    static List<Bullet*> objPool;

    static void FreeObjs();
};



#endif
