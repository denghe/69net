#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <cassert>
#include "lib/All.h"
using namespace std;

typedef function<void()> StepTimerItemType;
struct StepTimerItemHandlerType
{
    inline static void handle( StepTimerItemType& o )
    {
        o();
    }
};

template<int len, typename T = StepTimerItemType, typename HT = StepTimerItemHandlerType>
class StepTimer
{
public:
    explicit StepTimer( int n = 0 ) : _counter( n ) {}
    void update( int elapsed = 1 )
    {
        for( int i = 0; i < elapsed; ++i )
        {
            auto& os = _oss[ ( _counter + i ) % len ];
            for( int i = 0; i < os.size(); ++i ) HT::handle( os[ i ] );
            os.clear();
        }
        _counter += elapsed;
    }
    template<typename VT>
    void insert( int n, VT&& v )
    {
        assert( n > 0 && n < len );
        _oss[ ( _counter + n ) % len ].push_back( std::forward<VT>( v ) );
        //_oss[ ( _counter + n ) % len ].push( std::forward<VT>( v ) );
    }
    void clear()
    {
        for( auto& os : _oss ) os.clear();
        _counter = 0;
    }
private:
    int _counter;
    vector<T> _oss[ len ];
    //List<T> _oss[ len ];
};

struct EEE
{
    void xxx()
    {
        //cout << "eee";
    }
};


struct FooBase
{
    function<void()> _timerCallback;
};

struct FooBaseTimerCallbackHandler
{
    inline static void handle( weak_ptr<FooBase>& o )
    {
        if( auto p = o.lock() )
            p->_timerCallback();
    }
};




struct Foo : public FooBase
{
};


int main()
{
    int n = 0;
    StepTimer<999, weak_ptr<FooBase>, FooBaseTimerCallbackHandler> t( n );
    int _counter = 0;
    vector<shared_ptr<Foo>> _foos;
    for( int i = 0; i < 1000; ++i )
    {
        auto f = make_shared<Foo>();
        auto wf = weak_ptr<Foo>( f );
        f->_timerCallback = [&t, &_counter, wf]
        {
            _counter++;
            t.insert( 1, wf );
        };
        t.insert( 1, wf );
        _foos.push_back( f );
    }

    Stopwatch sw;
    while( n < 10000 )
    {
        t.update();
        //if( n == 1000 )
        //{
        //    for( auto& f : _foos ) f.reset();
        //}
        ++n;
    }
    Cout( sw.elapsedMillseconds() );
    Cout( "\nn = ", n, ", counter = ", _counter );

    system( "pause" );
    return 0;
}



