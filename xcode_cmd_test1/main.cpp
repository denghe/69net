#include "Lib/All.h"
#include "Lua/lua.hpp"
#include "LuaEx.h"

//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}

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
    else
    {
        CoutLine( L.err );
    }
    system( "pause" );
    return 0;
}