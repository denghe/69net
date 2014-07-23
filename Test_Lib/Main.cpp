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
    int i = 0;
    cout( String::toString( ++i ), String::toString( ++i ), String::toString( ++i ) );
    return 0;
}
