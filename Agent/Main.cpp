#include "Lib/All.h"

int main()
{
    // 每 0.2 秒输出一次总 counter 及 每秒 count
    std::chrono::milliseconds refreshDuration( 200 );
    std::chrono::seconds durationSec1( 1 );
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    auto firstTime = std::chrono::system_clock::now();
    auto cout = [ &]( int64 val )
    {
        auto now = std::chrono::system_clock::now();
        if( now - lastTime >= refreshDuration )
        {
            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
            if( !elapsedSeconds ) elapsedSeconds = 1;
            CoutPos( 0, 0, val, ", ", val / elapsedSeconds );
            lastTime = now;
        }
    };


    int bufLen = 4096;
    SharedMemory::setPrefixName( "69" );
    auto& smb = *(SMBuffer*)SharedMemory::create( 123, bufLen );
    smb.init( bufLen );

    int64 counter = 0;
    while( true )
    {
        if( smb.space() )
        {
            ++counter;
            smb.write( (char*)&counter, sizeof( counter ) );
            cout( counter );
        }
        //else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    system( "pause" );
    return 0;
}
