#include "Lib/All.h"
using namespace xxx;
int main()
{
    ByteBuffer bb;
    Stopwatch sw;
    for( int i = 0; i < 999999999; ++i )
    {
        bb.Clear();
        bb.WriteMulti( (ushort)12345, (byte)67, "asdf", 0.12f );
    }
    Cout( sw.ElapsedMillseconds() );
    Cout( bb.Dump() );
    return 0;
}
