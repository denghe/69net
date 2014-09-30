#include "Precompile.h"

Game1::~Game1()
{
    delete G::node; G::node = nullptr;
    G::logic = nullptr;
}
Game1::Game1()
{
    G::logic = this;
    G::node = new Node();

    // code here
    /*
    设计尺寸：    768 * 1024
    格子划分：    12 * 16
    每格尺寸：    64 * 64
    细胞尺寸：    32 * 32 ( 即 1/4 格 )
    */


    G::window->Init( L"test", 768, 1024, 0, 0, true );
    //G::_glwindow->setVsync( false );

    G::window->resizeCallback = []
    {
        glViewport( 0, 0, G::window->width, G::window->height );          // 占据整个窗口
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D( 0, G::window->width, 0, G::window->height );
    };

    G::node->beforeDraw = []( int durationTicks )
    {
        glLineWidth( 3 );
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 2, 0x3f07 );
        glBegin( GL_LINE_LOOP );
        {
            glVertex2s( 191, 594 );
            glVertex2s( 390, 21 );
            glVertex2s( 594, 577 );
            glVertex2s( 12, 236 );
            glVertex2s( 767, 262 );
        }
        glEnd();
    };
}

void Game1::Update()
{
    // todo: code here
    printf( "." );
}
