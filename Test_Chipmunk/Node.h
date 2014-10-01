#ifndef __NODE_H__
#define __NODE_H__

class Node : public Ref
{
public:
    // return new Node();
    static Node* create();

    // override 尾部 call Node::Draw 以处理 childs
    virtual void Draw( int durationTicks );

    // override 头部 call Node::Draw 以处理 add 流程
    virtual void Add( Node* child );

    // override 头部先 bool alive = child->refCount > 1, 再 call Node::Remove, 最后再 if( alive ) {  code ... }
    virtual void Remove( Node* child );

    // remove all childs
    void Clear();

    void RemoveFromParent();

protected:
    Node() = default;
    ~Node();

#ifdef USE_STL
    vector<Node*> childs;
#else
    List<Node*> childs;
#endif
    Node* parent = nullptr;

};

#endif
