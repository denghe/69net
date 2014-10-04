#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <cassert>
#include "Lib/All.h"
using namespace std;

int main()
{
    set<int> sss;
    {
        Stopwatch sw;
        for( int i = 0; i < 9999999; ++i )
        {
            sss.insert( i );
        }
        Cout( sw.elapsedMillseconds() );
        Cout( sss.size() );

        sw.reset();
        for( int i = 0; i < 9999999; ++i )
        {
            sss.erase( i );
        }
        Cout( sw.elapsedMillseconds() );
        Cout( sss.size() );
    }

    Set<int> myset;
    {
        Stopwatch sw;
        myset.reserve( 9999999 );
        for( int i = 0; i < 9999999; ++i )
        {
            myset.insert( i );
        }
        Cout( sw.elapsedMillseconds() );
        Cout( myset.size() );

        sw.reset();
        for( int i = 0; i < 9999999; ++i )
        {
            myset.erase( i );
        }
        Cout( sw.elapsedMillseconds() );
        Cout( myset.size() );
    }

    system( "pause" );
    return 0;
}

