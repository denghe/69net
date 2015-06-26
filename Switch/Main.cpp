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
    lua_State* ls;
    bool needClose;
    String err;

    Lua()
        : ls( luaL_newstate() )
        , needClose( true )
    {
    }
    Lua( lua_State* L )
        : ls( L )
        , needClose( false )
    {
    }
    ~Lua()
    {
        if( needClose )
        {
            lua_close( ls );
        }
    }

    void ToErrPop()
    {
        size_t len = 0;
        err.Assign( lua_tolstring( ls, -1, &len ), (int)len, false );
        lua_pop( ls, 1 );
    }

    inline void OpenLibs()
    {
        luaL_openlibs( ls );
    }

    inline bool DoFile( String const& fn )
    {
        if( luaL_dofile( ls, fn.C_str() ) == LUA_OK ) return true;
        ToErrPop();
        return false;
    }

    inline bool DoString( String const& code )
    {
        if( luaL_dostring( ls, code.C_str() ) == LUA_OK ) return true;
        ToErrPop();
        return false;
    }

    inline bool CheckStack( int n )
    {
        return lua_checkstack( ls, n ) != 0;
    }

    inline void Push( lua_Number v )
    {
        lua_pushnumber( ls, v );
    }
    inline void Push( lua_Integer v )
    {
        lua_pushinteger( ls, v );
    }
    inline void Push( int v )
    {
        lua_pushinteger( ls, v );
    }
    inline void Push( String const& v )
    {
        lua_pushlstring( ls, v.C_str(), v.Size() );
    }
    inline void Push( char const* v )
    {
        lua_pushstring( ls, v );
    }
    inline void PushNil()
    {
        lua_pushnil( ls );
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
    inline bool SafePush( int v )
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
    inline bool SafePushNil()
    {
        if( !CheckStack( 1 ) ) return false;
        PushNil();
        return true;
    }

    inline void PCall_PushParms()
    {
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
        PCall_PushParms( parms... );
    }
    template<typename ...TS>
    int PCall( String const& fn, TS const&...parms )
    {
        auto n = sizeof...( parms );
        if( !CheckStack( n + 1 ) ) return -1;
        int top = GetTop();
        if( lua_getglobal( ls, fn.C_str() ) != LUA_TFUNCTION )   // stack +1: 1
        {
            Pop( 1 );                           // stack -1: 0
            return -1;
        }
        PCall_PushParms( parms... );                    // stack +n: 1+n
        if( lua_pcall( ls, n, LUA_MULTRET, 0 ) != LUA_OK )       // stack -1-n: 0    T: stack +r: r    F: stack +e: e
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
        lua_pop( ls, n );
    }

    //void GetGlobal

    inline int GetTop()
    {
        return lua_gettop( ls );
    }
    inline void SetTop( int idx )
    {
        lua_settop( ls, idx );
    }
    inline bool TopIs( int idx )
    {
        return GetTop() == idx;
    }
    inline LuaDataTypes GetType( int idx )
    {
        return map_lua_type_enums[ lua_type( ls, idx ) + 1 ];
    }
    inline const char* GetTypeName( int idx )
    {
        return lua_typename( ls, lua_type( ls, idx ) );
    }
    inline bool IsInteger( int idx )
    {
        return lua_isinteger( ls, idx ) == 1;
    }
    inline String ToString( int idx, bool ref = false )
    {
        size_t len;
        auto rtv = lua_tolstring( ls, idx, &len );
        return String( rtv, len, len, ref );
    }
    inline lua_Number ToNumber( int idx )
    {
        return lua_tonumber( ls, idx );
    }
    inline lua_Integer ToInteger( int idx )
    {
        return lua_tointeger( ls, idx );
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


    template<typename T>
    static int LuaFunc_Index( lua_State* ls )
    {
        assert( lua_gettop( ls ) == 2 );                        // 2            // metatable, key
        lua_getupvalue( ls, -lua_gettop( ls ) - 1, 1 );         // +1   3       // get upvalue 1: foo*
        auto ud = (T*)lua_touserdata( ls, -1 );
        lua_pop( ls, 1 );                                       // -1   2       // remove upvalue
        ud->GetField( ls );
        return 1;
    }
    template<typename T>
    static int LuaFunc_NewIndex( lua_State* ls )
    {
        assert( lua_gettop( ls ) == 3 );                        // 3            // metatable, key£¬ value
        lua_getupvalue( ls, -lua_gettop( ls ) - 1, 1 );         // +1   4       // get upvalue 1: foo*
        auto ud = (T*)lua_touserdata( ls, -1 );
        lua_pop( ls, 1 );                                       // -1   3       // remove upvalue
        ud->SetField( ls );
        return 0;
    }

    template<typename T>
    void Register( T* foo, char const* typeName )
    {
        luaL_Reg reg[] =
        {
            { "__index", LuaFunc_Index < T > },
            { "__newindex", LuaFunc_NewIndex < T > },
            { nullptr, nullptr }
        };
        lua_newtable( ls );                                     // +1   1       // global Foo table
        luaL_newlibtable( ls, reg );                            // +1   2       // metatable
        lua_pushlightuserdata( ls, foo );                       // +1   3       // upvalue
        luaL_setfuncs( ls, reg, 1 );                            // -1   2       // bind funcs + upvalue to metatable
        lua_setmetatable( ls, -2 );                             // -1   1       // bind metatable to Foo table
        lua_setglobal( ls, typeName );                          // -1   0       // set Foo table to global
    }

    void Dump( int idx )
    {
        Cout( "data type = ", GetTypeName( idx ), "; value = " );
        switch( GetType( idx ) )
        {
        case LuaDataTypes::String:
            CoutLine( ToString( idx ) );
            break;
        case LuaDataTypes::Number:
            if( IsInteger( idx ) )
            {
                CoutLine( ToInteger( idx ) );
            }
            else
            {
                CoutLine( ToNumber( idx ) );
            }
            break;
        default:
            CoutLine( "{ to do }" );
        }
    }

    void DumpMulti( int n )
    {
        int top = GetTop();
        for( int i = top - n + 1; i <= top; ++i )
        {
            Dump( i );
        }
    }

    void DumpTop()
    {
        auto top = lua_gettop( ls );
        Cout( "top = ", top, " " );
        if( top ) Dump( -1 );
        else CoutLine();
    };
};




struct Foo
{
    int x = 0;
    int y = 0;

    inline void GetField( lua_State* ls ) const
    {
        auto key = lua_tostring( ls, -1 );
        if( strcmp( key, "x" ) == 0 )
        {
            lua_pushinteger( ls, x );
        }
        else if( strcmp( key, "y" ) == 0 )
        {
            lua_pushinteger( ls, y );
        }
        else
        {
            lua_pushnil( ls );
        }
    }
    inline void SetField( lua_State* ls )
    {
        auto key = lua_tostring( ls, -2 );
        if( strcmp( key, "x" ) == 0 )
        {
            x = (int)lua_tointeger( ls, -1 );
        }
        if( strcmp( key, "y" ) == 0 )
        {
            y = (int)lua_tointeger( ls, -1 );
        }
    }
};


int main()
{
    Lua L;
    L.OpenLibs();
    L.DumpTop();

    Foo f;
    L.Register( &f, "Foo" );
    L.DumpTop();

    L.DoString( R"--(

function f1( x, y )
    Foo.x = x
    Foo.y = y
end

function f2()
    return Foo.x, Foo.y;
end

)--" );
    L.DumpTop();

    L.PCall( "f1", 12, 34 );
    L.DumpTop();
    if( !L.PCallPop( "f2", [ &]( int n ) { L.DumpMulti( n ); } ) )
    {
        CoutLine( "err: ", L.err );
    }
    L.DumpTop();

    system( "pause" );
    return 0;
}

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
