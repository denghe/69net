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
    FlatBuffer fb;

    int count = 999999;
    // 仿一个字典数据
    fb.write( count );
    for( int i = 0; i < count; ++i )
    {
        fb.writes( i, i );
    }

    Stopwatch sw;
    // 还原
    fb.offset() = 0;
    Dict<int, int> d;
    if( !fb.read( d ) ) cout( "read fail." );
    cout( d.size(), ' ', d[ 0 ], ' ', d[ count - 1 ], ", elapsed ms = ", sw.elapsed() );

    sw.reset();
    fb.offset() = 0;
    std::unordered_map<int, int> sd;
    int len;
    fb.read( len );
    for( int i = 0; i < len; ++i )
    {
        int k, v;
        if( !fb.read( k ) ) cout( "read fail." );
        if( !fb.read( v ) ) cout( "read fail." );
        sd.insert( std::make_pair( k, v ) );
    }
    cout( sd.size(), ' ', sd[ 0 ], ' ', sd[ count - 1 ], ", elapsed ms = ", sw.elapsed() );

    return 0;
}
