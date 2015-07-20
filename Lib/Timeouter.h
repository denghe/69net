#pragma once

namespace xxx
{
    // () 超过指定时长将返回 true 并更新最后调用时间
    // 当前主用于限制控制台输出量, 控制频次, 放弃部分显示机会, 免得拖慢
    struct Timeouter
    {
        std::chrono::time_point<std::chrono::system_clock> lastTime = std::chrono::system_clock::now();
        int timeoutMs;
        explicit Timeouter( int timeoutMs = 200 ) : timeoutMs( timeoutMs ) {}
        inline bool Timeout()
        {
            auto now = std::chrono::system_clock::now();
            auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>( now - lastTime ).count();
            if( elapsedMs >= timeoutMs )
            {
                lastTime = now;
                return true;
            }
            return false;
        }
        inline bool operator()()
        {
            return Timeout();
        }
    };
}

