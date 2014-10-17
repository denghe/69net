#include "Lib/All.h"
using namespace std;
using namespace xxx;

template<typename T>
struct State : public CorBase < State<T> >
{
    T* owner = nullptr;
    //void Init( T* _owner ) { owner = _owner; };
};

struct Foo : public CorBase < Foo >
{
    State<Foo> *currState = nullptr;
    std::function<void()> changeState;
    template<typename T, typename ...PTS>
    void CreateState( PTS... ps )
    {
        currState = manager->CreateItem<T>( this, std::forward<PTS>( ps )... );
    }
    struct DieState : public State < Foo >
    {
        void Init( Foo* _owner ) { owner = _owner; };
        bool Process( int _ticks ) override
        {
            COR_BEGIN;
            COR_SLEEP( 3 );
            auto _owner = this->owner;
            owner->changeState = [ _owner ] { _owner->currState = nullptr; };
            COR_END;
        }
        void Destroy() override { owner = nullptr; }
    };
    struct LiveState : public State < Foo >
    {
        void Init( Foo* _owner ) { owner = _owner; };
        bool Process( int _ticks ) override
        {
            COR_BEGIN;
            COR_SLEEP( 2 );
            auto _owner = this->owner;
            owner->changeState = [ _owner ] { _owner->CreateState<DieState>(); };
            COR_END;
        }
        void Destroy() override { owner = nullptr; }
    };
    struct BornState : public State < Foo >
    {
        void Init( Foo* _owner ) { owner = _owner; };
        bool Process( int _ticks ) override
        {
            COR_BEGIN;
            COR_SLEEP( 1 );
            auto _owner = this->owner;
            owner->changeState = [ _owner ] { _owner->CreateState<LiveState>(); };
            COR_END;
        }
        void Destroy() override { owner = nullptr; }
    };
    void Init()
    {
        CreateState<BornState>();
    }
    bool Process( int _ticks ) override
    {
        do
        {
            if( currState )
            {
                if( currState->Process( _ticks ) ) return true;
                else manager->DestroyItem( currState );
            }
            if( changeState ) changeState();
        } while( currState );
        return false;
    }
};

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
