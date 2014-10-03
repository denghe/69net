#ifndef __INPUT_H__
#define __INPUT_H__

namespace xxx
{
    class Input
    {
    public:
        Input();
        ~Input();

        bool touching = false;
        Point touchPos;
        FlatQueue<TouchEvent> touchEvents;
    };
}

#endif
