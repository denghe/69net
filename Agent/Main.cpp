#include "Lib/All.h"

int main()
{
    int bufLen = 32;
    SharedMemory::setPrefixName( "69" );
    auto& smb = *(SMBuffer*)SharedMemory::create( 123, bufLen );
    smb.init( bufLen );

    int i = 0;
    while( true )
    {
        if( smb.space() )
        {
            char buf = ++i % bufLen;
            smb.write( &buf, 1 );
        }
        else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    system( "pause" );
    return 0;
}
