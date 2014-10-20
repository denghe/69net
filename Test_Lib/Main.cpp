#include "Lib/All.h"
using namespace std;
using namespace xxx;


template<typename T>
struct StateBase : CorCore
{
    T* owner = nullptr;
};

template<typename T>
struct DieState : public StateBase < T >
{
    DieState( T* _owner );
    void Init();   // todo: more parms
    bool Process( int _ticks ) override;
    void Destroy() override;
};

template<typename T>
struct LiveState : public StateBase < T >
{
    LiveState( T* _owner );
    void Init();   // todo: more parms
    bool Process( int _ticks ) override;
    void Destroy() override;
};

template<typename T>
struct BornState : public StateBase < T >
{
    BornState( T* _owner );
    void Init();   // todo: more parms
    bool Process( int _ticks ) override;
    void Destroy() override;
};


struct Foo : public CorBase < Foo >
{
    BornState<Foo>          bornState;
    LiveState<Foo>          liveState;
    DieState<Foo>           dieState;
    StateBase<Foo>*         currState = nullptr;
    std::function<void()>   changeState;
    Foo();
    void Init();
    bool Process( int _ticks ) override;
};









Foo::Foo()
    : bornState( this )
    , liveState( this )
    , dieState( this )
{
}

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




template<typename T>
BornState<T>::BornState( T* _owner )
{
    owner = _owner;
}

template<typename T>
void BornState<T>::Init()
{
    owner->currState = this;
    Cout( "born begin" );
}

template<typename T>
bool BornState<T>::Process( int _ticks )
{
    COR_BEGIN;
    Cout( "born ing..." );
    COR_SLEEP( 1 );
    auto _owner = owner;
    owner->changeState = [ _owner ] { _owner->liveState.Init(); };
    COR_END;
}

template<typename T>
void BornState<T>::Destroy()
{
    Cout( "born end" );
}



template<typename T>
LiveState<T>::LiveState( T* _owner )
{
    owner = _owner;
}

template<typename T>
void LiveState<T>::Init()
{
    owner->currState = this;
    Cout( "live begin" );
}

template<typename T>
bool LiveState<T>::Process( int _ticks )
{
    COR_BEGIN;
    Cout( "live ing..." );
    COR_SLEEP( 2 );
    auto _owner = owner;
    owner->changeState = [ _owner ] { _owner->dieState.Init(); };
    COR_END;
}

template<typename T>
void LiveState<T>::Destroy()
{
    Cout( "live end" );
}




template<typename T>
DieState<T>::DieState( T* _owner )
{
    owner = _owner;
}

template<typename T>
void DieState<T>::Init()
{
    owner->currState = this;
    Cout( "die begin" );
}

template<typename T>
bool DieState<T>::Process( int _ticks )
{
    COR_BEGIN;
    Cout( "die ing..." );
    COR_SLEEP( 3 );
    owner->changeState = nullptr;
    COR_END;
}

template<typename T>
void DieState<T>::Destroy()
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
