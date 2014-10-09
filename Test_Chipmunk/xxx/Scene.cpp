#include "Precompile.h"

namespace xxx
{
    Scene::Scene()
    {
        G::scene = this;

        color = { 0, 0, 0, 0 };      // bg color
        pos = { 0, 0 };
        worldPos = pos;

        //glClearColor( 0, 0, 0, 0 );
        //glShadeModel( GL_FLAT );
    }

    Scene::~Scene()
    {
        Node::Clear();
        G::scene = nullptr;
    }

    void Scene::Draw( int _durationTicks )
    {
        if( dirty )
        {
            glViewport( 0, 0, ( GLsizei )size.w, ( GLsizei )size.h );
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();

            // 等比修正 size( 存实际布局尺寸, 短边撑满: 即短边等于设计尺寸短边，长边变长 )
            if( designSize.w > designSize.h )
            {
                size.w = designSize.h * size.w / size.h;
                size.h = designSize.h;
            }
            else
            {
                size.h = designSize.w * size.h / size.w;
                size.w = designSize.w;
            }

            gluOrtho2D( 0, size.w, 0, size.h );
            glClearColor( color.r, color.g, color.b, color.a );
        }

        glClear( GL_COLOR_BUFFER_BIT );

        for( int i = 0; i < childs.size(); ++i )
        {
            childs[ i ]->Draw( _durationTicks );
        }
        dirty = false;                // resize 的时候会标脏
    }
}
