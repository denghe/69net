#include "Lib/All.h"

int main()
{
    // wait sm keeper( Agent project ) create sm
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    int bufLen = 32;
    SharedMemory::setPrefixName( "69" );
    auto& smb = *(SMBuffer*)SharedMemory::create( 123, bufLen );

    while( true )
    {
        char buf;
        if( smb.size() )
        {
            smb.read( &buf, 1 );
            Cout( (int)buf );
        }
        else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    system( "pause" );
    return 0;
}
