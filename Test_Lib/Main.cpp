#include "Lib/All.h"
#include "Pool.h"
#include "Stack.h"
int main()
{
    Stopwatch sw;
    int length = 10000000;
    Stack<int> s( length );
    for( int i = 0; i < length; ++i )
    {
        s.push( i );
    }
    std::cout << sw.elapsed() << std::endl;


    //Stack<int> st;
    //st.push( 123 );
    //st.pop();

    //Pool<4, 4096> pool;
    //for( int i = 0; i < 999; ++i )
    //{
    //    auto p = (int*)pool.alloc();
    //    std::cout << p << std::endl;
    //}
    //pool.clear();

    return 0;
}
