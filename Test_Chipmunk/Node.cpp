#include "Precompile.h"

Node::Node()
{
}

Node::~Node()
{
    for( auto& o : this->childs ) delete o;
}

void Node::Draw( int durationTicks )
{
    if( beforeDraw ) beforeDraw( durationTicks );
    for( auto& o : this->childs )
    {
        o->Draw( durationTicks );
    }
}

void Node::Add( Node* child )
{
    assert( !child->parent && child != this );
    child->parent = this;
    this->childs.push_back( child );
    if( child->afterAdd ) child->afterAdd();
}

void Node::Remove( Node* child )
{
    assert( child != this );
    auto it = find( this->childs.begin(), this->childs.end(), child );
    assert( it != this->childs.end() );
    this->childs.erase( it );
    child->parent = nullptr;
    if( child->afterRemove ) child->afterRemove();
}

void Node::RemoveFromParent()
{
    assert( this->parent );
    this->parent->Remove( this );
}

void Node::Clear()
{
    for( auto& o : this->childs )
    {
        this->Remove( o );
    }
}
