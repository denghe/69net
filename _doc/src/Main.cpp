#include "Precompile.h"
#include <vld.h>

using namespace std;

template<typename T>
struct Node : public enable_shared_from_this < Node<T> >
{
    T Data;
    weak_ptr<Node> Parent;
    vector<shared_ptr<Node>> Childs;

    void AddChild( shared_ptr<Node> c )
    {
        auto p = shared_from_this();
        assert( p != c );
        c->RemoveFromParent();
        c->Parent = p;
        Childs.push_back( c );
    }
    void RemoveChild( shared_ptr<Node>& c )
    {
        auto it = find( Childs.begin(), Childs.end(), c );
        if( it != Childs.end() )
        {
            Childs.erase( it );
        }
        c->Parent.reset();
    }
    void RemoveFromParent()
    {
        if( auto p = Parent.lock() )
        {
            p->RemoveChild( shared_from_this() );
        }
    }

    static shared_ptr<Node> Create( shared_ptr<Node> parent = nullptr )
    {
        auto rtv = shared_ptr<Node>( new Node() );
        rtv->Parent = parent;
        return rtv;
    }

    //template<typename CT>
    //void AddChild( CT && c )
    //{
    //    c->RemoveFromParent();
    //    c->Parent = shared_from_this();
    //    Childs.push_back( forward<CT>( c ) );
    //}

    template<typename CT, typename ... CTS>
    void AddChild( CT && c, CTS && ...childs )
    {
        AddChild( forward<CT>( c ) );
        AddChild( forward<CTS>( childs )... );
    }

    template<typename... CTS>
    static shared_ptr<Node> Create( shared_ptr<Node> parent, CTS&&... args )
    {
        auto rtv = shared_ptr<Node>( new Node() );
        if( parent )
        {
            rtv->Parent = parent;
            parent->Childs.push_back( rtv );
        }
        rtv->AddChild( forward<CTS>( args )... );
        return rtv;
    }
};

struct Foo
{
    string Name;
    int Age;
    Foo() : Age( 0 ) {}
};
typedef Node<Foo> FooNode;

void Dump( FooNode )
{

}

int main()
{
    auto r1 = FooNode::Create();
    auto r2 = FooNode::Create( r1, FooNode::Create(), FooNode::Create() );
    r1->AddChild( r2->Childs[ 1 ] );
    r1->Childs[ 1 ]->Data.Name = "asdf";
    r2->Childs[ 0 ]->Data.Name = "qwer";
    return 0;
}




//#include <string>
//#include <iostream>
//#include <vector>
//#include <cassert>
//#include <unordered_map>
//#include <memory>
//using namespace std;
//
//class Foo_t;
//typedef shared_ptr<Foo_t> Foo;
//
//class Bar_t;
//typedef shared_ptr<Bar_t> Bar;
//
//class Foo_t
//{
//public:
//    string Name;
//    int Level;
//    vector<Bar> Childs;
//
//    static Foo create( string const & name, int level )
//    {
//        Foo f( new Foo_t(), []( Foo_t * p )
//        {
//            delete p;
//        } );
//        f->Name = name;
//        f->Level = level;
//        // bars
//        return f;
//    }
//};
//
//class Bar_t
//{
//public:
//    string Name;
//    weak_ptr<Foo_t> Parent;
//
//    static Bar create( string const & name, Foo& parent )
//    {
//        Bar b( new Bar_t, []( Bar_t * p )
//        {
//            delete p;
//        } );
//        b->Name = name;
//        b->Parent = parent;
//        return b;
//    }
//};
//
//unordered_map<string, Foo> _foos =
//{
//    { "asdf", Foo_t::create( "asdf", 123 ) },
//    { "qwer", Foo_t::create( "qwer", 55 ) },
//    { "zxcv", Foo_t::create( "zxcv", 0 ) },
//};
//
//Foo FindFoo( string const& name )
//{
//    Foo f;
//    auto it = _foos.find( name );
//    if( it == _foos.end() ) return f;
//    f = it->second;
//    return f;
//}
//
//
//int main()
//{
//    {
//        auto f = FindFoo( "qwer" );
//        if( !f ) cout << "not found" << endl;
//        else
//        {
//            cout << f->Level << endl;
//            f->Childs.push_back( Bar_t::create( "b1", f ) );
//            f->Childs.push_back( Bar_t::create( "b2", f ) );
//        }
//
//        f = FindFoo( "eeeeeeee" );
//        if( !f )cout << "not found" << endl;
//        else cout << f->Level << endl;
//    }
//    return 0;
//}





