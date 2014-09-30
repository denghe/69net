#ifndef __OUTPUTER_H__
#define __OUTPUTER_H__

class Shape;
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
