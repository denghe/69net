#include "Lib/All.h"

int main()
{
    static const int tc = 100000000;
    FastPool pool( 4 );
    std::cout << "Time: ";
    Stopwatch sw;
    for( int i = 0; i < tc; ++i )
    {
        void * p1 = pool.alloc();
        void * p2 = pool.alloc();
        void * p3 = pool.alloc();
        void * p4 = pool.alloc();
        void * p5 = pool.alloc();
        void * p6 = pool.alloc();
        void * p7 = pool.alloc();
        void * p8 = pool.alloc();
        pool.free( p1 );
        pool.free( p2 );
        pool.free( p3 );
        pool.free( p4 );
        pool.free( p5 );
        pool.free( p6 );
        pool.free( p7 );
        pool.free( p8 );
    }
    std::cout << sw.elapsedMillseconds() << "s" << std::endl;
    system( "pause" );
    return 0;
}
