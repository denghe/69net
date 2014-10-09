#include "Precompile.h"

/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )

// 实现一个跟随鼠标匀速移动的机体

*/

static const int dw = 768, dh = 1024, rowCount = 16, columnCount = 12;

Game3::Game3()
{
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game3", dw, dh );
    //G::window->setVsync( false );

    cdgrid = new CdGrid();
    cdgrid->Init( { dw * 3, dh * 3 }, rowCount * 3, columnCount * 3 );  // 3*3 区块，正中间为屏幕映射
}

Game3::~Game3()
{
    delete cdgrid;
}

Node* createPlane()
{
    auto b = Create<Node>();
    auto b1 = Create<Box>();    // -
    auto b2 = Create<Box>();    // |
    auto b3 = Create<Box>();    // -
    b->Add( b1 );
    b->Add( b2 );
    b->Add( b3 );

    b1->size = { 16 * 8, 16 * 2 };
    b2->size = { 16 * 2, 16 * 5 };
    b2->pos = { 16 * 0, 16 * -0.5 };
    b3->size = { 16 * 4, 16 * 1 };
    b3->pos = { 16 * 0, 16 * -3.5 };

    b1->color = b2->color = b3->color = { 111, 111, 255, 0 };
    return b;
}

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

    Monster()
    {
        size = { 16 * 2, 16 * 1 };
    }

    void Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
    {
        assert( !node );

        pos = _pos;
        xyInc = { ( rand() % 100 ) / 50.0f - 1, -1 };

        nodeContainer = _nodeContainer;
        node = ::Create<Box>();
        node->size = size;
        node->pos = _pos;
        node->color = { 255, 0, 0, 0 };
        nodeContainer->Add( node );

        cditemContainer = _cditemContainer;
        cditem = _cditemContainer->CreateItem();
        cditem->Init( { size.w, size.h }, { dw, dh }, 1, 0xFFFFFFFFu, this );
        cditem->Index();
    }

    bool Update()
    {
        if( pos.x < -size.w || pos.x > dw + size.w
            || pos.y < -size.h || pos.y > dh + size.h ) return false;

        pos.x += xyInc.x;
        pos.y += xyInc.y;

        if( node )
        {
            node->pos = pos;
            node->dirty = true;
        }
        if( cditem )
        {
            cditem->Update( { dw + pos.x, dh + pos.y } );
        }

        // todo: more cd check ai code here

        return true;
    }

    void Destroy()
    {
        node->RemoveFromParent();
        node = nullptr;
        cditem->Destroy();
        cditem = nullptr;

        objs.erase( idx );
        freeObjs.push( this );
    }

    static Monster* Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
    {
        Monster* rtv;
        if( freeObjs.size() )
        {
            rtv = freeObjs.top_pop();
        }
        else rtv = new Monster();
        rtv->Init( _nodeContainer, _cditemContainer, _pos );
        rtv->idx = objs.insert( rtv ).first;
        return rtv;
    }

    Hash<Monster*>::Node* idx = nullptr;
    static Hash<Monster*> objs;
    static List<Monster*> freeObjs;
};
Hash<Monster*> Monster::objs;
List<Monster*> Monster::freeObjs;


struct Bullet
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

    Bullet()
    {
        size = { 16 * 1, 16 * 2 };
    }

    void Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
    {
        assert( !node );

        pos = _pos;
        xyInc = { ( rand() % 100 ) / 10.0f - 5, 10 };

        nodeContainer = _nodeContainer;
        node = ::Create<Box>();
        node->size = size;
        node->pos = _pos;
        node->color = { 111, 255, 111, 0 };
        nodeContainer->Add( node );

        cditemContainer = _cditemContainer;
        cditem = _cditemContainer->CreateItem();
        cditem->Init( { size.w, size.h }, { dw, dh }, 2, 0xFFFFFFFFu, this );
        cditem->Index();
    }

    bool Update()
    {
        if( pos.x < -size.w || pos.x > dw + size.w
            || pos.y < -size.h || pos.y > dh + size.h ) return false;

        pos.x += xyInc.x;
        pos.y += xyInc.y;

        if( node )
        {
            node->pos = pos;
            node->dirty = true;
        }
        if( cditem )
        {
            cditem->Update( { dw + pos.x, dh + pos.y } );
        }

        // 如果有碰到任意怪
        if( auto tar = cditem->GetCollisionItem() )
        {
            auto m = (Monster*)tar->userData;
            m->Destroy();
            return false;
        }

        return true;
    }

    void Destroy()
    {
        node->RemoveFromParent();
        node = nullptr;
        cditem->Destroy();
        cditem = nullptr;

        objs.erase( idx );
        freeObjs.push( this );
    }

    static Bullet* Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
    {
        Bullet* rtv;
        if( freeObjs.size() )
        {
            rtv = freeObjs.top_pop();
        }
        else rtv = new Bullet();
        rtv->Init( _nodeContainer, _cditemContainer, _pos );
        rtv->idx = objs.insert( rtv ).first;
        return rtv;
    }

    Hash<Bullet*>::Node* idx = nullptr;
    static Hash<Bullet*> objs;
    static List<Bullet*> freeObjs;
};
Hash<Bullet*> Bullet::objs;
List<Bullet*> Bullet::freeObjs;



