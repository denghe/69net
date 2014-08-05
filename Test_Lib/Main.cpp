#include "Lib/All.h"
#include "PacketTestPackets.h"
using namespace PacketTestPackets;

int main()
{
    FlatBuffer fb;
    Foo f;
    f._isss[ 0 ][ 0 ][ 0 ] = 1;
    f._isss[ 1 ][ 0 ][ 0 ] = 2;
    f._isss[ 2 ][ 0 ][ 0 ] = 3;
    f._isss[ 0 ][ 1 ][ 0 ] = 4;
    f._isss[ 1 ][ 1 ][ 0 ] = 5;
    f._isss[ 2 ][ 1 ][ 0 ] = 6;
    fb.write( f );
    Cout( fb.dump() );

    //FlatBuffer fb;
    ////Foo3 f;
    ////f._f._f._bool = true;
    ////f._f._f._byte = 1;
    ////f._f._f._short = 2;
    ////f._f._f._int = 3;
    ////f._f._f._long = 4;
    ////f._f._int = 5;
    ////f._s = "asdfqwer";

    //int n = 99999999;

    //Foo f;
    //Stopwatch sw;
    //sw.reset();
    //for( int i = 0; i < n; ++i )
    //{
    //    fb.clear();
    //    f.writeBufferDirect( fb );
    //}
    //Cout( sw.elapsedMillseconds() );
    //Cout( fb.dump() );

    //sw.reset();
    //for( int i = 0; i < n; ++i )
    //{
    //    fb.offset() = 0;
    //    if( !f.readBuffer( fb ) ) return 0;
    //}
    //Cout( sw.elapsedMillseconds() );
    //Cout( f._byte );
    ////Cout( f._long );

    //Cout( std::is_pod<Foo>::value );
    //Cout( std::is_pod<Foo2>::value );
    //Cout( std::is_pod<Foo3>::value );

    system( "pause" );
    return 0;
}
