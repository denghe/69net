#include "Lib/All.h"
#include "PacketTest.h"

int main()
{
    FlatBuffer fb;
    PacketTestPackets::Foo2 f;
    fb.write( f );
    Cout( fb.dump() );

    system( "pause" );
    return 0;
}