// 填充查表用结构
struct Xya
{
    signed char x;
    signed char y;
    signed char a;
};
static Xya _xyas[ dw * 2 ][ dh * 2 ];
static float _xyam = 127;
void fillxya()
{
    float PI = 3.14159265359;
    float PI_2 = PI / 2;
    float PI2 = PI * 2;
    for( int x = 0; x < _countof( _xyas ); ++x )
    {
        for( int y = 0; y < _countof( _xyas[ x ] ); ++y )
        {
            auto& xya = _xyas[ x ][ y ];

            auto X = x - dw;
            auto Y = y - dh;

            if( X == 0 && Y == 0 )
            {
                xya.x = xya.y = 1;
                xya.a = 0;
            }
            else
            {
                auto D = sqrt( X*X + Y*Y );
                xya.x = X / D * _xyam;
                xya.y = Y / D * _xyam;
                xya.a = -atan2( Y, X ) / PI2 * _xyam;
            }
        }
    }
}
Xya* getxya( Point const& a, Point const& b )
{
    return &_xyas[ (int)( b.x - a.x + dw ) ][ (int)( b.y - a.y + dh ) ];
    // xya.x / _xyam * speed, xya.y / _xyam * speed
    // xya.a / _xyam * 360 + 90
}

void Game3::Loaded()
{
    fillxya();

    plane = createPlane();
    plane->pos = { scene.size.w / 2, scene.size.h / 2 };
    scene.Add( plane );
}

void Game3::Update()
{
    input.touchEvents.clear();
    if( input.touching )
    {
        // 跟鼠标移
        auto xya = getxya( plane->pos, input.touchPos );
        auto xi = xya->x / _xyam * 15;
        auto yi = xya->y / _xyam * 15;
        auto maxxi = input.touchPos.x - plane->pos.x;
        auto maxyi = input.touchPos.y - plane->pos.y;
        if( xi < 0 && xi < maxxi ) xi = maxxi;
        else if( xi > 0 && xi > maxxi ) xi = maxxi;
        if( yi < 0 && yi < maxyi ) yi = maxyi;
        else if( yi > 0 && yi > maxyi ) yi = maxyi;
        plane->pos.x += xi;
        plane->pos.y += yi;
        plane->dirty = true;
    }

    // 发射子弹
    for( int i = 0; i < 5; ++i )
    {
        auto bullet = Bullet::Create( &scene, cdgrid, { plane->pos.x, plane->pos.y + 16 * 2 } );
    }

    // 产生怪
    for( int i = 0; i < 10; ++i )
    {
        auto monster = Monster::Create( &scene, cdgrid, { rand() % dw, dh } );
    }

    // 怪前进
    for( int i = Monster::objs.size() - 1; i >= 0; --i )
    {
        auto o = Monster::objs[ i ]->key;
        if( !o->Update() )
        {
            o->Destroy();
        }
    }

    // 子弹前进
    for( int i = Bullet::objs.size() - 1; i >= 0; --i )
    {
        auto o = Bullet::objs[ i ]->key;
        if( !o->Update() )
        {
            o->Destroy();
        }
    }

    // 每几秒显示一次总的怪和弹数
    static int counter = 0;
    if( ++counter >= 60 )
    {
        counter = 0;
        Cout( "total bullets:", Bullet::objs.size(), "\ntotal bullets:", Monster::objs.size() );
    }
}





//struct Action : Ref { };
//struct Sequence : public Action { List<Action*> childs; };
//struct Repeater : public Action { Action* action; int count; };
//struct MoveTo : public Action { float x, y; };
//struct CallFunc : public Action { std::function<void()> func; };
//struct Delay : public Action { float sec; };
