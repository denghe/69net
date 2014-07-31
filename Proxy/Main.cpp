#include "Lib/All.h"

struct Foo
{
    int i;
    char s[ 123 ];
    char* ss;
};

int main()
{
    Cout( std::is_pod<Foo>::value );
    system( "pause" );
    return 0;
}

