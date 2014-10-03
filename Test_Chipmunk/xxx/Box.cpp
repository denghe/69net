#include "Precompile.h"

namespace xxx
{
    void Box::Draw( int _durationTicks )
    {
        // todo: angle support? cache calc result? render list?
        if( parent->dirty )
        {
            dirty = true;
        }
        if( dirty )
        {
            pos =
            {
                parent->pos.x
                + parent->size.w * dock.x
                + offset.x
                - size.w * anchor.x,

                parent->pos.y
                + parent->size.h * dock.y
                + offset.y
                - size.h * anchor.y,
            };
        }

        glColor3ubv( (GLubyte*)&color );
        glBegin( GL_LINE_LOOP );
        glVertex2f( pos.x, pos.y );
        glVertex2f( pos.x + size.w, pos.y );
        glVertex2f( pos.x + size.w, pos.y + size.h );
        glVertex2f( pos.x, pos.y + size.h );
        glEnd();

        Node::Draw( _durationTicks );

        dirty = false;
    }
}
