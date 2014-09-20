#ifndef __LOOPER_H__
#define __LOOPER_H__
#include "Lib/All.h"
#include "Logic.h"
#include "Render.h"

class Looper
{
public:
    Looper( Logic* logic, Render* render = nullptr );
    ~Looper();
    void update( int durationTicks );
private:
    Logic* _logic;
    Render* _render;

    // todo: 参数设置
    int _logicFrameTicks = 1000 / 60;                           // 每帧毫秒数, 当前为 1 tick = 1 ms
    int _logicFrameTickLimit = _logicFrameTicks * 1;            // * 1: 不跳帧

    int _accumulatTicks = 0;                                    // 积蓄时长
    int _renderTicks = 0;                                       // 渲染的传入时长( 与执行了多少次帧逻辑相符 )
};

#endif
