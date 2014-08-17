#include "Lib/All.h"
#include "PacketTestPackets.h"

using namespace std;
int main()
{
    //FlatBuffer fb;
    //int count = 100000;

    //PacketTestPackets::Obj_t3 t3;
    //// ‘§»»
    //for( int i = 0; i < count; ++i )
    //{
    //    fb.write( t3 );
    //}

    //fb.clear();
    //Stopwatch sw;
    //for( int i = 0; i < count; ++i )
    //{
    //    fb.write( t3 );
    //}
    //Cout( "write ", count, " t3 to fb, elapsed ", sw.elapsedMillseconds(), "ms" );
    //Cout( fb.size() );

    //sw.reset();
    //for( int i = 0; i < count; ++i )
    //{
    //    fb.read( t3 );
    //}
    //Cout( "read ", count, " t3 from fb, elapsed ", sw.elapsedMillseconds(), "ms" );
    //Cout( fb.offset() );

    //int count = 999999;
    //Pool p( 32 );
    //vector<String> ss;
    //vector<string> stdss;
    //// ‘§»»
    //for( int i = 0; i < count; ++i )
    //{
    //    ss.emplace_back( p );
    //    stdss.emplace_back();
    //}

    //ss.clear();
    //Stopwatch sw;
    //for( int i = 0; i < count; ++i )
    //{
    //    ss.emplace_back( "asdfasdfasdfasdfasdf" );
    //}
    //Cout( "List push new String: ", sw.elapsedMillseconds() );

    //ss.clear();

    //sw.reset();
    //for( int i = 0; i < count; ++i )
    //{
    //    ss.emplace_back( p, "asdfasdfasdfasdfasdf" );
    //}
    //Cout( "List push pool String: ", sw.elapsedMillseconds() );

    //stdss.clear();
    //sw.reset();
    //for( int i = 0; i < count; ++i )
    //{
    //    stdss.emplace_back( "asdfasdfasdfasdfasdf" );
    //}
    //Cout( "List push empty std::string: ", sw.elapsedMillseconds() );


    //{
    //    Dict<String, String> d;
    //    Stopwatch sw;
    //    Pool p( 128 );
    //    for( int i = 0; i < 999999; ++i )
    //    {
    //        d.insert( String::make( p, "asdfqwerasdfqwerasdfqwer", i ), String::make( p, "asdfqwerasdfqwerasdfqwer", i ) );
    //    }
    //    Cout( sw.elapsedMillseconds() );
    //    Cout( d[ "asdfqwerasdfqwerasdfqwer99998" ] );
    //    sw.reset();
    //    d.clear();
    //    Cout( sw.elapsedMillseconds() );
    //}

    //{
    //    unordered_map<string, string> d;
    //    Stopwatch sw;
    //    for( int i = 0; i < 999999; ++i )
    //    {
    //        d.insert( make_pair( "asdfqwerasdfqwerasdfqwer" + to_string( i ), "asdfqwerasdfqwerasdfqwer" + to_string( i ) ) );
    //    }
    //    cout << sw.elapsedMillseconds() << endl;
    //    cout << d[ "asdfqwerasdfqwerasdfqwer99998" ] << endl;
    //    sw.reset();
    //    d.clear();
    //    cout << sw.elapsedMillseconds() << endl;
    //}

    system( "pause" );
    return 0;
}



//#include "Lib/All.h"
//
//int main()
//{
//    static const int tc = 100000000;
//    Pool pool( 4 );
//    std::cout << "Time: ";
//    Stopwatch sw;
//    for( int i = 0; i < tc; ++i )
//    {
//        void * p1 = pool.alloc();
//        void * p2 = pool.alloc();
//        void * p3 = pool.alloc();
//        void * p4 = pool.alloc();
//        void * p5 = pool.alloc();
//        void * p6 = pool.alloc();
//        void * p7 = pool.alloc();
//        void * p8 = pool.alloc();
//        pool.free( p1 );
//        pool.free( p2 );
//        pool.free( p3 );
//        pool.free( p4 );
//        pool.free( p5 );
//        pool.free( p6 );
//        pool.free( p7 );
//        pool.free( p8 );
//    }
//    std::cout << sw.elapsedMillseconds() << "s" << std::endl;
//    system( "pause" );
//    return 0;
//}
