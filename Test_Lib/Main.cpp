#include "Lib/All.h"
template<typename ...TS>
void cout( TS const& ...parms )
{
    String s;
    s.append( parms... );
    std::cout << s.c_str() << std::endl;
}
int main()
{
    //int const count = 9999999;
    //Stopwatch sw;
    //Pool p( 64, 4096, count, true );
    //cout( "new pool, elapsed time = ", sw.elapsed() );
    //sw.reset();
    //for( int i = 0; i < count; ++i )
    //{
    //    p.alloc();
    //}
    //cout( "alloc, elapsed time = ", sw.elapsed() );

    //sw.reset();
    //for( int i = 0; i < count; ++i )
    //{
    //    new char[ 64 ];
    //}
    //cout( "new, elapsed time = ", sw.elapsed() );




    int const count = 9999999;
    Stopwatch sw;
    {
        std::vector<std::string> ss;
        ss.reserve( count );

        sw.reset();
        for( int i = 0; i < count; ++i )
        {
            ss.emplace_back( "12345678901234567890123456789012345678901234567890" );
        }
        cout( "push std strings, elapsed time = ", sw.elapsed() );

        cout( ss[ count - 1 ] );
        sw.reset();
    }
    cout( "free std ss, elapsed time = ", sw.elapsed() );

    sw.reset();
    Pool p( 64, 4096, count, true );
    cout( "new pool, elapsed time = ", sw.elapsed() );
    {
        List<String> ss( count );

        sw.reset();
        for( int i = 0; i < count; ++i )
        {
            // 从 Pool 分配 String 的首发内存 并复制内容进去
            ss.emplace( p, "12345678901234567890123456789012345678901234567890" );
        }
        cout( "push Strings, elapsed time = ", sw.elapsed() );
        cout( ss[ count - 1 ] );

        sw.reset();
    }
    cout( "free ss, elapsed time = ", sw.elapsed() );




    return 0;
}
