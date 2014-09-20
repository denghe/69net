#include "Lib/All.h"
#include "Platform.h"
#include "Render.h"

Render::Render()
{
    // todo: backup gl state

    // init gl
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POINT_SMOOTH );

    glHint( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );
    glHint( GL_POINT_SMOOTH_HINT, GL_DONT_CARE );

    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );      // ( sC * 1 ) + ( dC * ( 1 - sA ) ) = rC

    glClear( GL_COLOR_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glOrtho( 0, 1, 0, 1, -1, 1 );
}

Render::~Render()
{
    // todo: restore gl state
}

void Render::update( int durationTicks )
{

}
