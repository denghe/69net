//#include "Lib/All.h"
//using namespace xxx;
//int main()
//{
//    // wait sm keeper( Agent project ) create sm
//    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
//
//    // ÿ 0.2 ����ʾһ��
//    std::chrono::milliseconds refreshDuration( 200 );
//    std::chrono::time_point<std::chrono::system_clock> lastTime;
//    auto cout = [ &]( int64 val )
//    {
//        auto now = std::chrono::system_clock::now();
//        if( now - lastTime >= refreshDuration )
//        {
//            CoutPos( 0, 0, val, ' ' );
//            lastTime = now;
//        }
//    };
//
//    int bufLen = 1024 * 500;
//    SharedMemory::SetPrefixName( "69" );
//    auto& smb = *(SMBuffer*)SharedMemory::Get( 123, bufLen );
//
//    int64 counter;
//    while( true )
//    {
//        if( smb.Size() >= sizeof( counter ) )
//        {
//            smb.Read( (char*)&counter, sizeof( counter ) );
//            cout( counter );
//        }
//        else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
//    }
//
//    system( "pause" );
//    return 0;
//}
//
