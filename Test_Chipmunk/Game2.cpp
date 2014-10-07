#include "Precompile.h"

/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )
*/

Game2::Game2()
{
    int dw = 768, dh = 1024;
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"test", dw, dh );
    //G::window->setVsync( false );
}


static Point angleXyIncs[ 360 ];           // 存放度数对应的 xy 增量

void Game2::Loaded()
{
    for( int i = 0; i < 360; ++i )  // 填充 360 度的 xy 增量表
    {
        auto a = M_PI * 2 / 360 * i;
        angleXyIncs[ i ] = { sin( a ), cos( a ) };
    }

    mb = new MarginBox();
    mb->AutoRelease();
    mb->margin = { 5, 5, 5, 5 };
    mb->size = { scene.size.w - 10, scene.size.h - 10 };
    scene.Add( mb );
}

void Game2::Update()
{
    input.touchEvents.clear();      // 这里先无视鼠标事件，只关注状态
    if( input.touching )
    {
        createObj( input.touchPos.x - mb->offset.x, input.touchPos.y - mb->offset.y );
    }

    List<Hash<Object*>::Node*> dead;
    for( int i = 0; i < objs.size(); ++i )
    {
        auto o = objs[ i ];
        if( !o->key->Update() )
        {
            dead.push( o );
        }
    }
    for( int i = 0; i < dead.size(); ++i )
    {
        auto& o = dead[ i ]->key;   // ref = 2
        o->RemoveFromParent();      // ref = 1
        objs.erase( o );            // ref = 1
        o->Release();               // ref = 0 ( delete )
    }
}

Game2::~Game2()
{
    for( int i = 0; i < objs.size(); ++i )
    {
        auto& o = objs[ i ]->key;   // ref = 2
        o->Release();               // ref = 1 ( 留待 scene.Childs 杀 )
    }
}

void Game2::createObj( float x, float y )
{
    auto o = new Object();          // ref = 1
    objs.insert( o );               // ref = 1
    mb->Add( o );                   // ref = 2
    o->size = { 1, 1 };
    o->offset = { x, y, };
    o->xyInc = angleXyIncs[ rand() % 360 ];
}
