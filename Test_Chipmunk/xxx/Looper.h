#ifndef __LOOPER_H__
#define __LOOPER_H__

namespace xxx
{
    class Looper
    {
    public:
        void Update( int durationTicks );
    //private:
        int logicFrameTicks = 1000 / 60;                        // 每帧 tick 数( 1 tick = 1 ms )
        int logicFrameTicksLimit = logicFrameTicks * 1;         // 帧间隔时长限定：超过这个时长也只算 1 帧
        int accumulatTicks = 0;                                 // 积蓄时长
    };
}

#endif
