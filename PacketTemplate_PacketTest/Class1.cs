#pragma warning disable 0169
using PacketLibrary;

class Foo
{
    [Limits( 1, 2, 3 )]
    int[][][] isss;

    //[Limits( 3, 5, 1, 16, 0, 100, 8 )]
    //Dict<string, List<byte[]>>[][] eee;
}

//class Foo
//{
//    [Desc( "Byte的备注" )]
//    byte    Byte;
//    [Desc( "Ushort的备注" )]
//    ushort  Ushort;
//    [Desc( "Uint的备注" )]
//    uint    Uint;
//    ulong   Ulong;
//    sbyte   Sbyte;
//    short   Short;
//    int     Int;
//    long    Long;
//    double  Double;
//    float   Float;
//    bool    Bool;
//}

//[Desc( "pod class too" )]
//class Foo2
//{
//    short   Short;
//    int     Int;
//    Foo     f;
//    long    Long;
//}

//[Desc( "have string member, is not pod but can memmove" )]
//class Foo3
//{
//    Foo2    f;
//    string  s;
//}

//class Bar
//{
//    [Limit( 16 )]
//    byte  [] Bytes;
//    [Limit( 16 )]
//    ushort[] Ushorts;
//    [Limit( 16 )]
//    uint  [] Uints;
//    [Limit( 16 )]
//    ulong [] Ulongs;
//    [Limit( 16 )]
//    sbyte [] Sbytes;
//    [Limit( 16 )]
//    short [] Shorts;
//    [Limit( 16 )]
//    int   [] Ints;
//    [Limit( 16 )]
//    long  [] Longs;
//    [Limit( 16 )]
//    double[] Doubles;
//    [Limit( 16 )]
//    float [] Floats;
//    [Limit( 16 )]
//    bool  [] Bools;
//}
