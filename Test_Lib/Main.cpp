#include "Lib/All.h"
#include "Pool.h"
#include "List.h"
int main()
{
    //{
    //    int length = 9999999;
    //    List<bool> s1;
    //    Stopwatch sw;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        s1.push( ( i % 2 ) == 0 );
    //    }
    //    std::cout << sw.elapsed() << " " << s1.size() << " " << ( s1.top() ? "true" : "false" ) << std::endl;
    //    std::cout << s1.byteSize() << std::endl;

    //    //while( s1.size() )
    //    //{
    //    //    std::cout << ( s1.top() ? "true " : "false " );
    //    //    s1.pop();
    //    //}
    //    //std::cout << s1.byteSize() << std::endl;

    //    sw.reset();
    //    std::stack<bool> s2;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        s2.push( ( i % 2 ) == 0 );
    //    }
    //    std::cout << sw.elapsed() << " " << s2.size() << " " << ( s2.top() ? "true" : "false" ) << std::endl;
    //}

    //{
    //    int length = 10000000;
    //    List<char> s1;
    //    Stopwatch sw;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        s1.push( i );
    //    }
    //    std::cout << sw.elapsed() << " " << s1.size() << " " << s1.top() << std::endl;
    //}
    //sw.reset();
    //List<char> s11;
    //for( int i = 0; i < 999; ++i )
    //{
    //    s11 = s1;
    //}
    //std::cout << sw.elapsed() << " " << s11.size() << " " << s11.top() << std::endl;

    //std::stack<char> s2;
    //sw.reset();
    //for( int i = 0; i < length; ++i )
    //{
    //    s2.push( i );
    //}
    //std::cout << sw.elapsed() << " " << s2.size() << " " << s2.top() << std::endl;
    //sw.reset();
    //std::stack<char> s22;
    //for( int i = 0; i < 999; ++i )
    //{
    //    s22 = s2;
    //}
    //std::cout << sw.elapsed() << " " << s22.size() << " " << s22.top() << std::endl;



    //std::cout << sizeof( Pool<4,4096> ) << std::endl;

    Pool<4, 4096> pool;
    pool.collect();
    //for( int i = 0; i < 999; ++i )
    //{
    //    auto p = (int*)pool.alloc();
    //    std::cout << p << std::endl;
    //}
    //pool.clear();

    return 0;
}
