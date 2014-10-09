#include "Precompile.h"

Monster::Monster()
{
    size = { 16 * 2, 16 * 1 };
}

void Monster::Init( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
{
    assert( !cditem );

    pos = _pos;
    xyInc = { ( rand() % 100 ) / 50.0f - 1, -1 };

    node.size = size;
    node.pos = _pos;
    node.color = { 255, 0, 0, 0 };
    _nodeContainer->Add( &node );

    cditemContainer = _cditemContainer;
    cditem = _cditemContainer->CreateItem();
    cditem->Init( { size.w / 2, size.h / 2 }, { G::scene->size.w, G::scene->size.h }, 1, 0xFFFFFFFFu, this );
    cditem->Index();
}

bool Monster::Update()
{
    if( pos.x < -size.w || pos.x > G::scene->size.w + size.w
        || pos.y < -size.h || pos.y > G::scene->size.h + size.h ) return false;

    pos.x += xyInc.x;
    pos.y += xyInc.y;

    node.pos = pos;
    node.dirty = true;
    cditem->Update( { G::scene->size.w + pos.x, G::scene->size.h + pos.y } );

    return true;
}

void Monster::Destroy()
{
    node.RemoveFromParent();
    cditem->Destroy();
    cditem = nullptr;

    objs.erase( idx );
    objPool.push( this );
}

Monster* Monster::Create( Node* _nodeContainer, CdGrid* _cditemContainer, Point const& _pos )
{
    Monster* rtv;
    if( objPool.size() )
    {
        rtv = objPool.top_pop();
    }
    else rtv = new Monster();
    rtv->Init( _nodeContainer, _cditemContainer, _pos );
    rtv->idx = objs.insert( rtv ).first;
    return rtv;
}

void Monster::FreeObjs()
{
    for( int i = objs.size() - 1; i >= 0; --i )
    {
        auto o = objs[ i ]->key;
        o->node.RemoveFromParent();
        delete o;
    }
    for( int i = objPool.size() - 1; i >= 0; --i )
    {
        delete objPool[ i ];
    }
}

Hash<Monster*> Monster::objs;
List<Monster*> Monster::objPool;
