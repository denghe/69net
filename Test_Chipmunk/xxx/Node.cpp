#include "Precompile.h"

namespace xxx
{

    Node* Node::Create()
    {
        return new Node();
    }

    Node::Node()
    {
        this->size = { 0, 0 };
        this->dock = { 0, 0 };
        this->anchor = { 0.5f, 0.5f };
        this->offset = { 0, 0 };
        this->color = { 255, 255, 255, 0 };
        this->dirty = true;
    }

    Node::~Node()
    {
        Node::Clear();
    }

    void Node::Draw( int durationTicks )
    {
#ifdef USE_STL
        for( auto& o : this->childs )
        {
            o->Draw( durationTicks );
        }
#else
        for( int i = 0; i < this->childs.size(); ++i )
        {
            this->childs[ i ]->Draw( durationTicks );
        }
#endif
    }

    void Node::Add( Node* child )
    {
        assert( !child->parent && child != this );
        child->Retain();
        child->parent = this;
#ifdef USE_STL
        this->childs.push_back( child );
#else
        this->childs.push( child );
#endif
    }

    void Node::Remove( Node* child )
    {
        assert( child != this );
#ifdef USE_STL
        auto it = find( this->childs.begin(), this->childs.end(), child );
        assert( it != this->childs.end() );
        this->childs.erase( it );
#else
        auto i = this->childs.find( child );
        assert( i >= 0 );
        this->childs.erase( i );
#endif
        child->parent = nullptr;

        if( child->refCount > 1 )
        {
            child->Release();
            child->Removed();
        }
        else
        {
            child->Release();
        }
    }

    void Node::RemoveFromParent()
    {
        assert( this->parent );
        this->parent->Remove( this );
    }

    void Node::Clear()
    {
#ifdef USE_STL
        for( auto& o : this->childs )
        {
            this->Remove( o );
        }
#else
        for( int i = 0; i < this->childs.size(); ++i )
        {
            this->Remove( this->childs[ i ] );
        }
#endif
    }

    void Node::Added()
    {
        this->dirty = true;
    }

    void Node::Removed()
    {
    }

}
