#include "Lib/All.h"
#include "GLWindow.h"
#include "Video.h"

Video::Video( GLWindow* w )
    : _w( w )
{
    w->init( L"test", 1024, 768, 0, 0, false );
    w->setVsync( false );


    // 这段原用于 resize 时 call
    glViewport( 0, 0, _w->_w, _w->_h );          // 占据整个窗口
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, _w->_w, 0, _w->_h );


    //// todo: backup gl state

    ////// init gl
    ////glEnable( GL_LINE_SMOOTH );
    ////glEnable( GL_POINT_SMOOTH );

    ////glHint( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );
    ////glHint( GL_POINT_SMOOTH_HINT, GL_DONT_CARE );

    ////glEnable( GL_BLEND );
    ////glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );      // ( sC * 1 ) + ( dC * ( 1 - sA ) ) = rC

    ////glClear( GL_COLOR_BUFFER_BIT );
    ////glMatrixMode( GL_MODELVIEW );
    ////glLoadIdentity();
    ////glOrtho( 0, 1, 0, 1, -1, 1 );

    //glClearColor( 0, 0, 0, 0 );
    ////glClearDepth( 1 );
    ////glClear( GL_COLOR_BUFFER_BIT );// | GL_DEPTH_BUFFER_BIT );


    //// 将就在这里输出静态图了
    //glRects( 5, 5, _w->_w - 5, _w->_h - 5 );

    ////glColor3b( 255, 0, 0 );
    //glLineWidth( 3 );
    //glEnable( GL_LINE_STIPPLE );
    //glLineStipple( 2, 0x3f07 );
    //glBegin( GL_LINE_LOOP );
    //{
    //    glVertex2s( 191, 594 );
    //    glVertex2s( 390, 21 );
    //    glVertex2s( 594, 577 );
    //    glVertex2s( 12, 236 );
    //    glVertex2s( 779, 262 );
    //}
    //glEnd();

    //#define PI 3.1415926535898
    ////glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    ////glEnable( GL_CULL_FACE );
    ////glCullFace( GL_BACK );
    //GLint circle_points = 100;

    //glColor3f( 1, 0, 0 );
    //glBegin( GL_POLYGON );
    //for( int i = 0; i < circle_points; ++i )
    //{
    //    auto angle = 2 * PI * i / circle_points;
    //    glVertex2f( cos( angle ) * 100 + 200, sin( angle ) * 100 + 200 );
    //}
    //glEnd();

    //glColor3f( 0, 1, 0 );
    //glBegin( GL_POLYGON );
    //for( int i = circle_points - 1; i >= 0; --i )
    //{
    //    auto angle = 2 * PI * i / circle_points;
    //    glVertex2f( cos( angle ) * 100 + 250, sin( angle ) * 100 + 250 );
    //}
    //glEnd();


    //auto drawOneLinef = []( float x1, float y1, float x2, float y2 )
    //{
    //    glBegin( GL_LINES );
    //    glVertex2f( x1, y1 );
    //    glVertex2f( x2, y2 );
    //    glEnd();
    //};

    //int i;
    //glClear( GL_COLOR_BUFFER_BIT );
    //glColor3f( 1, 1, 1 );
    //glEnable( GL_LINE_STIPPLE );

    //glLineStipple( 1, 0x0101 );
    //drawOneLinef( 50, 125, 150, 125 );
    //glLineStipple( 1, 0x00FF );
    //drawOneLinef( 150, 125, 250, 125 );
    //glLineStipple( 1, 0x1C47 );
    //drawOneLinef( 250, 125, 350, 125 );





    //// 从下往上，从左往右，2进制从高往低位（默认），每4字节填充一排，即 32 像素
    //GLubyte mask[] = 
    //{
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    //};

    //glClearColor( 0, 0, 0, 0 );
    //glShadeModel( GL_FLAT );

    //glColor3f( 1, 1, 1 );
    //glRects( 25, 25, 125, 125 );
    //glEnable( GL_POLYGON_STIPPLE );
    //glPolygonStipple( mask );
    //glRects( 125, 25, 225, 125 );



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
