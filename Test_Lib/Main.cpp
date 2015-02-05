#include "Lib/All.h"
int main()
{
    xxx::ByteBuffer bb;
    bb.FastWriteMulti( (ushort)12345, (byte)67, "asdf", 0.12f );
    xxx::Cout( bb.Dump() );

    bb.Clear();

    return 0;
}
