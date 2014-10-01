#include "Precompile.h"

namespace xxx
{

    NodeBase* NodeBase::Create()
    {
        return new NodeBase();
    }

    NodeBase::~NodeBase()
    {
        NodeBase::Clear();
    }

    void NodeBase::Draw( int durationTicks )
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

    void NodeBase::Add( NodeBase* child )
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

    void NodeBase::Remove( NodeBase* child )
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
        child->Release();
    }

    void NodeBase::RemoveFromParent()
    {
        assert( this->parent );
        this->parent->Remove( this );
    }

    void NodeBase::Clear()
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

}
