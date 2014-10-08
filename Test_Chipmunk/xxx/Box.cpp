#include "Precompile.h"

namespace xxx
{
    void Box::Drawing( int _durationTicks )
    {
        glColor3ubv( (GLubyte*)&color );
        glBegin( GL_LINE_LOOP );
        glVertex2f( pos.x, pos.y );
        glVertex2f( pos.x + size.w, pos.y );
        glVertex2f( pos.x + size.w, pos.y + size.h );
        glVertex2f( pos.x, pos.y + size.h );
        glEnd();
    }
}
