#include "Precompile.h"

namespace xxx
{
    void Box::Drawing( int _durationTicks )
    {
        //glColor3ubv( (GLubyte*)&color );
        //glBegin( GL_LINE_LOOP );
        //glVertex2f( worldPos.x, worldPos.y );
        //glVertex2f( worldPos.x + size.w, worldPos.y );
        //glVertex2f( worldPos.x + size.w, worldPos.y + size.h );
        //glVertex2f( worldPos.x, worldPos.y + size.h );
        //glEnd();

        if( dirty )
        {
            vertexs[ 0 ] = worldPos.x;  vertexs[ 1 ] = worldPos.y;
            vertexs[ 2 ] = worldPos.x + size.w;  vertexs[ 3 ] = worldPos.y;
            vertexs[ 4 ] = worldPos.x + size.w;  vertexs[ 5 ] = worldPos.y + size.h;
            vertexs[ 6 ] = worldPos.x;  vertexs[ 7 ] = worldPos.y + size.h;

            //vertexs_indexs[ 0 ] = 0;
            //vertexs_indexs[ 1 ] = 1;
            //vertexs_indexs[ 2 ] = 2;
            //vertexs_indexs[ 3 ] = 3;
        }

        glColor3ubv( (GLubyte*)&color );
        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 2, GL_FLOAT, 0, vertexs );
        //glDrawElements( GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, vertexs_indexs );
        glDrawArrays( GL_LINE_LOOP, 0, 4 );

        //glBegin( GL_LINE_LOOP );
        //glArrayElement( 0 );
        //glArrayElement( 1 );
        //glArrayElement( 2 );
        //glArrayElement( 3 );
        //glEnd();
    }
}
