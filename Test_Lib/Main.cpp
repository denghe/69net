#include "Lib/All.h"
using namespace std;

#define COR_BEGIN switch( labNum ) { case 0:
#define COR_YIELD_RETURN labNum = __LINE__; return true; case __LINE__:
#define COR_END } return false;

struct Foo
{
    int i;
    int idx = 0;
    int labNum = 0;
    bool process()
    {
        COR_BEGIN;
        for( i = 0; i < 10; ++i )
        {
            Cout( i );
            COR_YIELD_RETURN;
        }
        COR_END;
    }
};

int main()
{
    List<Foo*> foos;
    for( int i = 0; i < 3; ++i )
    {
        auto foo = new Foo();
        foo->idx = foos.size();
        foos.push( foo );
    }
    int alive = foos.size();
    while( alive )
    {
        for( int i = foos.size() - 1; i >= 0; --i )
        {
            auto& foo = foos[ i ];
            if( !foo->process() )
            {
                foos.top()->idx = i;
                foos.eraseFast( i );
                delete foo;
                --alive;
            }
        }
    }

    system( "pause" );
    return 0;
}
