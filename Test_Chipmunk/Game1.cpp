#include "Precompile.h"

Game1::~Game1()
{
}
Game1::Game1()
{
    scene.designSize = { 768, 1024 };

    G::window->Init( L"test", (int)scene.designSize.width, (int)scene.designSize.height, 0, 0, true );
    G::window->resizeCallback = []
    {
        G::scene->size = { (float)G::window->width, (float)G::window->height };
        G::scene->dirty = true;
    };
    //G::window->setVsync( false );


    auto uibox = MarginBoxNode::Create();
    uibox->margin = { 5, 5, 5, 5 };
    scene.Add( uibox );

    auto title = BoxNode::Create();
    title->color = { 0, 255, 0, 0 };
    title->dock = { 0.5, 1 };
    title->anchor = { 0.5, 1 };
    title->size = { 300, 30 };
    title->offset = { 0, -1 };
    uibox->Add( title );

    auto close = BoxNode::Create();
    close->color = { 255, 0, 0, 0 };
    close->dock = { 1, 1 };
    close->anchor = { 1, 1 };
    close->size = { 30, 30 };
    close->offset = { -1, -1 };
    uibox->Add( close );
}

void Game1::Update()
{
    // todo: loop code here

}


/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )
*/
