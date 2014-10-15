#include "Precompile.h"

Game1::Game1()
{
    int dw = 768, dh = 1024;
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game1", dw, dh );
    //G::window->setVsync( false );
}

static Point angleXyIncs[ 360 ];           // 存放度数对应的 xy 增量

void Game1::Loaded()
{
    for( int i = 0; i < 360; ++i )  // 填充 360 度的 xy 增量表
    {
        auto a = M_PI * 2 / 360 * i;
        angleXyIncs[ i ] = { sin( a ), cos( a ) };
    }

    mb = Create<MarginBox>();       // 画个离 windows 边缘 5 的边框
    mb->margin = { 5, 5, 5, 5 };
    mb->size = { scene.size.w - 10, scene.size.h - 10 };
    scene.Add( mb );
}

void Game1::Update()
{
    input.touchEvents.Clear();      // 这里先无视鼠标事件，只关注状态
    if( input.touching )
    {
        createObj( input.touchPos.x - mb->pos.x, input.touchPos.y - mb->pos.y );
    }

    for( int i = objs.Size() - 1; i >= 0; --i )
    {
        auto n = objs[ i ];
        auto& o = n->key;
        if( !o->Update() )     // 更新所有 object, 死掉的加 kill list
        {
            o->RemoveFromParent();      // ref = 1
            o->Release();               // ref = 0 ( delete )
            objs.Erase( n );
        }
    }
}

Game1::~Game1()
{
    for( int i = 0; i < objs.Size(); ++i )
    {
        auto& o = objs[ i ]->key;       // ref = 2
        o->Release();                   // ref = 1 ( scene.childs 会杀 )
    }
}

void Game1::createObj( float x, float y )
{
    auto o = new Object();              // ref = 1
    objs.Insert( o );                   // ref = 1
    mb->Add( o );                       // ref = 2
    o->size = { 1, 1 };
    o->pos = { x, y, };
    o->xyInc = angleXyIncs[ rand() % 360 ];
}
