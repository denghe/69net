#include "Lib/All.h"
#include "GLWindow.h"
#include "Video.h"

Video::Video( GLWindow* w )
    : _w( w )
{
    w->init( L"test", 800, 600, 0, 0, false );
    w->setVsync( false );


    // todo: backup gl state

    //// init gl
    //glEnable( GL_LINE_SMOOTH );
    //glEnable( GL_POINT_SMOOTH );

    //glHint( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );
    //glHint( GL_POINT_SMOOTH_HINT, GL_DONT_CARE );

    //glEnable( GL_BLEND );
    //glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );      // ( sC * 1 ) + ( dC * ( 1 - sA ) ) = rC

    //glClear( GL_COLOR_BUFFER_BIT );
    //glMatrixMode( GL_MODELVIEW );
    //glLoadIdentity();
    //glOrtho( 0, 1, 0, 1, -1, 1 );


    glClearColor( 0, 0, 0, 0 );         // 占据整个窗口
    //glClearDepth( 1 );
    glClear( GL_COLOR_BUFFER_BIT );// | GL_DEPTH_BUFFER_BIT );

    // 这段原用于 resize 时 call
    glViewport( 0, 0, _w->_w, _w->_h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, _w->_w, 0, _w->_h );

    // 将就在这里输出静态图了
    glRecti( 5, 5, _w->_w - 5, _w->_h - 5 );

    // 输出
    glFlush();
}

Video::~Video()
{
    // todo: restore gl state
}

void Video::update( int durationTicks )
{

}