//#include <string>
//#include <iostream>
//#include <vector>
//#include <cassert>
//#include <unordered_map>
//#include <functional>
//
//using namespace std;
//
//#define DATA_INNER_REF_H( T )                       \
//    friend T;                                       \
//private:                                            \
//    int _refCount = 1;                              \
//public:
//
//
//#define DATA_OUTER_REF_H( T )                       \
//private:                                            \
//    Data * _data = nullptr;                         \
//    function<void()> _dispose;                      \
//public:                                             \
//    T();                                            \
//    ~T();                                           \
//    T( T const & other );                           \
//    T& operator=( T const & other );                \
//    void Init();                                    \
//    bool IsNull();                                  \
//    void setNull();
//
//
//#define DATA_OUTER_REF_CPP( T )                     \
//T::T()                                              \
//{                                                   \
//}                                                   \
//T::~T()                                             \
//{                                                   \
//    if( _data )                                     \
//    {                                               \
//        if( _dispose ) _dispose();                  \
//        _data->_refCount--;                         \
//        if( _data->_refCount <= 0 )                 \
//        {                                           \
//            delete _data;                           \
//        }                                           \
//    }                                               \
//}                                                   \
//T::T( T const & other )                             \
//{                                                   \
//    if( other._data )                               \
//    {                                               \
//        ++other._data->_refCount;                   \
//    }                                               \
//    _data = other._data;                            \
//}                                                   \
//T& T::operator=( T const & other )                  \
//{                                                   \
//    if( _data != other._data )                      \
//    {                                               \
//        if( other._data )                           \
//        {                                           \
//            ++other._data->_refCount;               \
//        }                                           \
//        if( _data )                                 \
//        {                                           \
//            --_data->_refCount;                     \
//            if( (_data->_refCount) <= 0 )           \
//            {                                       \
//                delete _data;                       \
//                cout << "del" << endl;              \
//            }                                       \
//        }                                           \
//        _data = other._data;                        \
//    }                                               \
//    return *this;                                   \
//}                                                   \
//void T::Init()                                      \
//{                                                   \
//    assert( !_data );                               \
//    _data = new Data();                             \
//}                                                   \
//bool T::IsNull()                                    \
//{                                                   \
//    return _data == nullptr;                        \
//}                                                   \
//void T::setNull()                                   \
//{                                                   \
//    if( _data )                                     \
//    {                                               \
//        --_data->_refCount;                         \
//        if( (_data->_refCount) <= 0 )               \
//        {                                           \
//            delete _data;                           \
//        }                                           \
//        _data = nullptr;                            \
//    }                                               \
//}
//
//
//#define DATA_OUTER_PROP_H( T, N )                   \
//inline T & N()                                      \
//{                                                   \
//    assert( _data );                                \
//    return _data->N;                                \
//}                                                   \
//inline T const& N() const                           \
//{                                                   \
//    assert( _data );                                \
//    return _data->N;                                \
//}                                                   \
//inline T const& get##N() const                      \
//{                                                   \
//    assert( _data );                                \
//    return _data->N;                                \
//}                                                   \
//template<typename NT>                               \
//void set##N( NT && v )                              \
//{                                                   \
//    assert( _data );                                \
//    _data->N = forward<NT>( v );                    \
//}
//
//
//struct Bar;
//struct Foo
//{
//    struct Data
//    {
//        string Name;
//        int Level = 0;
//        vector<Bar> Childs;
//
//        DATA_INNER_REF_H( Foo );
//    };
//    DATA_OUTER_REF_H( Foo );
//
//    DATA_OUTER_PROP_H( string, Name );
//    DATA_OUTER_PROP_H( int, Level );
//    DATA_OUTER_PROP_H( vector<Bar>, Childs );
//
//    Foo( string const& name, int level );
//    
//};
//
//
//struct Bar
//{
//    struct Data
//    {
//        string Name;
//        Foo Parent;
//
//        DATA_INNER_REF_H( Bar );
//    };
//    DATA_OUTER_REF_H( Bar );
//
//    DATA_OUTER_PROP_H( string, Name );
//    DATA_OUTER_PROP_H( Foo, Parent );
//
//    Bar( string const& name, Foo parent );
//};
//
//
//DATA_OUTER_REF_CPP( Foo );
//Foo::Foo( string const& name, int level )
//{
//    Init();
//    Name() = name;
//    Level() = level;
//
//    _dispose = [this] 
//    {
//        Childs().clear();
//    };
//}
//
//DATA_OUTER_REF_CPP( Bar );
//Bar::Bar( string const& name, Foo parent )
//{
//    Init();
//    Name() = name;
//    Parent() = parent;
//
//    _dispose = [ this ]
//    {
//        Parent().setNull();
//    };
//}
//
//
//unordered_map<string, Foo> _foos =
//{
//    { "asdf", Foo( "asdf", 123 ) },
//    { "qwer", Foo( "qwer", 55 ) },
//    { "zxcv", Foo( "zxcv", 0 ) },
//};
//
//Foo FindFoo( string const& name )
//{
//    Foo f;
//    auto it = _foos.find( name );
//    if( it == _foos.end() ) return f;
//    f = it->second;
//    return f;
//}
//
//int main()
//{
//    auto f = FindFoo( "qwer" );
//    if( f.IsNull() )cout << "not found" << endl;
//    else
//    {
//        cout << f.Level() << endl;
//        f.Childs().push_back( Bar( "b1", f ) );
//        f.Childs().push_back( Bar( "b2", f ) );
//    }
//
//    f = FindFoo( "eeeeeeee" );
//    if( f.IsNull() )cout << "not found" << endl;
//    else  cout << f.Level() << endl;
//
//    return 0;
//
//    //{
//    //    Foo f1;
//    //    f1.Init();
//    //    f1.Name() = "asdf";
//    //    f1.Level() = 123;
//    //    Foo f2;
//    //    f2 = f1;
//    //    Foo f3 = f1;
//    //    f3 = f2;
//    //    cout << f1.Name() << f1.Level();
//    //    cout << f2.Name() << f2.Level();
//    //    cout << endl;
//    //}
//}






