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
    List<void*> ps;
    int c16 = 0, c32 = 0, c8 = 0, c4 = 0, c64 = 0;
    for( int i = 0; i < 9999999; ++i )
    {
        auto p = (size_t)( void* )new char();
        if( (p & 63 ) == 0 ) c64++;
        else if( ( p & 31 ) == 0 ) c32++;
        else if( ( p & 15 ) == 0 ) c16++;
        else if( ( p & 7 ) == 0 ) c8++;
        else if( ( p & 3 ) == 0 ) c4++;
        ps.Push( (void*)p );
    }
    CoutLine( c64, " ", c32, " ", c16, " ", c8, " ", c4 );


    std::unordered_map<void*, int> map;
    Dict<void*, int> dict;
    dict.Reserve( 9999999 );
    Stopwatch sw;
    for( int i = 0; i < 9999999; ++i )
    {
        map.insert( std::make_pair( ps[ i ], i ) );
    }
    CoutLine( "map insert ms = ", sw.ElapsedMillseconds() );
    sw.Reset();
    for( int i = 0; i < 9999999; ++i )
    {
        dict.Insert( ps[ i ], i );
    }
    CoutLine( "dict insert ms = ", sw.ElapsedMillseconds() );



    return 0;
/*
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

    return 0;*/
}
