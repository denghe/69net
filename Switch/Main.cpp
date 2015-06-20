#include "Lib/All.h"

using namespace std;
using namespace xxx;

#include "lua.hpp"
enum class LuaDataTypes
{
    NoValue, Nil, Boolean, UserData, Number, String, Table, Function, Thread, Proto
};
const LuaDataTypes map_lua_type_enums[] = {
    LuaDataTypes::NoValue,
    LuaDataTypes::Nil, LuaDataTypes::Boolean, LuaDataTypes::UserData, LuaDataTypes::Number,
    LuaDataTypes::String, LuaDataTypes::Table, LuaDataTypes::Function, LuaDataTypes::UserData, LuaDataTypes::Thread,
    LuaDataTypes::Proto
};
struct Lua
{
    lua_State* L = nullptr;
    String errMsg;
    Lua() { L = luaL_newstate(); }
    ~Lua() { lua_close( L ); }
    inline bool LoadString( String const& code )
    {
        auto rtv = luaL_loadstring( L, code.C_str() );
        // todo if rtv errMsg = ....
        return rtv == 0;
    }
    inline bool PCall()
    {
        auto rtv = lua_pcall( L, 0, LUA_MULTRET, 0 );
        // todo if rtv errMsg = ....
        return rtv == 0;
    }
    inline int GetTop()
    {
        return lua_gettop( L );
    }
    inline void SetTop( int idx )
    {
        lua_settop( L, idx );
    }
    inline LuaDataTypes GetType( int idx )
    {
        return map_lua_type_enums[ lua_type( L, idx ) + 1 ];
    }
    inline const char* GetTypeName( int idx )
    {
        return lua_typename( L, lua_type( L, idx ) );
    }
    inline bool IsInteger( int idx )
    {
        return lua_isinteger( L, idx ) == 1;
    }
    inline String GetString( int idx, bool ref = false )
    {
        size_t len;
        auto rtv = lua_tolstring( L, idx, &len );
        return String( rtv, len, len, ref );
    }
    inline lua_Number GetNumber( int idx )
    {
        return lua_tonumber( L, idx );
    }
    inline lua_Integer GetInteger( int idx )
    {
        return lua_tointeger( L, idx );
    }

    // todo: http://blog.codingnow.com/2015/05/lua_c_api.html
};

int main()
{
    Lua L;
    L.LoadString( R"--(
    return "asdf", 12, 34.5
)--" );
    L.PCall();
    auto dump = [ &]( int idx )
    {
        switch( L.GetType( idx ) )
        {
        case LuaDataTypes::String:
            CoutLine( L.GetString( idx ) );
            break;
        case LuaDataTypes::Number:
            if( L.IsInteger( idx ) )
            {
                CoutLine( L.GetInteger( idx ) );
            }
            else
            {
                CoutLine( L.GetNumber( idx ) );
            }
        }
    };
    for( int i = 1; i <= L.GetTop(); ++i )
    {
        Cout( "data type = ", L.GetTypeName( i ), "; value = " );
        dump( i );
    }
    L.SetTop( 0 );

    system( "pause" );
    return 0;
}
