#include "Precompile.h"

struct Point
{
    float x, y;
};
struct Size
{
    float width, height;
};
struct PositionNode : public Node
{
    Point position;
    Point anchor;
};
struct BoxNode : PositionNode
{
    Size size;
};


struct Scene1 : public Scene
{
    Scene1()
    {
        glClearColor( 0, 0, 0, 0 );
        //glShadeModel( GL_FLAT );
    }
    void Draw( int durationTicks ) override
    {
        glClear( GL_COLOR_BUFFER_BIT );
        Node::Draw( durationTicks );
    }
};

struct BoxShapeNode : public BoxNode
{
    BoxShapeNode()
    {
        position.x = position.y = 0;
        anchor.x = anchor.y = 0.5f;
        size.width = size.height = 0;
    }
    void Draw( int durationTicks ) override
    {
        glBegin( GL_LINE_LOOP );
        glVertex2f( position.x, position.y );
        glVertex2f( position.x + size.width, position.y );
        glVertex2f( position.x + size.width, position.y + size.height );
        glVertex2f( position.x, position.y + size.height );
        glEnd();
        Node::Draw( durationTicks );
    }
    static BoxShapeNode* create() { return new BoxShapeNode(); }
};


Game1::~Game1()
{
    delete G::scene;
}
Game1::Game1()
{
    // init G::window
    G::window->Init( L"test", 768, 1024, 0, 0, true );
    G::window->resizeCallback = []
    {
        glViewport( 0, 0, G::window->width, G::window->height );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D( 0, G::window->width, 0, G::window->height );
    };
    //G::window->setVsync( false );

    // init G::scene
    new Scene1();

    auto box = BoxShapeNode::create();
    box->position = { 5, 5 };
    box->size = { 100, 100 };

    G::scene->Add( box );
}

void Game1::Update()
{
    // todo: loop code here
}


/*
设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )
*/
