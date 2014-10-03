#include "Precompile.h"

namespace xxx
{

    Node* Node::Create()
    {
        return new Node();
    }

    Node::Node()
    {
        size = { 0, 0 };
        dock = { 0, 0 };
        anchor = { 0.5f, 0.5f };
        offset = { 0, 0 };
        color = { 255, 255, 255, 0 };
    }

    Node::~Node()
    {
        Node::Clear();
    }

    void Node::Draw( int _durationTicks )
    {
#ifdef USE_STL
        for( auto& o : childs )
        {
            o->Draw( _durationTicks );
        }
#else
        for( int i = 0; i < childs.size(); ++i )
        {
            childs[ i ]->Draw( _durationTicks );
        }
#endif
    }

    void Node::Add( Node* _child )
    {
        assert( !_child->parent && _child != this );
        _child->Retain();
        _child->parent = this;
#ifdef USE_STL
        childs.push_back( _child );
#else
        childs.push( _child );
#endif
    }

    void Node::Remove( Node* _child )
    {
        assert( _child != this );
#ifdef USE_STL
        auto it = find( childs.begin(), childs.end(), _child );
        assert( it != childs.end() );
        childs.erase( it );
#else
        auto i = childs.find( _child );
        assert( i >= 0 );
        childs.erase( i );
#endif
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
#ifdef USE_STL
        for( auto& o : childs )
        {
            Remove( o );
        }
#else
        for( int i = 0; i < childs.size(); ++i )
        {
            Remove( childs[ i ] );
        }
#endif
    }

    void Node::Added()
    {
        dirty = true;
    }

    void Node::Removed()
    {
    }

}
