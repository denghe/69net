#include "Precompile.h"

Bullet::Bullet()
{
    size = { 16 * 1, 16 * 2 };
}

void Bullet::Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
{
    assert( !node );

    pos = _pos;
    xyInc = { ( rand() % 100 ) / 20.0f - 2.5f, 10 };

    nodeContainer = _nodeContainer;
    node = ::Create<Box>();
    node->size = size;
    node->pos = _pos;
    node->color = { 111, 255, 111, 0 };
    nodeContainer->Add( node );

    cditemContainer = _cditemContainer;
    cditem = _cditemContainer->CreateItem();
    cditem->Init( { size.w / 2, size.h / 2 }, { G::scene->size.w, G::scene->size.h }, 2, 0xFFFFFFFFu, this );
    cditem->Index();
}

bool Bullet::Update()
{
    if( pos.x < -size.w || pos.x > G::scene->size.w + size.w
        || pos.y < -size.h || pos.y > G::scene->size.h + size.h ) return false;

    pos.x += xyInc.x;
    pos.y += xyInc.y;

    if( node )
    {
        node->pos = pos;
        node->dirty = true;
    }
    if( cditem )
    {
        cditem->Update( { G::scene->size.w + pos.x, G::scene->size.h + pos.y } );
    }

    // ��������������
    if( auto tar = cditem->GetCollisionItem() )
    {
        auto m = (Monster*)tar->userData;
        m->Destroy();
        return false;
    }

    return true;
}

void Bullet::Destroy()
{
    node->RemoveFromParent();
    node = nullptr;
    cditem->Destroy();
    cditem = nullptr;

    objs.erase( idx );
    objPool.push( this );
}

Bullet* Bullet::Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
{
    Bullet* rtv;
    if( objPool.size() )
    {
        rtv = objPool.top_pop();
    }
    else rtv = new Bullet();
    rtv->Init( _nodeContainer, _cditemContainer, _pos );
    rtv->idx = objs.insert( rtv ).first;
    return rtv;
}

void Bullet::FreeObjs()
{
    for( int i = objs.size() - 1; i >= 0; --i )
    {
        delete objs[ i ]->key;
    }
    for( int i = objPool.size() - 1; i >= 0; --i )
    {
        delete objPool[ i ];
    }
}
Hash<Bullet*> Bullet::objs;
List<Bullet*> Bullet::objPool;