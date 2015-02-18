#include "Lib/All.h"

inline void w7( char * buf, int & offset, uint32 v )
{
Lab1:
    auto b7 = uint8( v );
    if( ( v >>= 7 ) )
    {
        buf[ offset++ ] = b7 | 0x80u;
        goto Lab1;
    }
    buf[ offset++ ] = b7;
}

inline void r7( uint32 & v, char * buf, int & offset )
{
    auto p = buf + offset;
    uint i = 0, b7, lshift = 0, idx = 0;
Lab1:
    b7 = p[ idx++ ];
    if( b7 > 0x7F )
    {
        i |= ( b7 & 0x7F ) << lshift;
        lshift += 7;
        goto Lab1;
    }
    else i |= b7 << lshift;
    v = i;
    offset += idx;
}

inline void w72( char * buf, int & offset, uint32 v )
{
    if( v <= 0x7Fu )
    {
        buf[ offset++ ] = v;
    }
    else if( v <= 0x3FFFu )
    {
        buf[ offset ] = v | 0x80u;
        buf[ offset + 1 ] = v >> 7;
        offset += 2;
    }
    else if( v <= 0x1FFFFFu )
    {
        buf[ offset ] = v | 0x80u;
        buf[ offset + 1 ] = ( v >> 7 ) | 0x80u;
        buf[ offset + 2 ] = v >> 14;
        offset += 3;
    }
    else if( v <= 0xFFFFFFFu )
    {
        buf[ offset ] = v | 0x80u;
        buf[ offset + 1 ] = ( v >> 7 ) | 0x80u;
        buf[ offset + 2 ] = ( v >> 14 ) | 0x80u;
        buf[ offset + 3 ] = v >> 21;
        offset += 4;
    }
    else
    {
        buf[ offset ] = v | 0x80u;
        buf[ offset + 1 ] = ( v >> 7 ) | 0x80u;
        buf[ offset + 2 ] = ( v >> 14 ) | 0x80u;
        buf[ offset + 3 ] = ( v >> 21 ) | 0x80u;
        buf[ offset + 4 ] = v >> 28;
        offset += 5;
    }
}

using namespace std;
using namespace xxx;
int main()
{
    ByteBuffer fb;
    uint count = 0xFFFFFFFF;

    char buf[ 16 ];
    int offset = 0;
    uint forRead = 0;

    Stopwatch sw;
    for( uint i = 0; i < count; ++i )
    {
        offset = 0;
        w72( buf, offset, i );
        //offset = 0;
        //r7( forRead, buf, offset );
    }
    Cout( sw.ElapsedMillseconds(), "ms" );
    offset = 0;
    r7( forRead, buf, offset );
    Cout( forRead );






    //for( int i = 0; i < count; ++i )
    //{
    //    fb.write( i );
    //}
    //fb.clear();
    //Stopwatch sw;
    //for( int i = 0; i < count; ++i )
    //{
    //    fb.write( i );
    //}
    //Cout( "write ", count, " i to fb, elapsed ", sw.elapsedMillseconds(), "ms" );
    //Cout( fb.size() );


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
