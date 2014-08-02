#include "Lib/All.h"
#include "PacketTest.h"
using namespace PacketTestPackets;

int main()
{
    FlatBuffer fb;
    Foo3 f;
    f._f._f._bool = true;
    f._f._f._byte = 1;
    f._f._f._short = 2;
    f._f._f._int = 3;
    f._f._f._long = 4;
    f._f._int = 5;
    f._s = "asdfqwer";

    auto f2 = std::move( f );

    // todo: move 后清0简单类型的生成？ 简单类型不需要加 move?

    //fb.write( f );
    //Cout( fb.dump() );

    //Cout( std::is_pod<Foo>::value );
    //Cout( std::is_pod<Foo2>::value );
    //Cout( std::is_pod<Foo3>::value );

    system( "pause" );
    return 0;
}
