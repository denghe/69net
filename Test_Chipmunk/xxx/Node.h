#ifndef __NODE_H__
#define __NODE_H__

namespace xxx
{
    struct Point
    {
        float x, y;
    };
    struct Size
    {
        float w, h;
    };
    struct Color3b
    {
        GLubyte r, g, b;
    };
    struct Color4b
    {
        GLubyte r, g, b, a;
    };


    struct Node : public Ref
    {
        Size    size;                               // 长宽( 逻辑概念，派生类具体杂用要看情况 )
        Point   dock;                               // 悬靠点( 0 ~ 1 百分比，相对于 parent size )
        Point   anchor;                             // 锚点( 0 ~ 1 百分比, 相对于 size )
        Point   pos;                                // 相对于悬靠点的锚点偏移坐标
        Color4b color;
        // todo: scale, angle  (matrix?) shader? blend?
        void*   userData;

        bool    dirty;                              // 脏标记( 默认会影响到子 )
        Point   worldPos;                           // 实际绘制用的全局坐标( draw 时 dirty 填充 )

        Node*   parent;
        int     idx;                                // 位于 childs 的索引。用于快速 Remove
        Node    *prev, *next, *first, *last;        // 双向链表。用于维持 Draw 的顺序
        List<Node*> childs;

        virtual void Draw( int _durationTicks );
        virtual void Drawing( int _durationTicks ); // Draw 的过程中会调用( 算完坐标啥的之后, 遍历子之前 )

        virtual void Add( Node* _child );
        virtual void Added();                       // 被 parent Add 后会调用该函数

        virtual void Remove( Node* _child );
        virtual void Removed();                     // 被 parent Remove 后会调用该函数( 如果还有引用计数的话 )
        void RemoveFromParent();
        virtual void Clear();                       // remove all childs

        Node();
        ~Node();
    };
}

#endif
