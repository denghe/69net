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

// http://blog.codingnow.com/2015/05/lua_c_api.html
struct Lua
{
    lua_State* L = nullptr;
    String err;

    Lua()
    {
        L = luaL_newstate();
    }

    ~Lua()
    {
        lua_close( L );
    }

    void ToErrPop()
    {
        size_t len = 0;
        err.Assign( lua_tolstring( L, -1, &len ), (int)len, false );
        lua_pop( L, 1 );
    }

    inline void OpenLibs()
    {
        luaL_openlibs( L );
    }

    inline bool DoFile( String const& fn )
    {
        if( luaL_dofile( L, fn.C_str() ) == LUA_OK ) return true;
        ToErrPop();
        return false;
    }

    inline bool DoString( String const& code )
    {
        if( luaL_dostring( L, code.C_str() ) == LUA_OK ) return true;
        ToErrPop();
        return false;
    }

    inline bool CheckStack( int n )
    {
        return lua_checkstack( L, n ) != 0;
    }

    inline void Push( lua_Number v )
    {
        lua_pushnumber( L, v );
    }
    inline void Push( lua_Integer v )
    {
        lua_pushinteger( L, v );
    }
    inline void Push( String const& v )
    {
        lua_pushlstring( L, v.C_str(), v.Size() );
    }
    inline void Push( char const* v )
    {
        lua_pushstring( L, v );
    }

    inline bool SafePush( lua_Number v )
    {
        if( !CheckStack( 1 ) ) return false;
        Push( v );
        return true;
    }
    inline bool SafePush( lua_Integer v )
    {
        if( !CheckStack( 1 ) ) return false;
        Push( v );
        return true;
    }
    inline bool SafePush( String const& v )
    {
        if( !CheckStack( 1 ) ) return false;
        Push( v );
        return true;
    }
    inline bool SafePush( char const* v )
    {
        if( !CheckStack( 1 ) ) return false;
        Push( v );
        return true;
    }

    template<typename T>
    void PCall_PushParms( T const& p )
    {
        Push( p );
    }
    template<typename T, typename...TS>
    void PCall_PushParms( T const& p0, TS const&...parms )
    {
        Push( p0 );
        PCall_Push( parms... );
    }
    template<typename ...TS>
    int PCall( String const& fn, TS const&...parms )
    {
        auto n = sizeof...( parms );
        if( !CheckStack( n + 1 ) ) return -1;
        int top = GetTop();
        if( lua_getglobal( L, fn.C_str() ) != LUA_TFUNCTION )   // stack +1: 1
        {
            Pop( 1 );                           // stack -1: 0
            return -1;
        }
        if( n ) PCall_PushParms( parms... );                    // stack +n: 1+n
        if( lua_pcall( L, n, LUA_MULTRET, 0 ) != LUA_OK )       // stack -1-n: 0    T: stack +r: r    F: stack +e: e
        {
            ToErrPop();                        // stack -e: 0
            return -1;
        }
        return GetTop() - top;                                  // stack: r
    }

    template<typename ...TS>
    bool PCallPop( String const& fn, std::function<void( int n )> handler, TS const&...parms )
    {
        int rtv = PCall( fn, parms... );
        if( rtv < 0 ) return false;
        handler( rtv );
        Pop( rtv );
        return true;
    }

    inline void Pop( int n )
    {
        lua_pop( L, n );
    }

    //void GetGlobal

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
    inline String ToString( int idx, bool ref = false )
    {
        size_t len;
        auto rtv = lua_tolstring( L, idx, &len );
        return String( rtv, len, len, ref );
    }
    inline lua_Number ToNumber( int idx )
    {
        return lua_tonumber( L, idx );
    }
    inline lua_Integer ToInteger( int idx )
    {
        return lua_tointeger( L, idx );
    }
    inline String ToStringPop( bool ref = false )
    {
        auto rtv = ToString( -1, ref );
        Pop( 1 );
        return rtv;
    }
    inline lua_Number ToNumberPop()
    {
        auto rtv = ToNumber( -1 );
        Pop( 1 );
        return rtv;
    }
    inline lua_Integer ToIntegerPop()
    {
        auto rtv = ToInteger( -1 );
        Pop( 1 );
        return rtv;
    }

};

int main()
{
    Lua L;
    L.OpenLibs();
    CoutLine( "top = ", L.GetTop() );
    L.DoString( R"--(
function xxx( ... )
    print( 1 )
    local t = {...}
    print( 2 )
    table.insert( t, "asdf" )
    table.insert( t, 12 )
    table.insert( t, 34.5 )
    print( 3 )
    return table.unpack( t ) 
end
)--" );
    CoutLine( "top = ", L.GetTop() );
    auto handler = [ &]( int n )
    {
        CoutLine( "n = ", n );
        auto dump = [ &]( int idx )
        {
            switch( L.GetType( idx ) )
            {
            case LuaDataTypes::String:
                CoutLine( L.ToString( idx ) );
                break;
            case LuaDataTypes::Number:
                if( L.IsInteger( idx ) )
                {
                    CoutLine( L.ToInteger( idx ) );
                }
                else
                {
                    CoutLine( L.ToNumber( idx ) );
                }
                break;
            default:
                CoutLine( "{to do}" );
            }
        };
        for( int i = 1; i <= L.GetTop(); ++i )
        {
            Cout( "data type = ", L.GetTypeName( i ), "; value = " );
            dump( i );
        }
    };
    if( !L.PCallPop( "xxx", handler, "p0", (int64)1, "p2", 3.000001 ) )
    {
        CoutLine( "err: ", L.err );
    }
    CoutLine( "top = ", L.GetTop() );

    system( "pause" );
    return 0;
}
