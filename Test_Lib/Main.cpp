#include "Lib/All.h"
using namespace std;
int main()
{
    cout << "sizeof(String) = " << sizeof( String ) << endl;

    List<String> ls;
    ls.reserve( 9999999 );
    Pool p( 128, 4096, 9999999, true );
    cout << "pool size = " << p.size() << endl;

    Stopwatch sw;
    for( int i = 0; i < 9999999; ++i )
    {
        ls.push( String( p ) );
    }
    cout << "ls.push( String( p ) );  time = " << sw.elapsed() << endl;
    cout << "pool size = " << p.size() << endl;

    sw.reset();
    ls.clear();
    cout << "ls.clear();  time = " << sw.elapsed() << endl;
    cout << "pool size = " << p.size() << endl;

    vector<string> vs;
    vs.reserve( 9999999 );
    sw.reset();
    for( int i = 0; i < 9999999; ++i )
    {
        string s;
        s.reserve( 128 );
        vs.push_back( move( s ) );
    }
    cout << "vs.push_back( move( s ) );  time = " << sw.elapsed() << endl;

    sw.reset();
    vs.clear();
    cout << "vs.clear();  time = " << sw.elapsed() << endl;

    return 0;
}
