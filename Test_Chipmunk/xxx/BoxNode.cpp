#include "Precompile.h"

namespace xxx
{
    BoxNode::BoxNode()
    {
        this->position = { 0, 0 };
        this->anchor = { 0.5f, 0.5f };
        this->size = { 0, 0 };
        this->scale = { 1, 1 };
        this->color = { 255, 255, 255 };
        this->dirty = true;
    }

    void BoxNode::Draw( int durationTicks )
    {
        // todo: angle support? cache calc result? render list?
        if( this->dirty )
        {
            this->currSize =
            {
                this->size.width * this->scale.x,
                this->size.height * this->scale.y
            };
            this->currPosition =
            {
                this->position.x - this->size.width * this->anchor.x,
                this->position.y - this->size.height * this->anchor.y
            };
            this->dirty = false;
        }
        auto x = this->currPosition.x;
        auto y = this->currPosition.y;
        auto w = this->currSize.width;
        auto h = this->currSize.height;
        glColor3ubv( (GLubyte*)&color );
        glBegin( GL_LINE_LOOP );
        glVertex2f( x, y );
        glVertex2f( x + w, y );
        glVertex2f( x + w, y + h );
        glVertex2f( x, y + h );
        glEnd();
        NodeBase::Draw( durationTicks );
    }
}
