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
    b2->offset = { 16 * 0, 16 * -0.5 };
    b3->size = { 16 * 4, 16 * 1 };
    b3->offset = { 16 * 0, 16 * -3.5 };

    b1->color = b2->color = b3->color = { 111, 111, 255, 0 };
    return b;
}

struct Bullet : public Box
{
    Point xyInc;
    bool Update()
    {
        if( offset.y >= dh ) return false;
        offset.x += xyInc.x;
        offset.y += xyInc.y;
        dirty = true;
        return true;
    }
};
static Hash<Bullet*> bullets;
Bullet* createBullet( Point const& pos )
{
    auto b = Create<Bullet>();
    b->size = { 16 * 1, 16 * 2 };
    b->offset = pos;
    b->color = { 111, 255, 111, 0 };
    return b;
}

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
    plane->offset = { scene.size.w / 2, scene.size.h / 2 };
    scene.Add( plane );
}

void Game3::Update()
{
    input.touchEvents.clear();
    if( input.touching )
    {
        // 跟鼠标移
        auto xya = getxya( plane->offset, input.touchPos );
        auto xi = xya->x / _xyam * 15;
        auto yi = xya->y / _xyam * 15;
        auto maxxi = input.touchPos.x - plane->offset.x;
        auto maxyi = input.touchPos.y - plane->offset.y;
        if( xi < 0 && xi < maxxi ) xi = maxxi;
        else if( xi > 0 && xi > maxxi ) xi = maxxi;
        if( yi < 0 && yi < maxyi ) yi = maxyi;
        else if( yi > 0 && yi > maxyi ) yi = maxyi;
        plane->offset.x += xi;
        plane->offset.y += yi;
        plane->dirty = true;
    }

    // 射
    auto bullet = createBullet( { plane->offset.x, plane->offset.y + 16 * 2 } );
    bullet->xyInc = { ( rand() % 100 ) / 10.0f - 5, 10 };
    scene.Add( bullet );
    bullets.insert( bullet );

    // 弹飞
    for( int i = bullets.size() - 1; i >= 0; --i )
    {
        auto o = bullets[ i ];
        auto bullet = o->key;
        if( !bullet->Update() )
        {
            bullet->RemoveFromParent();
            bullets.erase( o );
        }
    }

    // todo: 出怪？用 CD 检测
}

Game3::~Game3()
{
}
