#ifndef __NODE_H__
#define __NODE_H__

class Node
{
public:
    Node();
    Node( Node* parent );
    virtual ~Node();
    void Add( Node* child );
    void Remove( Node* child );
    void RemoveFromParent();
    void Clear();
    virtual void Draw( int durationTicks );

    function<void( int durationTicks )> beforeDraw;
    function<void()> afterAdd;
    function<void()> afterRemove;
protected:
    vector<Node*> childs;
    Node* parent = nullptr;
};

#endif
