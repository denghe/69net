#include "Lib/All.h"
using namespace std;
using namespace xxx;

struct Foo
{
    explicit Foo( int n ) { Cout( n ); }
};

int main()
{
    Nullable<Foo> f;
    Cout( f.HasValue() );
    f.Emplace( 123 );
    Cout( f.HasValue() );
    f.Clear();
    Cout( f.HasValue() );

    system( "pause" );
    return 0;
}
