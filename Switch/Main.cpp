#include "Lib/All.h"
#include "LuaEx.h"

using namespace xxx;

struct Foo1
{
    int x = 0;
    int y = 0;
    inline void SetXY( int x, int y )
    {
        this->x = x; this->y = y;
    }
};

int main()
{
    LuaEx L;
    L.OpenLibBase();
    Foo1 f1;
    L.Struct( &f1, "Foo1" )
        .Field( "x", &Foo1::x )
        .Field( "y", &Foo1::y, true )
        .Function( "SetXY", []( Lua L )
        {
            if( auto o = L.CheckAndGetUpValue<Foo1, int, int>() )
            {
                o->SetXY( L.ToInt( -2 ), L.ToInt( -1 ) );
            }
            return 0;
        } )
        .Function( "GetXY", []( Lua L )
        {
            if( auto o = L.GetUpValue<Foo1>() )
            {
                L.Push( o->x, o->y );
                return 2;
            }
            return 0;
        } );
    L.DoString( R"--(
function f1()
    Foo1.SetXY( 123, 456 )
    print( Foo1.GetXY() )
    Foo1.x = 0
    Foo1.y = 0    -- readonly
    print( Foo1.GetXY() )
end
)--" );
    L.PcallPop( "f1", nullptr );
    system( "pause" );
    return 0;
}



//struct Foo2
//{
//    String x;
//};

//Foo1 f1; Foo2 f2, f3;
//CoutLine( "f1.x, f1.y, f2.x, f3.x = ", f1.x, " ", f1.y, " ", f2.x, " ", f3.x );

//LuaStruct<Foo2>().Field( "x", &Foo2::x );
//L.Struct( &f2, "Foo2" );
//L.Struct( &f3, "Foo3" );

//L.PcallPop( "f1", nullptr, 12, 34, "asdfqwer", "eee" );
//L.PcallPop( "f2", [ &]( int n ) { L.DumpMulti( n ); } );


//function xxx( ... )
//    print( 1 )
//    local t = {...}
//    print( 2 )
//    table.insert( t, "asdf" )
//    table.insert( t, 12 )
//    table.insert( t, 34.5 )
//    print( 3 )
//    return table.unpack( t ) 
//end
//function f1( x, y, name1, name2 )
//    Foo1.x = x
//    Foo1.y = y
//    Foo2.x = name1
//    Foo3.x = name2
//end
//function f2()
//    return Foo1.x, Foo1.y, Foo2.x, Foo3.x
//end