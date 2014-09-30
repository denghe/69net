#include "Precompile.h"

Video::Video( GLWindow* w )
    : _w( w )
{
    w->init( L"test", 768, 1024, 0, 0, true );
    //w->setVsync( false );

    // 这段原用于 resize 时 call
    glViewport( 0, 0, _w->_w, _w->_h );          // 占据整个窗口
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, _w->_w, 0, _w->_h );
}

Video::~Video()
{
}

void Video::update( int durationTicks )
{

}
