#include "Precompile.h"

/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )

先实现随机位置 ( 16 ~ 767 - 16, 16 ~ 1023 - 16 ) 显示 N 个方块，鼠标点击消除
*/

//static int dw = 768, dh = 1024, rowCount = 16, columnCount = 12;

static int dw = 1024, dh = 768, rowCount = 12, columnCount = 16;

Game2::Game2()
{
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game2", dw, dh );
    //G::window->setVsync( false );
}

void Game2::createBox( int _x, int _y )
{
    auto box = Create<Box>();
    box->pos = { _x, _y };
    box->size = { 32, 32 };
    scene.Add( box );

    auto item = cdgrid.CreateItem();
    //box->userData = item;
    item->userData = box;
    item->pos = { _x, _y };
    item->radius = { 16, 16 };
    item->Index();
}

static CdItem* mouseItem = nullptr;
void Game2::Loaded()
{
    cdgrid.Init( { dw, dh }, rowCount, columnCount );
    for( int i = 0; i < 1000; ++i )
    {
        int x = 16 + rand() % ( dw - 1 - 16 - 16 );
        int y = 16 + rand() % ( dh - 1 - 16 - 16 );
        createBox( x, y );
    }

    mouseItem = cdgrid.CreateItem();
    mouseItem->pos = { 16, 16 };
    mouseItem->radius = { 16, 16 };
    mouseItem->Index();
}

void Game2::Update()
{
    while( input.touchEvents.Size() )
    {
        input.touchEvents.Clear();
        if( input.touching )
        {
            auto x = input.touchPos.x;
            auto y = input.touchPos.y;

            if( x < 16 ) x = 16;
            if( x > dw - 1 - 16 ) x = dw - 1 - 16;

            if( y < 16 ) y = 16;
            if( y > dh - 1 - 16 ) y = dh - 1 - 16;

            mouseItem->Update( { x, y } );
            mouseItem->GetCollisionItems( touchedItems );
            for( int i = 0; i < touchedItems.Size(); ++i )
            {
                auto& item = touchedItems[ i ];
                scene.Remove( (Node*)item->userData );
                item->Destroy();
            }
        }
    }
}

Game2::~Game2()
{
}

//auto& e = input.touchEvents.top();
////if( e.type == TouchEventType::Down )
////{
////}
//input.touchEvents.pop();
