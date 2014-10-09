#include "Precompile.h"

/*
设计尺寸：    768 * 1024
格子划分：    24 * 32
每格尺寸：    32 * 32
细胞尺寸：    16 * 16

// 鼠标按住不放，投放怪进场。如果怪间重叠，会自行走开。超出屏的杀

*/


struct Monster4
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

    Monster4();

    void Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    bool Update();

    void Destroy();

    static Monster4* Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos );

    Hash<Monster4*>::Node* idx = nullptr;
    static Hash<Monster4*> objs;
    static List<Monster4*> objPool;

    static void FreeObjs();
};

Monster4::Monster4()
{
    size = { 16 * 2, 16 * 1 };
    xyInc = { 0, 0 };
}

void Monster4::Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
{
    assert( !node );

    pos = _pos;

    nodeContainer = _nodeContainer;
    node = ::Create<Box>();
    node->size = size;
    node->pos = _pos;
    node->color = { 255, 0, 0, 0 };
    nodeContainer->Add( node );

    cditemContainer = _cditemContainer;
    cditem = _cditemContainer->CreateItem();
    cditem->Init( { size.w / 2, size.h / 2 }, { G::scene->size.w, G::scene->size.h }, 0, 0xFFFFFFFFu, this );
    cditem->Index();
}

bool Monster4::Update()
{
    if( pos.x < -size.w || pos.x > G::scene->size.w + size.w
        || pos.y < -size.h || pos.y > G::scene->size.h + size.h ) return false;

    // xyInc =  todo: 找附近的怪，远离它( 甚至可以降低找的次数 )
    if( auto o = cditem->GetCollisionItem() )
    {
        auto other = (Monster4*)o->userData;
        auto xya = getxya( { pos.x, pos.y }, { other->pos.x, other->pos.y } );
        xyInc.x = -xya->x / _xyam;
        xyInc.y = -xya->y / _xyam;
    }
    else
    {
        xyInc = { 0, 0 };
    }

    if( xyInc.x == 0 && xyInc.y == 0 ) return true;

    pos.x += xyInc.x;
    pos.y += xyInc.y;

    if( node )
    {
        node->pos = pos;
        node->dirty = true;
    }
    if( cditem )
    {
        cditem->Update( { G::scene->size.w + pos.x, G::scene->size.h + pos.y } );
    }

    return true;
}

void Monster4::Destroy()
{
    node->RemoveFromParent();
    node = nullptr;
    cditem->Destroy();
    cditem = nullptr;

    objs.erase( idx );
    objPool.push( this );
}

Monster4* Monster4::Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
{
    Monster4* rtv;
    if( objPool.size() )
    {
        rtv = objPool.top_pop();
    }
    else rtv = new Monster4();
    rtv->Init( _nodeContainer, _cditemContainer, _pos );
    rtv->idx = objs.insert( rtv ).first;
    return rtv;
}

void Monster4::FreeObjs()
{
    for( int i = objs.size() - 1; i >= 0; --i )
    {
        delete objs[ i ]->key;
    }
    for( int i = objPool.size() - 1; i >= 0; --i )
    {
        delete objPool[ i ];
    }
}

Hash<Monster4*> Monster4::objs;
List<Monster4*> Monster4::objPool;



static const int dw = 768, dh = 1024, rowCount = 32, columnCount = 24;

Game4::Game4()
{
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game4", dw, dh );
    //G::window->setVsync( false );

    cdgrid.Init( { dw * 3, dh * 3 }, rowCount * 3, columnCount * 3 );  // 3*3 区块，正中间为屏幕映射
}

static Node* createPlane()
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


void Game4::Loaded()
{
    fillxya();
}

void Game4::Update()
{
    input.touchEvents.clear();
    if( input.touching )
    {
        // 产生怪
        for( int i = 0; i < 1; ++i )
        {
            auto monster = Monster4::Create( &scene, &cdgrid, input.touchPos );
        }
    }

    // 怪移动
    for( int i = Monster4::objs.size() - 1; i >= 0; --i )
    {
        auto o = Monster4::objs[ i ]->key;
        if( !o->Update() )
        {
            o->Destroy();
        }
    }

    // 每几秒显示一次总的怪数
    static int counter = 0;
    if( ++counter >= 60 )
    {
        counter = 0;
        Cout( "total monsters:", Monster4::objs.size() );
    }
}

Game4::~Game4()
{
    Monster4::FreeObjs();
}
