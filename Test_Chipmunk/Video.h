#ifndef __OUTPUTER_H__
#define __OUTPUTER_H__
#include "GLWindow.h"

class Video
{
public:
    Video( GLWindow* w );
    ~Video();

    void update( int durationTicks );
//private:
    GLWindow* _w;
};

#endif
