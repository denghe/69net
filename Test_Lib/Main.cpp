#include "Lib/All.h"
using namespace xxx;


class Foo
{
public:
    int i;
    double d;
    float f;
    String s;
    List<short> ss;

    // ByteBuffer interface
    inline void WriteTo( ByteBuffer& bb ) const
    {
        bb.WriteMulti( i, d, f, s, ss );
    }
    inline void FastWriteTo( ByteBuffer& bb ) const
    {
        bb.FastWriteMulti( i, d, f, s, ss );
    }
    inline bool ReadFrom( ByteBuffer& bb )
    {
        return bb.ReadMulti( i, d, f, s, ss );
    }
};

int main()
{
    ByteBuffer bb;
    Foo f{ 123, 4.5, 6.7f, "asdfqw", { 1, 2, 3, 4, 5 } };
    Stopwatch sw;
    for( int i = 0; i < 100000000; ++i )
    {
        bb.Clear();
        bb.Write( f );
    }
    Cout( sw.ElapsedMillseconds() );
    CoutLine( bb.Dump() );

    Foo f2;
    f2.ReadFrom( bb );

    CoutLine( f2.i, " ", f2.d, " ", f2.f, " ", f2.s );
    for( int i = 0; i < f2.ss.Size(); ++i ) Cout( i ? "," : "", f2.ss[ i ] );
    CoutLine();

    return 0;
}
