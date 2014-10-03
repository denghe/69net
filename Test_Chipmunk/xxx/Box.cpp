#include "Precompile.h"

namespace xxx
{
    void Box::Draw( int durationTicks )
    {
        // todo: angle support? cache calc result? render list?
        if( this->parent->dirty )
        {
            this->dirty = true;
        }
        if( this->dirty )
        {
            this->position =
            {
                this->parent->position.x
                + this->parent->size.width * this->dock.x
                + this->offset.x
                - this->size.width * this->anchor.x,
                this->parent->position.y
                + this->parent->size.height * this->dock.y
                + this->offset.y
                - this->size.height * this->anchor.y,
            };
        }

        auto x = this->position.x;
        auto y = this->position.y;
        auto w = this->size.width;
        auto h = this->size.height;
        glColor3ubv( (GLubyte*)&color );
        glBegin( GL_LINE_LOOP );
        glVertex2f( x, y );
        glVertex2f( x + w, y );
        glVertex2f( x + w, y + h );
        glVertex2f( x, y + h );
        glEnd();

        Node::Draw( durationTicks );

        this->dirty = false;
    }
}
