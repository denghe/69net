#include "Lib/All.h"
using namespace std;
using namespace xxx;


template<typename T, typename... Args>
struct MaxSize
{
    enum
    {
        value = sizeof( T ) > MaxSize<Args...>::value
        ? sizeof( T )
        : MaxSize<Args...>::value
    };
};
template<typename T>
struct MaxSize < T >
{
    enum { value = sizeof( T ) };
};


struct Foo;

struct FooStateBase : public CorCore
{
    Foo* owner;
};
struct FooState1 : public FooStateBase
{
    String name;
    void Init( Foo* _owner, String const& _name /* , ... */ );
    bool Process( int _ticks ) override;
    void Destroy() override;
};
struct FooState2 : public FooStateBase
{
    String name;
    void Init( Foo* _owner, String const& _name /* , ... */ );
    bool Process( int _ticks ) override;
    void Destroy() override;
};
struct Foo : public CorBase < Foo >
{
    std::aligned_storage_t < MaxSize<
        FooState1, FooState2 /* , ... */
    >::value, 8 > stateData[ 1 ];
    FooStateBase* state = (FooStateBase*)stateData;
    String name;
    void Init( String const& _name );
    bool Process( int _ticks ) override;
};



void FooState1::Init( Foo* _owner, String const& _name )
{
    new ( this ) FooState1();
    owner = _owner;
    // ...
    name = _name;
    Cout( name, " Init ed" );
}
bool FooState1::Process( int _ticks )
{
    COR_BEGIN;
    Cout( name, " Process: before sleep( 2 )" );
    COR_SLEEP( 2 );
    Cout( name, " Process: sleeped" );
    COR_END;
}
void FooState1::Destroy()
{
    Cout( name, " Destroy ing" );
    // ...
    auto o = owner;
    String fs2Name;
    fs2Name.Append( o->name, "'s FS2" );
    FooState1::~FooState1();
    ( (FooState2*)o->state )->Init( o, std::move( fs2Name ) );
}

void FooState2::Init( Foo* _owner, String const& _name )
{
    new ( this )FooState2();
    owner = _owner;
    // ...
    name = _name;
    Cout( name, " Init ed" );
}
bool FooState2::Process( int _ticks )
{
    COR_BEGIN;
    Cout( name, " Process: before sleep( 4 )" );
    COR_SLEEP( 4 );
    Cout( name, " Process: sleeped" );
    COR_END;
}
void FooState2::Destroy()
{
    Cout( name, " Destroy ing" );
    // ...
    auto o = owner;
    FooState2::~FooState2();
    o->state = nullptr;
}



void Foo::Init( String const& _name )
{
    name = _name;
    auto stateName = name;
    stateName.Append( "'s FS1" );
    ( (FooState1*)state )->Init( this, stateName );
}

bool Foo::Process( int _ticks )
{
    if( state->Process( _ticks ) ) return true;
    state->Destroy();
    return state != nullptr;
}





int main()
{
    CorManager<Cor> cm;
    cm.CreateItem<Foo>( "Foo1" );
    cm.CreateItem<Foo>( "Foo2" );
    cm.CreateItem<Foo>( "Foo3" );
    int i = 0;
    do
    {
        Cout( "step: ", ++i );
    } while( cm.Process( i ) );

    system( "pause" );
    return 0;
}
