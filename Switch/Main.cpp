#include "Lib/All.h"
#include "LuaEx.h"

using namespace xxx;

struct Foo1
{
    int x = 0, y = 0;
    void SetXY( int x = 0, int y = 0 )
    {
        this->x = x; this->y = y;
    }
};
int main()
{
    LuaEx L;
    L.Struct<Foo1>()
        .Field( "X坐标", &Foo1::x )
        .Field( "Y坐标", &Foo1::y )
        .Function( "设置坐标", &Foo1::SetXY );
    Foo1 f1;
    L.SetGlobal( "类实例", &f1 );
    L.DoString( R"--(
function 函数名( 全角Ｘ, 全角Ｙ )
    类实例.设置坐标( 全角Ｘ, 全角Ｙ )
    print( 类实例.X坐标, 类实例.Y坐标 )
    类实例.X坐标, 类实例.Y坐标 = 11, 22
end
)--" );
    if( L.CallFunc( "函数名" , nullptr, 123, 456 ) )
    {
        CoutLine( "f1.x y = ", f1.x, " ", f1.y );
    }
    system( "pause" );
    return 0;
}

// todo: 错误处理
// todo: android/ios/linux 下测试



//LE.SetGlobal( "eee", []( Lua L )
//{
//    int i1, i2, i3, i4;
//    String s;
//    L.GetUpValue( 1, i1, i2, i3, i4, s );
//    Cout( "eee parms = ", i1, i2, i3, i4, s );
//    return 0;
//}, 1, 2, 3, 4, "asdf" );

//struct ER
//{
//    template<typename R, typename T, typename ...PS>
//    static auto xxx(R(T::* f)( PS... ))->decltype( f )
//    {
//        typedef R( T::* FuncType )( PS... );
//        auto v = (uint64)*(size_t*)&f;
//        CoutLine( v );
//        FuncType f2;
//        (uint64)*(size_t*)&f2 = v;
//        return f2;
//    }
//};

//Foo1 f;
//( f.*ER::xxx( &Foo1::SetXY ) )( 123, 456 );
//CoutLine( "f.x y = ", f.x, f.y );


//.Field( "x", &Foo1::x )
//.Field( "y", &Foo1::y, true )
//.Function( "SetXY", []( Lua L )
//{
//    if( L.CallInstanceFunc( &Foo1::SetXY ) )
//    {
//    }
//    else if( auto o = L.ToInstance<Foo1, int>() )
//    {
//        o->SetXY( L.ToInt( -1 ) );
//    }
//    else if( auto o = L.ToInstance<Foo1>() )
//    {
//        o->SetXY();
//    }
//    else {}         // todo: throw exception to lua
//    return 0;
//} )
//.Function( "GetXY", []( Lua L )
//{
//    if( auto o = L.CheckAndGetUpValue<Foo1>() )
//    {
//        L.PushMulti( o->x, o->y );
//        return 2;
//    }
//    return 0;
//} );


//print( Foo1.GetXY() )
//Foo1.x = 0
//Foo1.y = 0    --readonly
//print( Foo1.GetXY() )


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