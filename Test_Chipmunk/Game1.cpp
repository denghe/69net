#include "Precompile.h"

struct Scene1 : public Scene
{
    Scene1()
    {
        glClearColor( 0, 0, 0, 0 );
        //glShadeModel( GL_FLAT );
    }
    void Draw( int durationTicks ) override
    {
        glClear( GL_COLOR_BUFFER_BIT );
        NodeBase::Draw( durationTicks );
    }
};

Game1::~Game1()
{
    delete G::scene;
}
Game1::Game1()
{
    G::window->Init( L"test", 768, 1024, 0, 0, true );
    G::window->resizeCallback = []
    {
        glViewport( 0, 0, G::window->width, G::window->height );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D( 0, G::window->width, 0, G::window->height );
    };
    //G::window->setVsync( false );

    auto scene = new Scene1();   // init G::scene


    for( auto i = 0; i < 1024; i += 20 )
    {
        auto bn = BoxNode::Create();
        bn->position = { G::window->width / 2, G::window->height / 2 };
        bn->size = { i, i };
        scene->Add( bn );
    }

}

void Game1::Update()
{
    // todo: loop code here

#ifdef USE_STL
    for( auto node : G::scene->childs )
    {
#else
    for( auto i = 0; i < G::scene->childs.size(); ++i )
    {
        auto node = G::scene->childs[ i ];
#endif
        auto bn = (BoxNode*)node;
        if( bn->size.width <= 1024 )
        {
            bn->size.width += 1;
            bn->size.height += 1;
        }
        else
        {
            bn->size.width = 0;
            bn->size.height = 0;
        }
        bn->dirty = true;
    }
}


/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )
*/
