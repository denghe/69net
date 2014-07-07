#include "Lib/All.h"
using namespace std;
int main()
{
    int count = 9999999;
    string s;
    Stopwatch sw;
    String tmp( 64 );
    for( int i = 0; i < count; ++i )
    {
        tmp.appendFormat( "{2} {1} {0} {2} {1} {0} {2}", "World", "Hello", i );
        s.assign( tmp.c_str(), tmp.size() );
        tmp.clear();
    }
    cout << sw.elapsed() << "  " << s << endl;


    return 0;
}
