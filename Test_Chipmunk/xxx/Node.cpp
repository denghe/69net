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
        
        idx = 0;
        prev = next = first = last = nullptr;
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

        Node* o = first;
        while( o )
        {
            o->Draw( _durationTicks );
            o = o->next;
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
        _child->idx = childs.Size();

        if( !childs.Size() )            // 第 1 个被加进集合的
        {
            first = last = _child;
            _child->prev = _child->next = nullptr;
        }
        else
        {
            last->next = _child;
            _child->prev = last;
            _child->next = nullptr;
            last = _child;
        }

        childs.Push( _child );
    }

    void Node::Remove( Node* _child )
    {
        assert( _child != this );
        if( first == last )
        {
            first = last = nullptr;
            childs.Erase( 0 );
        }
        else
        {
            if( first == _child )
            {
                _child->next->prev = nullptr;
                first = _child->next;
            }
            else if( last == _child )
            {
                _child->prev->next = nullptr;
                last = _child->prev;
            }
            else
            {
                _child->prev->next = _child->next;
                _child->next->prev = _child->prev;
            }

            childs.Top()->idx = _child->idx;    // 修正索引
            childs.EraseFast( _child->idx );    // 将最后个元素移到当前要 erase 的位置, --size
        }

        _child->parent = nullptr;
        _child->Removed();
        _child->Release();
    }

    void Node::RemoveFromParent()
    {
        assert( parent );
        parent->Remove( this );
    }

    void Node::Clear()
    {
        for( int i = childs.Size() - 1; i >=0; --i )
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
