#ifndef __OBJECT_H__
#define __OBJECT_H__

struct Object : public Box
{
    STATIC_CREATE( Object );
    Point xyInc;            // x, y increase value per frame
    void Update();
};

#endif
