#include "Lib/All.h"
template<typename ...TS>
void cout( TS const& ...parms )
{
    String s;
    s.append( parms... );
    puts( s.c_str() );
}
template<typename ...TS>
void coutn( TS const& ...parms )
{
    cout( parms..., '\n' );
}
#include <windows.h>
template<typename ...TS>
void coutPos( short x, short y, TS const& ...parms )
{
    static auto h = GetStdHandle( STD_OUTPUT_HANDLE );
    static COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition( h, pos );
    cout( parms... );
}

int main()
{
    // 来个 counter 用于显示 以体现 循环速度
    int counter = 0;
    // timer 相关
    std::chrono::milliseconds refreshDuration( 200 );
    std::chrono::seconds durationSec1( 1 );
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    auto firstTime = std::chrono::system_clock::now();

    while( true )
    {
        counter++;
        std::this_thread::sleep_for( std::chrono::milliseconds( 0 ) );

        auto now = std::chrono::system_clock::now();
        if( now - lastTime >= refreshDuration )
        {
            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
            if( !elapsedSeconds ) elapsedSeconds = 1;
            coutPos( 0, 0, counter, ", ", counter / elapsedSeconds );
            lastTime = now;
        }
    }

    return 0;
}
