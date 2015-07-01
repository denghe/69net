#pragma once
#include "lua.hpp"

namespace xxx
{
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
        lua_State* L;
        Lua( lua_State* L ) : L( L ) {}

        inline void OpenLibs()
        {
            luaL_openlibs( L );
        }
        inline void OpenLibBase()
        {
            luaopen_base( L );
        }
        inline bool DoFile( String const& fn )
        {
            return luaL_dofile( L, fn.C_str() ) == LUA_OK;
        }
        inline bool CheckStack( int n )
        {
            return lua_checkstack( L, n ) != 0;
        }
        inline bool DoString( String const& code )
        {
            return luaL_dostring( L, code.C_str() ) == LUA_OK;
        }
        inline void Push( lua_Number v )
        {
            lua_pushnumber( L, v );
        }
        inline void Push( lua_Integer v )
        {
            lua_pushinteger( L, v );
        }
        inline void Push( int v )
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
        inline void Push( std::nullptr_t v )
        {
            lua_pushnil( L );
        }

        // for light user data
        template<typename T>
        inline void Push( T* v )
        {
            lua_pushlightuserdata( L, v );
        }

        inline void Push() {}

        template<typename T, typename...TS>
        void Push( T const& p0, TS const&...parms )
        {
            Push( p0 );
            Push( parms... );
        }

        inline void PushNil()
        {
            lua_pushnil( L );
        }


        template<typename...TS>
        void PushCClosure( lua_CFunction f, TS const&...parms )
        {
            Push( parms... );
            lua_pushcclosure( L, f, sizeof...( parms ) );
        }

        template<typename T>
        T* GetUpValue( int idx = 1 )
        {
            T* rtv;
            lua_getupvalue( L, -lua_gettop( L ) - 1, idx );
            To( rtv );
            lua_pop( L, 1 );
            return rtv;
        }

        template<typename T>
        void GetUpValue( int idx, T& v )
        {
            lua_getupvalue( L, -lua_gettop( L ) - 1, idx );
            To( v );
            lua_pop( L, 1 );
        }

        template<typename T, typename...TS>
        void GetUpValue( int idx, T& p0, TS&...parms )
        {
            GetUpValue( idx, p0 );
            GetUpValue( idx + 1, parms... );
        }



        inline bool IsNumber( int idx )
        {
            return lua_isnumber( L, idx ) == 1;
        }
        inline bool IsInteger( int idx )
        {
            return lua_isinteger( L, idx ) == 1;
        }
        inline bool IsString( int idx )
        {
            return lua_isstring( L, idx ) == 1;
        }
        inline bool IsNil( int idx )
        {
            return lua_isnil( L, idx ) == 1;
        }

        template<typename T, typename...TS>
        bool Is( int idx )
        {
            if( !Is<T>( idx ) ) return false;
            return Is<TS...>( idx + 1 );
        }
        template<>
        bool Is<int>( int idx )
        {
            return IsInteger( idx );
        }
        template<>
        bool Is<int64>( int idx )
        {
            return IsInteger( idx );
        }
        template<>
        bool Is<float>( int idx )
        {
            return IsNumber( idx );
        }
        template<>
        bool Is<double>( int idx )
        {
            return IsNumber( idx );
        }
        template<>
        bool Is<String>( int idx )
        {
            return IsString( idx );
        }

        template<typename T, typename...TS>
        T* CheckAndGetUpValue()
        {
            auto top = GetTop();
            if( top != sizeof...( TS ) )
            {
                return nullptr;
            }
            if( !Is<TS...>( -top ) )
            {
                return nullptr;
            }
            return GetUpValue<T>();
        }


        inline void Pop( int n )
        {
            lua_pop( L, n );
        }
        inline int GetTop()
        {
            return lua_gettop( L );
        }
        inline void SetTop( int idx )
        {
            lua_settop( L, idx );
        }

        inline bool TopIs( int idx )
        {
            return GetTop() == idx;
        }
        inline LuaDataTypes GetType( int idx )
        {
            return map_lua_type_enums[ lua_type( L, idx ) + 1 ];
        }
        inline const char* GetTypeName( int idx )
        {
            return lua_typename( L, lua_type( L, idx ) );
        }



        inline char const* ToCString( int idx )
        {
            return lua_tostring( L, idx );
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
        inline int ToInt( int idx )
        {
            return (int)lua_tointeger( L, idx );
        }

        inline void To( int32& v )
        {
            v = (int32)lua_tointeger( L, -1 );
        }
        inline void To( int64& v )
        {
            v = lua_tointeger( L, -1 );
        }
        inline void To( double& v )
        {
            v = lua_tonumber( L, -1 );
        }
        inline void To( String& v )
        {
            size_t len;
            auto rtv = lua_tolstring( L, -1, &len );
            v.Assign( rtv, len, false );
        }

        // for light user data
        template<typename T>
        inline void To( T*& v )
        {
            v = (T*)lua_touserdata( L, -1 );
        }

        template<typename T, typename...TS>
        void To( T& p0, TS&...parms )
        {
            To( p0 );
            To( parms... );
        }

    };

