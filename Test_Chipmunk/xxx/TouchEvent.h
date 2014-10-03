#ifndef __TOUCHEVENT_H__
#define __TOUCHEVENT_H__

namespace xxx
{
    enum class TouchEventType
    {
        Down, Move, Up, Cancel
    };

    class TouchEvent
    {
    public:
        TouchEventType type;
        float x;
        float y;
    };
}

#endif