//void output( char* s, String* ss, int len )
//{
//    String idxStr;
//    int idx;
//    bool isGetingIndex = false;
//    while( *s )
//    {
//        if( isGetingIndex )
//        {
//            if( *s == '}' )
//            {
//                ++s;
//                Converts::fromString( idx, idxStr.c_str( ) );
//                assert( idx < len );    // out of range: param index
//                idxStr.clear( );
//                Cout << ss[ idx ];
//                isGetingIndex = false;
//                continue;
//            }
//            idxStr << *s++;
//        }
//        else
//        {
//            if( *s == '{' )
//            {
//                ++s;
//                if( *s != '{' )
//                {
//                    isGetingIndex = true;
//                    continue;
//                }
//            }
//            Cout << *s++;
//        }
//    }
//}
//
//template<typename T>
//void fill( String* ss, int idx, T && v )
//{
//    ss[ idx ].write( std::forward<T>( v ) );
//}
//
//template<typename T, typename ... TS>
//void fill( String* ss, int idx, T && v, TS && ... vs )
//{
//    ss[ idx ].write( std::forward<T>( v ) );
//    fill( ss, ++idx, std::forward<TS>( vs )... );
//}
//
//template<typename... TS>
//void print( char* s, TS... vs )
//{
//    String ss[ sizeof...( vs ) ];
//    fill( ss, 0, std::forward<TS>( vs )... );
//    output( s, ss, sizeof...( vs ) );
//}
//
//int main()
//{
//    print( "{1}{0}{0} {2}{3}{2} {0}{0}{1} ", 1, "<>", 3.45, L"汉字" );
//    return 0;
//}
