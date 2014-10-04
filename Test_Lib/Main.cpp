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
    {
        set<int> sss;
        Stopwatch sw;
        for( int i = 5555; i < 9999999; ++i )
        {
            auto oldval = ( i - 5555 ) % 9999;
            auto newval = i % 9999;
            sss.erase( oldval );
            sss.insert( newval );
        }
        Cout( sw.elapsedMillseconds() );
        Cout( sss.size() );
    }

    {
        vector<int> vvv;
        Stopwatch sw;
        for( int i = 5555; i < 9999999; ++i )
        {
            auto oldval = ( i - 5555 ) % 9999;
            auto newval = i % 9999;
            //vvv.erase( oldval );
            auto it = find( vvv.begin(), vvv.end(), oldval );
            if( it != vvv.end() ) vvv.erase( it );
            vvv.push_back( newval );
        }
        Cout( sw.elapsedMillseconds() );
        Cout( vvv.size() );
    }

    {
        Set<int> sss;
        Stopwatch sw;
        for( int i = 5555; i < 9999999; ++i )
        {
            auto oldval = ( i - 5555 ) % 9999;
            auto newval = i % 9999;
            sss.erase( oldval );
            sss.insert( newval );
        }
        Cout( sw.elapsedMillseconds() );
        Cout( sss.size() );
    }


    //Set<int> myset;
    //{
    //    Stopwatch sw;
    //    myset.reserve( 9999999 );
    //    for( int i = 0; i < 9999999; ++i )
    //    {
    //        myset.insert( i );
    //    }
    //    Cout( sw.elapsedMillseconds() );
    //    Cout( myset.size() );

    //    sw.reset();
    //    for( int i = 0; i < 9999999; ++i )
    //    {
    //        myset.erase( i );
    //    }
    //    Cout( sw.elapsedMillseconds() );
    //    Cout( myset.size() );
    //}


    system( "pause" );
    return 0;
}

