#include "Lib/All.h"
int main()
{
    // wait sm keeper( Agent project ) create sm
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    SharedMemory::setPrefixName( "69" );
    auto buf = SharedMemory::get( 123, 4096 );
    Cout( (size_t)buf );

    system( "pause" );
    return 0;
}
