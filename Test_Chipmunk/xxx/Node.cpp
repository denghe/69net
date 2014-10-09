#include "Precompile.h"

namespace xxx
{
    Node::Node()
    {
        size = { 0, 0 };
        dock = { 0, 0 };
        anchor = { 0.5f, 0.5f };
        pos = { 0, 0 };
        color = { 255, 255, 255, 0 };
        userData = nullptr;
        dirty = true;
        parent = nullptr;
    }

    Node::~Node()
    {
        Node::Clear();
    }

    void Node::Draw( int _durationTicks )
    {
        if( parent->dirty )
        {
            dirty = true;
        }
        if( dirty )
        {
            worldPos =
            {
                parent->worldPos.x
                + parent->size.w * dock.x
                + pos.x
                - size.w * anchor.x,

                parent->worldPos.y
                + parent->size.h * dock.y
                + pos.y
                - size.h * anchor.y,
            };
        }

        Drawing( _durationTicks );

        for( int i = 0; i < childs.size(); ++i )
        {
            childs[ i ]->Draw( _durationTicks );
        }
        dirty = false;
    }

    void Node::Drawing( int _durationTicks )
    {
    }

    void Node::Add( Node* _child )
    {
        assert( !_child->parent && _child != this );
        _child->Retain();
        _child->parent = this;
        childs.push( _child );
    }

    void Node::Remove( Node* _child )
    {
        assert( _child != this );
        auto i = childs.find( _child );
        assert( i >= 0 );
        childs.erase( i );
        _child->parent = nullptr;

        if( _child->refCount > 1 )
        {
            _child->Release();
            _child->Removed();
        }
        else
        {
            _child->Release();
        }
    }

    void Node::RemoveFromParent()
    {
        assert( parent );
        parent->Remove( this );
    }

    void Node::Clear()
    {
        for( int i = childs.size() - 1; i >=0; --i )
        {
            Remove( childs[ i ] );
        }
    }

    void Node::Added()
    {
        dirty = true;
    }

    void Node::Removed()
    {
    }

}
