#ifndef __LOOPER_H__
#define __LOOPER_H__
#include "Lib/All.h"
#include "Logic.h"

class Looper
{
public:
    Looper( Logic* logic );
    ~Looper();
    void update( int durationTicks );
//private:
    Logic* _logic;
    int _logicFrameTicks = 1000 / 60;                           // 每帧 tick 数( 1 tick = 1 ms )
    int _logicFrameTicksLimit = _logicFrameTicks * 1;           // 帧间隔时长限定：超过这个时长也只算 1 帧
    int _accumulatTicks = 0;                                    // 积蓄时长
};

#endif
