//#include "Lib/All.h"
//using namespace xxx;
//int main()
//{
//    // ÿ 0.2 �����һ���� counter �� ÿ�� count
//    std::chrono::milliseconds refreshDuration( 200 );
//    std::chrono::seconds durationSec1( 1 );
//    std::chrono::time_point<std::chrono::system_clock> lastTime;
//    auto firstTime = std::chrono::system_clock::now();
//    auto cout = [ &]( int64 val )
//    {
//        auto now = std::chrono::system_clock::now();
//        if( now - lastTime >= refreshDuration )
//        {
//            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
//            if( !elapsedSeconds ) elapsedSeconds = 1;
//            CoutPos( 0, 0, val, ", ", val / elapsedSeconds );
//            lastTime = now;
//        }
//    };
//
//
//    int bufLen = 1024 * 500;
//    SharedMemory::SetPrefixName( "69" );
//    auto& smb = *(SMBuffer*)SharedMemory::Create( 123, bufLen );
//    smb.Init( bufLen );
//
//    int64 counter = 0;
//    while( true )
//    {
//        if( smb.Space() )
//        {
//            ++counter;
//            smb.Write( (char*)&counter, sizeof( counter ) );
//            cout( counter );
//        }
//        else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
//    }
//
//    system( "pause" );
//    return 0;
//}
