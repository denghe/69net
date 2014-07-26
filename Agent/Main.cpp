#include "Lib/All.h"
int main()
{
    SharedMemory::setPrefixName( "69" );
    auto buf = SharedMemory::create( 123, 4096 );
    Cout( (size_t)buf );

    system( "pause" );
    return 0;
}
