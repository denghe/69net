#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <cassert>
#include "lib/All.h"
using namespace std;
template<int len>
class StepTimer
{
public:
    explicit StepTimer( int n = 0 ) : _counter( n ) {}
    void update( int elapsed = 1 )
    {
        for( int i = 0; i < elapsed; ++i )
        {
            auto& os = _oss[ ( _counter + i ) % len ];
            for( int i = 0; i < os.size(); ++i ) os[ i ]();
            os.clear();
        }
        _counter += elapsed;
    }
    template<typename VT>
    void insert( int n, VT&& v )
    {
        assert( n < len );
        _oss[ ( _counter + n ) % len ].emplace_back( std::forward<VT>( v ) );
        //_oss[ ( _counter + n ) % len ].emplace( std::forward<VT>( v ) );
    }
    void clear()
    {
        for( auto& os : _oss ) os.clear();
        _counter = 0;
    }
private:
    int _counter;
    vector<function<void()>> _oss[ len ];
    //List<function<void()>> _oss[ len ];
};

struct EEE
{
    void xxx() { cout << "eee"; }
};

int main()
{
    int n = 0;
    typedef StepTimer<999> TT;
    TT t( n );
    function<void()> f;
    f = [ &]
    {
        //cout << n << "\n";
        t.insert( 3, f );
    };

    //auto e = make_shared<EEE>();
    //weak_ptr<EEE> we( e );
    //t.insert( 10, [ we ]
    //{
    //    if( auto p = we.lock() )
    //        p->xxx();
    //} );

    Stopwatch sw;
    n = 0;
    t.clear();
    t.insert( 0, f );
    while( n++ < 9999999 )
    {
        t.update();
        //if( n == 5 ) e.reset();
    }
    Cout( sw.elapsedMillseconds() );
    cout << "\nn = " << n << endl;

    system( "pause" );
    return 0;
}



//#include "Lib/All.h"
//#include "PacketTestPackets.h"
//using namespace PacketTestPackets;
//
//int main()
//{
//
//    // todo: 生成物增加 fillTo( fb, p1, p2, ... ) 以及 fillTo( fb, T& dest ) 版
//
//    //FlatBuffer fb;
//    //Foo f;
//    //f._isss[ 0 ][ 0 ][ 0 ] = 1;
//    //f._isss[ 1 ][ 0 ][ 0 ] = 2;
//    //f._isss[ 2 ][ 0 ][ 0 ] = 3;
//    //f._isss[ 0 ][ 1 ][ 0 ] = 4;
//    //f._isss[ 1 ][ 1 ][ 0 ] = 5;
//    //f._isss[ 2 ][ 1 ][ 0 ] = 6;
//    //fb.write( f );
//    //Cout( fb.dump() );
//
//    ////FlatBuffer fb;
//    //////Foo3 f;
//    //////f._f._f._bool = true;
//    //////f._f._f._byte = 1;
//    //////f._f._f._short = 2;
//    //////f._f._f._int = 3;
//    //////f._f._f._long = 4;
//    //////f._f._int = 5;
//    //////f._s = "asdfqwer";
//
//    ////int n = 99999999;
//
//    ////Foo f;
//    ////Stopwatch sw;
//    ////sw.reset();
//    ////for( int i = 0; i < n; ++i )
//    ////{
//    ////    fb.clear();
//    ////    f.writeBufferDirect( fb );
//    ////}
//    ////Cout( sw.elapsedMillseconds() );
//    ////Cout( fb.dump() );
//
//    ////sw.reset();
//    ////for( int i = 0; i < n; ++i )
//    ////{
//    ////    fb.offset() = 0;
//    ////    if( !f.readBuffer( fb ) ) return 0;
//    ////}
//    ////Cout( sw.elapsedMillseconds() );
//    ////Cout( f._byte );
//    //////Cout( f._long );
//
//    ////Cout( std::is_pod<Foo>::value );
//    ////Cout( std::is_pod<Foo2>::value );
//    ////Cout( std::is_pod<Foo3>::value );
//
//    system( "pause" );
//    return 0;
//}
