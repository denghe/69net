using PacketLibrary;

class Foo
{
     byte    Byte  ;
     ushort  Ushort;
     uint    Uint  ;
     ulong   Ulong ;
     sbyte   Sbyte ;
     short   Short ;
     int     Int   ;
     long    Long  ;
     double  Double;
     float   Float ;
     bool    Bool;
}

class Bar
{
    [Limit( 16 )] byte  [] Bytes;
    [Limit( 16 )] ushort[] Ushorts;
    [Limit( 16 )] uint  [] Uints;
    [Limit( 16 )] ulong [] Ulongs;
    [Limit( 16 )] sbyte [] Sbytes;
    [Limit( 16 )] short [] Shorts;
    [Limit( 16 )] int   [] Ints;
    [Limit( 16 )] long  [] Longs;
    [Limit( 16 )] double[] Doubles;
    [Limit( 16 )] float [] Floats;
    [Limit( 16 )] bool  [] Bools;
}
