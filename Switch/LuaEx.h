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



        inline void Push( double v )
        {
            lua_pushnumber( L, v );
        }
        inline void Push( float v )
        {
            lua_pushnumber( L, v );
        }
        inline void Push( int64 v )
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
        inline void Push( void* v )
        {
            lua_pushlightuserdata( L, v );
        }

        inline void PushMulti()
        {
        }

        template<typename T>
        void PushMulti( T const& p0 )
        {
            Push( p0 );
        }
        template<typename T, typename...TS>
        void PushMulti( T const& p0, TS const&...parms )
        {
            Push( p0 );
            PushMulti( parms... );
        }

        inline void PushNil()
        {
            lua_pushnil( L );
        }


        template<typename...TS>
        void PushCClosure( lua_CFunction f, TS const&...parms )
        {
            PushMulti( parms... );
            lua_pushcclosure( L, f, sizeof...( parms ) );
        }

        template<typename T>
        T GetUpValue( int idx = 1 )
        {
            T rtv;
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
        typename std::enable_if<HasParms<TS...>::value, T*>::type CheckAndGetUpValue()
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
        template<typename T, typename...TS>
        typename std::enable_if<!HasParms<TS...>::value, T*>::type CheckAndGetUpValue()
        {
            return GetUpValue<T>();
        }

        template<typename T, typename...TS>
        T* ToInstance()
        {
            return CheckAndGetUpValue<T, TS...>();
        }







        template<typename R, typename T, typename...TS, size_t...I>
        R FuncTupleCaller( T* o, R( T::*f )( TS... ), std::tuple<TS...>& tp, IndexSequence<I...> )
        {
            return ( o->*f )( std::get<I>( tp )... );
        }



        template<typename Tuple, std::size_t N>
        struct TupleFiller
        {
            static void Fill( Lua& L, Tuple& t )
            {
                TupleFiller<Tuple, N - 1>::Fill( L, t );
                L.To( std::get<N - 1>( t ), -(int)( std::tuple_size<Tuple>::value - N + 1 ) );
            }
        };
        template<typename Tuple>
        struct TupleFiller < Tuple, 1 >
        {
            static void Fill( Lua& L, Tuple& t )
            {
                L.To( std::get<0>( t ), -(int)( std::tuple_size<Tuple>::value ) );
            }
        };




        template<typename T>
        bool CallInstanceFunc( int& rc, void( T::*f )( ) )
        {
            auto o = GetUpValue<T*>();
            ( o->*f )( );
            rc = 0;
            return true;
        }
        template<typename T, typename...TS>
        bool CallInstanceFunc( int& rc, void( T::*f )( TS... ) )
        {
            auto top = GetTop();
            if( top != sizeof...( TS ) ) return false;
            if( !Is<TS...>( -top ) ) return false;

            auto o = GetUpValue<T*>();
            std::tuple<TS...> tp;
            TupleFiller<decltype( tp ), sizeof...( TS )>::Fill( *this, tp );
            typedef typename MakeIndexSequence<sizeof...( TS )>::type IST;
            FuncTupleCaller( o, f, tp, IST() );
            rc = 0;
            return true;
        }


        template<typename R, typename T>
        bool CallInstanceFunc( int& rc, R( T::*f )( ) )
        {
            auto o = GetUpValue<T*>();
            Push( ( o->*f )( ) );
            rc = 1;
            return true;
        }
        template<typename R, typename T, typename...TS>
        bool CallInstanceFunc( int& rc, void( T::*f )( TS... ) )
        {
            auto top = GetTop();
            if( top != sizeof...( TS ) ) return false;
            if( !Is<TS...>( -top ) ) return false;

            auto o = GetUpValue<T*>();
            std::tuple<TS...> tp;
            TupleFiller<decltype( tp ), sizeof...( TS )>::Fill( *this, tp );
            typedef typename MakeIndexSequence<sizeof...( TS )>::type IST;
            Push( FuncTupleCaller( o, f, tp, IST() ) );
            rc = 1;
            return true;
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
            return String( rtv, (int)len, (int)len, ref );
        }
        inline double ToDouble( int idx )
        {
            return lua_tonumber( L, idx );
        }
        inline float ToFloat( int idx )
        {
            return (float)lua_tonumber( L, idx );
        }
        inline int64 ToInt64( int idx )
        {
            return lua_tointeger( L, idx );
        }
        inline int ToInt( int idx )
        {
            return (int)lua_tointeger( L, idx );
        }


        inline void To( int64& v, int idx = -1 )
        {
            v = lua_tointeger( L, idx );
        }
        inline void To( int32& v, int idx = -1 )
        {
            v = (int32)lua_tointeger( L, idx );
        }
        inline void To( double& v, int idx = -1 )
        {
            v = lua_tonumber( L, idx );
        }
        inline void To( float& v, int idx = -1 )
        {
            v = (float)lua_tonumber( L, idx );
        }
        inline void To( String& v, int idx = -1 )
        {
            size_t len;
            auto rtv = lua_tolstring( L, idx, &len );
            v.Assign( rtv, (int)len, false );
        }

        // for light user data
        template<typename T>
        inline void To( T*& v, int idx = -1 )
        {
            v = (T*)lua_touserdata( L, idx );
        }


        template<typename T>
        void ToMultiCore( int idx, T& p0 )
        {
            To( p0, idx );
        }
        template<typename T, typename...TS>
        void ToMultiCore( int idx, T& p0, TS&...parms )
        {
            To( p0, idx );
            if( idx < -1 ) ToMultiCore( idx - 1, parms... );
        }

        template<typename...TS>
        void ToMulti( TS&...parms )
        {
            auto n = -( int )sizeof...( parms );
            ToMultiCore( n, parms... );
        }



        inline void PushError( String const& errMsg )
        {
            Push( errMsg );
            lua_error( L );
        }
        inline void PushError( char const* errMsg )
        {
            Push( errMsg );
            lua_error( L );
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
                    CoutLine( ToInt64( idx ) );
                }
                else
                {
                    CoutLine( ToDouble( idx ) );
                }
                break;
            default:
                CoutLine( "{ to do }" );
            }
        }

        inline void DumpMulti( int count )
        {
            auto top = GetTop();
            for( int i = top - count + 1; i <= top; ++i )
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

    template<typename T>
    struct LuaEx_Struct
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

        LuaEx_Struct Field( char const* key, FuncType getter, FuncType setter )
        {
            auto& dict = GetDict();
            dict.Insert( key, std::make_pair( getter, setter ) );
            return *this;
        }
        template<typename FT>
        LuaEx_Struct Field( char const* key, FT T::* fieldOffset, bool readonly = false )
        {
            if( readonly )
            {
                GetDict().Insert( key, std::make_pair(
                    [ fieldOffset ]( Lua& L, T& o )
                {
                    L.Push( o.*fieldOffset );
                }, nullptr ) );
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
        LuaEx_Struct Function( char const* key, CFunction f )
        {
            Field( key, [ f ]( Lua& L, T& o )
            {
                L.PushCClosure( (lua_CFunction)f, &o );
            }, nullptr );
            return *this;
        }


        template<typename R, typename ...PS>
        LuaEx_Struct Function( char const* key, R( T::* f )( PS... ) )
        {
            static List<std::function<void()>> Deleters;
            typedef R( T::* FuncType )( PS... );
            auto fp = new FuncType( f );
            Deleters.Push( [ fp ] { delete fp; } );
            Field( key, [ fp ]( Lua& L, T& o )
            {
                auto cf = []( lua_State* ls )
                {
                    Lua L( ls );
                    FuncType* fp = nullptr;
                    L.GetUpValue( 2, fp );
                    int rtv = 0;
                    if( !L.CallInstanceFunc( rtv, *fp ) )
                    {
                        String err;
                        lua_Debug ar;
                        lua_getstack( ls, 1, &ar );
                        lua_getinfo( ls, "l", &ar );
                        err.Append( "line:", ar.currentline, ": Call function error." );
                        L.PushError( err );
                    }
                    return rtv;
                };
                L.PushCClosure( cf, &o, fp );
            }, nullptr );
            return *this;
        }

    };




    struct LuaEx : public Lua
    {
        bool createrIsMe;
        String err;

        LuaEx( bool openLibBase = true )
            : Lua( luaL_newstate() )
            , createrIsMe( true )
        {
            if( openLibBase ) OpenLibBase();
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
            auto n = ( int )sizeof...( parms );
            if( !CheckStack( n + 1 ) ) return -1;
            int top = GetTop();
            if( lua_getglobal( L, fn.C_str() ) != LUA_TFUNCTION )   // stack +1: 1
            {
                Pop( 1 );                           // stack -1: 0
                err.Clear();
                err.Append( "can't find function by name `", fn, "`" );
                return -1;
            }
            PushMulti( parms... );                                  // stack +n: 1+n
            if( lua_pcall( L, n, LUA_MULTRET, 0 ) != LUA_OK )       // stack -1-n: 0    T: stack +r: r    F: stack +e: e
            {
                ToErrPop();                        // stack -e: 0
                return -1;
            }
            return GetTop() - top;                                  // stack: r
        }

        template<typename ...TS>
        bool CallFunc( String const& fn, std::function<void( int n )> handler = nullptr, TS const&...parms )
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
        inline double ToDoublePop()
        {
            auto rtv = ToDouble( -1 );
            Pop( 1 );
            return rtv;
        }
        inline float ToFloatPop()
        {
            auto rtv = ToFloat( -1 );
            Pop( 1 );
            return rtv;
        }
        inline int64 ToInt64Pop()
        {
            auto rtv = ToInt64( -1 );
            Pop( 1 );
            return rtv;
        }
        inline int ToIntPop()
        {
            auto rtv = ToInt( -1 );
            Pop( 1 );
            return rtv;
        }


        template<typename T>
        static int __Index( lua_State* ls )
        {
            Lua L( ls );                                            // 2            // ud, key
            auto t = *(T**)lua_touserdata( ls, -2 );
            LuaEx_Struct<T>::GetField( L, *t );
            return 1;
        }
        template<typename T>
        static int __NewIndex( lua_State* ls )
        {
            Lua L( ls );                                            // 3            // ud, key，value
            auto t = *(T**)lua_touserdata( ls, -3 );
            LuaEx_Struct<T>::SetField( L, *t );
            return 0;
        }

        template<typename T>
        void SetGlobal( char const* varName, T const& t )
        {
            Push( t );
            lua_setglobal( L, varName );
        }

        template<typename...TS>
        void SetGlobal( char const* varName, lua_CFunction f, TS const&...parms )
        {
            PushCClosure( f, parms... );
            lua_setglobal( L, varName );
        }
        template<typename...TS>
        void SetGlobal( char const* varName, int( *f ) ( Lua L ), TS const&...parms )
        {
            SetGlobal( varName, (lua_CFunction)f, parms... );
        }


        template<typename T>
        void SetGlobal( char const* varName, T* t )
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
        }


        template<typename T>
        LuaEx_Struct<T> Struct()
        {
            return LuaEx_Struct<T>();
        }

    };
}
