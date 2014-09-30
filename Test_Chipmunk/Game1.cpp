#include "Precompile.h"

Game1::~Game1()
{
    delete G::node; G::node = nullptr;
    delete G::audio; G::audio = nullptr;
    delete G::input; G::input = nullptr;
    G::logic = nullptr;
}
Game1::Game1()
{
    G::logic = this;
    G::input = new Input();
    G::audio = new Audio();
    G::node = new Node();

    /*
    设计尺寸：    768 * 1024
    格子划分：    12 * 16
    每格尺寸：    64 * 64
    细胞尺寸：    32 * 32 ( 即 1/4 格 )
    */

    G::glwindow->Init( L"test", 768, 1024, 0, 0, true );
    //G::_glwindow->setVsync( false );

    // 这段原用于 resize 时 call
    // todo: 当 window resize 时会启用 timer, 理论上讲可以再传一个 timer 阶段的回调，即下面这段
    // 同时，进入正常循环之前，也先执行一把 timer 阶段的回调
    glViewport( 0, 0, G::glwindow->width, G::glwindow->height );          // 占据整个窗口
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, G::glwindow->width, 0, G::glwindow->height );

    // todo: code here
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
