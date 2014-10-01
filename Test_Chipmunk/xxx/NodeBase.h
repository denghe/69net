#ifndef __NODEBASE_H__
#define __NODEBASE_H__

namespace xxx
{
    class NodeBase : public Ref
    {
    public:

#ifdef USE_STL
        vector<NodeBase*> childs;
#else
        List<NodeBase*> childs;
#endif
        NodeBase* parent = nullptr;

        // return new Node();
        static NodeBase* Create();

        // override 尾部 call Node::Draw 以处理 childs
        virtual void Draw( int durationTicks );

        // override 头部 call Node::Draw 以处理 add 流程
        virtual void Add( NodeBase* child );

        // override 头部先 bool alive = child->refCount > 1, 再 call Node::Remove, 最后再 if( alive ) {  code ... }
        virtual void Remove( NodeBase* child );

        // remove all childs
        void Clear();

        void RemoveFromParent();

    protected:
        NodeBase() = default;
        ~NodeBase();
    };
}

#endif
