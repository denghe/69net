#include "Precompile.h"

namespace xxx
{
    void Box::Drawing( int _durationTicks )
    {
        glColor3ubv( (GLubyte*)&color );
        glBegin( GL_LINE_LOOP );
        glVertex2f( worldPos.x, worldPos.y );
        glVertex2f( worldPos.x + size.w, worldPos.y );
        glVertex2f( worldPos.x + size.w, worldPos.y + size.h );
        glVertex2f( worldPos.x, worldPos.y + size.h );
        glEnd();
    }
}
