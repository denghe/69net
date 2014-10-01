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
        float width, height;
    };
    struct Color3b
    {
        GLubyte r, g, b;
    };
    struct Color4b : public Color3b
    {
        GLubyte a;
    };
    struct Node : public NodeBase
    {
        Point   position;
        Point   anchor;
        Size    size;
        Point   scale;
        Color3b color;
        bool    dirty;
        // float angle, opacity;

        Point   currPosition;
        Size    currSize;
    };
}

#endif
