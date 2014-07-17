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
    int count = 9999;
    Stopwatch sw;
    List<String> ss;
    for( int i = 0; i < count; ++i )
    {
        //ss.emplace( "asdfqwer" );
        //ss.push( "asdfqwer" );
        ss.insertAt( 0, "asdfqwer" );
    }
    cout( sw.elapsed(), ", ", ss.size() );

    std::vector<String> ss2;
    sw.reset();
    for( int i = 0; i < count; ++i )
    {
        //ss2.emplace_back( "asdfqwer" );
        //ss2.push_back( "asdfqwer" );
        ss2.insert( ss2.begin(), "asdfqwer" );
    }
    cout( sw.elapsed(), ", ", ss2.size() );


    sw.reset();
    int c = 0;
    while( ss.size() )
    {
        ss.erase( 0 );
        c++;
    }
    cout( sw.elapsed(), ", ", ss.size(), ",", c );

    sw.reset();
    c = 0;
    while( ss2.size() )
    {
        ss2.erase( ss2.begin() );
        c++;
    }
    cout( sw.elapsed(), ", ", ss2.size(), ",", c );


    return 0;
}
