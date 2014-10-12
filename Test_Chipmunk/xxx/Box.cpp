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

        //if( dirty )
        //{
        //    vertexs[ 0 ] = worldPos.x;  vertexs[ 1 ] = worldPos.y;
        //    vertexs[ 2 ] = worldPos.x;  vertexs[ 3 ] = +size.w, worldPos.y;
        //    vertexs[ 4 ] = worldPos.x;  vertexs[ 5 ] = +size.w, worldPos.y + size.h;
        //    vertexs[ 6 ] = worldPos.x;  vertexs[ 7 ] = worldPos.y + size.h;
        //}

        //glColor3ubv( (GLubyte*)&color );
        //glVertexPointer( 2, GL_FLOAT, 0, vertexs );
        //glBegin( GL_LINE_LOOP );
        //glArrayElement( 0 );
        //glArrayElement( 1 );
        //glArrayElement( 2 );
        //glArrayElement( 3 );
        //glEnd();
    }
}
