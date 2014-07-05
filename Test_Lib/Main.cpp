#include "Lib/All.h"
int main()
{
    Stopwatch sw;
    String s( 999999999 );
    for( int i = 0; i < 9999999; ++i )
    {
        s.appendFormat( "{0}, {1}, {2}, {{3}, {3}", 111, "222erer", 33333, "ffffff44" );
    }
    std::cout << sw.elapsed() << ", " << s.size() << std::endl;

    return 0;
}
