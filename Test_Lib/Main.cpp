#include "Lib/All.h"

int main()
{
    // wait sm keeper( Agent project ) create sm
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    // 每 0.2 秒显示一次的 cout
    std::chrono::milliseconds refreshDuration( 200 );
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    auto cout = [ &]( int64 val )
    {
        auto now = std::chrono::system_clock::now();
        if( now - lastTime >= refreshDuration )
        {
            CoutPos( 0, 0, val, ' ' );
            lastTime = now;
        }
    };

    int bufLen = 102400;
    SharedMemory::setPrefixName( "69" );
    auto& smb = *(SMBuffer*)SharedMemory::create( 123, bufLen );


    int64 buf;
    while( true )
    {
        if( smb.size() )
        {
            smb.read( (char*)&buf, sizeof( buf ) );
            cout( buf );
        }
        // else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    system( "pause" );
    return 0;
}
