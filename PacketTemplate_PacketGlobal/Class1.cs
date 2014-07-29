using PacketLibrary;

[Desc( "this is Enum1" )]
enum Enum1 : byte
{
    a = 1,
    [Desc( "this is Enum1.b" )]
    b,
    c
}

[Desc( "this is Foo" )]
class Foo
{
    [Desc( "this is Foo.e1" )]
    Enum1 e1;

    Enum1[] e2;

    [Limit( 50 )]
    int[] p1;

    [Limit( 50 )]
    string[] p2;

    byte[][] p3;

    Dict<Enum1, string> p4;
}


class Item
{
    int level;
    string name;
}
class Player
{
    int level;
    string nickname;
    double hp;
    double atk;
    double def;
    Item[] items;
}
