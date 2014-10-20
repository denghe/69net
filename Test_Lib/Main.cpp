#include "Lib/All.h"
using namespace std;
using namespace xxx;


template<typename T>
struct StateBase : CorCore
{
    T* owner = nullptr;
};

struct Foo;

struct DieState : public StateBase < Foo >
{
    DieState( Foo* _owner );
    void Init();   // todo: more parms
    bool Process( int _ticks ) override;
    void Destroy() override;
};

struct LiveState : public StateBase < Foo >
{
    LiveState( Foo* _owner );
    void Init();   // todo: more parms
    bool Process( int _ticks ) override;
    void Destroy() override;
};

struct BornState : public StateBase < Foo >
{
    BornState( Foo* _owner );
    void Init();   // todo: more parms
    bool Process( int _ticks ) override;
    void Destroy() override;
};


struct Foo : public CorBase < Foo >
{
    BornState               bornState;
    LiveState               liveState;
    DieState                dieState;
    StateBase<Foo>*         currState = nullptr;
    std::function<void()>   changeState;
    Foo()
        : bornState( this )
        , liveState( this )
        , dieState( this )
    {
    }
    void Init();
    bool Process( int _ticks ) override;
};









void Foo::Init()
{
    changeState = [ this ] { bornState.Init(); };
}

bool Foo::Process( int _ticks )
{
    if( currState )
    {
        if( currState->Process( _ticks ) ) return true;
        currState->Destroy();
        currState = nullptr;
    }
    if( changeState ) changeState();
    if( currState ) return true;
    return false;
}






BornState::BornState( Foo* _owner )
{
    owner = _owner;
}

void BornState::Init()
{
    owner->currState = this;
    Cout( "born begin" );
}

bool BornState::Process( int _ticks )
{
    COR_BEGIN;
    Cout( "born ing..." );
    COR_SLEEP( 1 );
    auto _owner = owner;
    owner->changeState = [ _owner ] { _owner->liveState.Init(); };
    COR_END;
}

void BornState::Destroy()
{
    Cout( "born end" );
}



LiveState::LiveState( Foo* _owner )
{
    owner = _owner;
}

void LiveState::Init()
{
    owner->currState = this;
    Cout( "live begin" );
}

bool LiveState::Process( int _ticks )
{
    COR_BEGIN;
    Cout( "live ing..." );
    COR_SLEEP( 2 );
    auto _owner = owner;
    owner->changeState = [ _owner ] { _owner->dieState.Init(); };
    COR_END;
}

void LiveState::Destroy()
{
    Cout( "live end" );
}




DieState::DieState( Foo* _owner )
{
    owner = _owner;
}

void DieState::Init()
{
    owner->currState = this;
    Cout( "die begin" );
}

bool DieState::Process( int _ticks )
{
    COR_BEGIN;
    Cout( "die ing..." );
    COR_SLEEP( 3 );
    owner->changeState = nullptr;
    COR_END;
}

void DieState::Destroy()
{
    Cout( "die end" );
}



int main()
{
    CorManager<Cor> cm;
    cm.CreateItem<Foo>();
    int i = 0;
    do
    {
        Cout( "step: ", ++i );
    } while( cm.Process( i ) );

    system( "pause" );
    return 0;
}