    template<typename T>
    struct LuaStruct
    {
        typedef std::function<void( Lua&, T& )> FuncType;
        static Dict < String, std::pair<FuncType, FuncType>>& GetDict()
        {
            static Dict < String, std::pair<FuncType, FuncType>> dict;
            return dict;
        }
        static void GetField( Lua& L, T& o )
        {
            auto& dict = GetDict();
            auto key = L.ToString( -1, true );
            if( auto node = dict.Find( key ) )
            {
                if( node->value.first )
                {
                    node->value.first( L, o );
                    return;
                }
            }
            L.PushNil();
        }
        static void SetField( Lua& L, T& o )
        {
            auto& dict = GetDict();
            auto key = L.ToString( -2, true );
            if( auto node = dict.Find( key ) )
            {
                if( node->value.second )
                {
                    node->value.second( L, o );
                    return;
                }
            }
            L.PushNil();
        }

        LuaStruct Field( char const* key, FuncType getter, FuncType setter )
        {
            auto& dict = GetDict();
            dict.Insert( key, std::make_pair( getter, setter ) );
            return *this;
        }
        template<typename FT>
        LuaStruct Field( char const* key, FT T::* fieldOffset, bool readonly = false )
        {
            if( readonly )
            {
                GetDict().Insert( key, std::make_pair(
                    [ fieldOffset ]( Lua& L, T& o )
                {
                    L.Push( o.*fieldOffset );
                }, nullptr ));
            }
            else
            {
                GetDict().Insert( key, std::make_pair(
                    [ fieldOffset ]( Lua& L, T& o )
                {
                    L.Push( o.*fieldOffset );
                }, [ fieldOffset ]( Lua& L, T& o )
                {
                    L.To( o.*fieldOffset );
                } ) );
            }
            return *this;
        }

        typedef int( *CFunction ) ( Lua L );
        LuaStruct Function( char const* key, CFunction f )
        {
            Field( key, [f]( Lua& L, Foo1& o )
            {
                L.PushCClosure( (lua_CFunction)f, &o );
            }, nullptr );
            return *this;
        }
    };


    struct LuaEx : public Lua
    {
        bool createrIsMe;
        String err;

        LuaEx()
            : Lua( luaL_newstate() )
            , createrIsMe( true )
        {
        }
        LuaEx( lua_State* L )
            : Lua( L )
            , createrIsMe( false )
        {
        }
        ~LuaEx()
        {
            if( createrIsMe )
            {
                lua_close( L );
            }
        }

        LuaEx( LuaEx const& o ) = delete;
        LuaEx& operator=( LuaEx const& o ) = delete;

        inline void ToErrPop()
        {
            size_t len = 0;
            err.Assign( lua_tolstring( L, -1, &len ), (int)len, false );
            lua_pop( L, 1 );
        }

        inline bool DoFile( String const& fn )
        {
            if( Lua::DoFile( fn ) ) return true;
            ToErrPop();
            return false;
        }

        inline bool DoString( String const& code )
        {
            if( Lua::DoString( code ) ) return true;
            ToErrPop();
            return false;
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

        template<typename ...TS>
        int Pcall( String const& fn, TS const&...parms )
        {
            auto n = sizeof...( parms );
            if( !CheckStack( n + 1 ) ) return -1;
            int top = GetTop();
            if( lua_getglobal( L, fn.C_str() ) != LUA_TFUNCTION )   // stack +1: 1
            {
                Pop( 1 );                           // stack -1: 0
                err.Clear();
                err.Append( "can't find function by name `", fn, "`" );
                return -1;
            }
            Push( parms... );                                       // stack +n: 1+n
            if( lua_pcall( L, n, LUA_MULTRET, 0 ) != LUA_OK )       // stack -1-n: 0    T: stack +r: r    F: stack +e: e
            {
                ToErrPop();                        // stack -e: 0
                return -1;
            }
            return GetTop() - top;                                  // stack: r
        }

        template<typename ...TS>
        bool PcallPop( String const& fn, std::function<void( int n )> handler, TS const&...parms )
        {
            int rtv = Pcall( fn, parms... );
            if( rtv < 0 ) return false;
            if( handler ) handler( rtv );
            Pop( rtv );
            return true;
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
            Lua L( ls );                                            // 2            // ud, key
            auto t = *(T**)lua_touserdata( ls, -2 );
            LuaStruct<T>::GetField( L, *t );
            return 1;
        }
        template<typename T>
        static int __NewIndex( lua_State* ls )
        {
            Lua L( ls );                                            // 3            // ud, key£¬value
            auto t = *(T**)lua_touserdata( ls, -3 );
            LuaStruct<T>::SetField( L, *t );
            return 0;
        }
        template<typename T>
        LuaStruct<T> Struct( T* t, char const* varName )
        {
            static luaL_Reg reg[] =
            {
                // __gc
                { "__index", __Index < T > },
                { "__newindex", __NewIndex < T > },
                { nullptr, nullptr }
            };
            *(T**)lua_newuserdata( L, sizeof( T* ) ) = t;           // +1   1       // ud for store t
            if( luaL_newmetatable( L, typeid( std::decay<T>::type ).name() ) )         // +1   2
            {
                luaL_setfuncs( L, reg, 0 );                         // -0   2       // bind funcs to metatable
            }
            lua_setmetatable( L, -2 );                              // -1   1       // bind metatable to ud
            lua_setglobal( L, varName );                            // -1   0       // set ud to global
            return LuaStruct<T>();
        }





        inline void Dump( int idx )
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

        inline void DumpMulti( int n )
        {
            auto top = GetTop();
            for( int i = top - n + 1; i <= top; ++i )
            {
                Dump( i );
            }
        }

        inline void DumpTop()
        {
            auto top = GetTop();
            Cout( "top = ", top, " " );
            if( top ) Dump( -1 );
            else CoutLine();
        };
    };


}