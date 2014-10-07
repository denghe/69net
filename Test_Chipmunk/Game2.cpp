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
    G::window->Init( L"Game2", dw, dh );
    //G::window->setVsync( false );
}

void Game2::Loaded()
{
}

void Game2::Update()
{
}

Game2::~Game2()
{
}

void Game2::createObj( float x, float y )
{
    //auto o = new Object();          // ref = 1
    //objs.insert( o );               // ref = 1
    //mb->Add( o );                   // ref = 2
    //o->size = { 1, 1 };
    //o->offset = { x, y, };
    //o->xyInc = angleXyIncs[ rand() % 360 ];
}
