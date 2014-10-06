#ifndef __OBJECT_H__
#define __OBJECT_H__

struct Object : public Box
{
    Point xyInc;            // x, y increase value per frame
    bool Update();
};

#endif
