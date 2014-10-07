#include "Precompile.h"

/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )

先实现随机位置 ( 16 ~ 767 - 16, 16 ~ 1023 - 16 ) 显示 N 个方块，鼠标点击消除
*/

static int dw = 768, dh = 1024, rowCount = 16, columnCount = 12;

Game3::Game3()
{
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game3", dw, dh );
    //G::window->setVsync( false );
}

void Game3::Loaded()
{
}

void Game3::Update()
{
}

Game3::~Game3()
{
}
