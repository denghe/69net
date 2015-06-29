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
        if( handler ) handler( rtv );
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
    inline char const* ToCString( int idx )
    {
        return lua_tostring( ls, idx );
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
    inline char const* ToCStringPop()
    {
        auto rtv = ToCString( -1 );
        Pop( 1 );
        return rtv;
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
    static int __Index( lua_State* ls )
    {
        assert( lua_gettop( ls ) == 2 );                        // 2            // ud, key
        auto t = *(T**)lua_touserdata( ls, -2 );
        LuaGetterSetter<T>::GetField( ls, t );
        return 1;
    }
    template<typename T>
    static int __NewIndex( lua_State* ls )
    {
        assert( lua_gettop( ls ) == 3 );                        // 3            // ud, key£¬value
        auto t = *(T**)lua_touserdata( ls, -3 );
        LuaGetterSetter<T>::SetField( ls, t );
        return 0;
    }

    template<typename T>
    void Register( T* t, char const* typeName )
    {
        if( !LuaGetterSetter<T>::GetDict().Size() )
        {
            LuaGetterSetter<T>::Init();
        }
        static luaL_Reg reg[] =
        {
            // __gc
            { "__index", __Index < T > },
            { "__newindex", __NewIndex < T > },
            { nullptr, nullptr }
        };
        *(T**)lua_newuserdata( ls, sizeof( T* ) ) = t;          // +1   1       // ud for store t
        if( luaL_newmetatable( ls, typeName ) )                 // +1   2
        {
            luaL_setfuncs( ls, reg, 0 );                        // -0   2       // bind funcs to metatable
        }
        lua_setmetatable( ls, -2 );                             // -1   1       // bind metatable to ud
        lua_setglobal( ls, typeName );                          // -1   0       // set ud to global
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

template<typename T>
struct LuaGetterSetter {};

template<typename T>
struct LuaGetterSetterBase
{
    typedef void( *FuncType )( lua_State*, T* );
    static Dict < String, std::pair<FuncType, FuncType>>& GetDict()
    {
        static Dict < String, std::pair<FuncType, FuncType>> dict;
        return dict;
    }
    static void GetField( lua_State* ls, T* o )
    {
        auto& dict = GetDict();
        size_t len;
        auto p = lua_tolstring( ls, -1, &len );
        String key( p, len, len, true );
        auto node = dict.Find( key );
        if( node )
        {
            dict[ key ].first( ls, o );
        }
        else
        {
            lua_pushnil( ls );
        }
    }
    static void SetField( lua_State* ls, T* o )
    {
        auto& dict = GetDict();
        size_t len;
        auto p = lua_tolstring( ls, -2, &len );
        String key( p, len, len, true );
        auto node = dict.Find( key );
        if( node )
        {
            dict[ key ].second( ls, o );
        }
    }
    static void Register( char const* key, FuncType getter, FuncType setter )
    {
        auto& dict = GetDict();
        dict.Insert( key, std::make_pair( getter, setter ) );
    }
};

struct Foo1
{
    int x = 0;
    int y = 0;
};
struct Foo2
{
    String x;
};

template<>
struct LuaGetterSetter < Foo1 > : LuaGetterSetterBase < Foo1 >
{
    inline static void Init()
    {
        Register( "x", []( lua_State* ls, Foo1* o ) { lua_pushinteger( ls, o->x ); }, []( lua_State* ls, Foo1* o ) { o->x = (int)lua_tointeger( ls, -1 ); } );
        Register( "y", []( lua_State* ls, Foo1* o ) { lua_pushinteger( ls, o->y ); }, []( lua_State* ls, Foo1* o ) { o->y = (int)lua_tointeger( ls, -1 ); } );
    }
};

template<>
struct LuaGetterSetter < Foo2 > : LuaGetterSetterBase < Foo2 >
{
    inline static void GetX( lua_State* ls, Foo2* o )
    {
        lua_pushstring( ls, o->x.C_str() );
    }
    inline static void SetX( lua_State* ls, Foo2* o )
    {
        size_t len;
        auto s = lua_tolstring( ls, -1, &len );
        o->x.Assign( s, len, false );
    }
    inline static void Init()
    {
        Register( "x", GetX, SetX );
    }
};

int main()
{
    Lua L;
    L.OpenLibs();

    Foo1 f1; Foo2 f2;
    L.Register( &f1, "Foo1" );
    L.Register( &f2, "Foo2" );

    printf( "dict = %p\n", &LuaGetterSetter<Foo1>::GetDict() );
    printf( "dict = %p\n", &LuaGetterSetter<Foo2>::GetDict() );

    L.DoString( R"--(

function f1( x, y, name )
    Foo1.x = x
    Foo1.y = y
    Foo2.x = name
end

function f2()
    return Foo1.x, Foo1.y, Foo2.x
end

)--" );

    if( !L.PCallPop( "f1", nullptr, 12, 34, "asdfqwer" ) )
    {
        CoutLine( "err: ", L.err );
    }
    if( !L.PCallPop( "f2", [ &]( int n ) { L.DumpMulti( n ); } ) )
    {
        CoutLine( "err: ", L.err );
    }

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
