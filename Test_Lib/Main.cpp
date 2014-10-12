#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <cassert>
#include <list>
#include "Lib/All.h"
using namespace std;

struct StdFoo
{
    list<StdFoo*>::iterator it;
};

struct MyFoo
{
    Links<MyFoo*>::Node* it;
};

int main()
{
    vector<StdFoo*> stdfoos;
    stdfoos.reserve( 9999999 );
    vector<MyFoo*> myfoos;
    myfoos.reserve( 9999999 );

    for( int x = 0; x < 10; ++x )
    {

        {
            int count = 0;
            list<StdFoo*> c;
            Stopwatch sw;
            for( int i = 0; i < 9999999; ++i )
            {
                auto foo = new StdFoo();
                stdfoos.push_back( foo );
                c.push_back( foo );
                foo->it = --c.end();
            }
            Cout( "std::list pus_back ms: ", sw.elapsedMillseconds() );
            Cout( c.size() );

            sw.reset();
            for( int i = 0; i < 9999999; ++i )
            {
                c.erase( stdfoos[ i ]->it );
                delete stdfoos[ i ];
            }
            Cout( "std::list erase ms: ", sw.elapsedMillseconds() );

            stdfoos.clear();
        }

        {
            int count = 0;
            set<StdFoo*> c;
            Stopwatch sw;
            for( int i = 0; i < 9999999; ++i )
            {
                auto foo = new StdFoo();
                stdfoos.push_back( foo );
                c.insert( foo );
            }
            Cout( "std::set insert ms: ", sw.elapsedMillseconds() );
            Cout( c.size() );

            sw.reset();
            for( int i = 0; i < 9999999; ++i )
            {
                c.erase( stdfoos[ i ] );
                delete stdfoos[ i ];
            }
            Cout( "std::set erase ms: ", sw.elapsedMillseconds() );

            stdfoos.clear();
        }

        {
            int count = 0;
            Links<MyFoo*> c;
            Stopwatch sw;
            for( int i = 0; i < 9999999; ++i )
            {
                auto foo = new MyFoo();
                myfoos.push_back( foo );
                foo->it = c.insert( foo );
            }
            Cout( "Links pus_back ms: ", sw.elapsedMillseconds() );
            Cout( c.size() );

            sw.reset();
            for( int i = 0; i < 9999999; ++i )
            {
                c.erase( myfoos[ i ]->it );
                delete myfoos[ i ];
            }
            Cout( "Links erase ms: ", sw.elapsedMillseconds() );

            myfoos.clear();
        }

        {
            int count = 0;
            Hash<MyFoo*> c;
            Stopwatch sw;
            for( int i = 0; i < 9999999; ++i )
            {
                auto foo = new MyFoo();
                myfoos.push_back( foo );
                c.insert( foo );
            }
            Cout( "Hash pus_back ms: ", sw.elapsedMillseconds() );
            Cout( c.size() );

            sw.reset();
            for( int i = 0; i < 9999999; ++i )
            {
                c.erase( myfoos[ i ] );
                delete myfoos[ i ];
            }
            Cout( "Hash erase ms: ", sw.elapsedMillseconds() );

            myfoos.clear();
        }

        Cout( "\n\n" );
    }

    //int count;
    //for( int x = 0; x < 10; ++x )
    //{
    //    {
    //        list<int> c;
    //        Stopwatch sw;
    //        for( int i = 0; i < 9999999; ++i )
    //        {
    //            c.push_back( i );
    //        }
    //        Cout( "std::list pus_back ms: ", sw.elapsedMillseconds() );
    //        Cout( c.size() );

    //        count = 0;
    //        sw.reset();
    //        for( auto i : c ) count += i;
    //        Cout( "std::list foreach ms: ", sw.elapsedMillseconds() );
    //        Cout( count );

    //        count = 0;
    //        sw.reset();
    //        for( int i = 0; i < 99999; ++i )
    //        {
    //            auto it = find( c.begin(), c.end(), i );
    //            if( it != c.end() )
    //            {
    //                count += ( *it );
    //            }
    //        }
    //        Cout( "std::list find ms: ", sw.elapsedMillseconds() );
    //        Cout( count );

    //        vector<decltype( c.begin() )> its;
    //        for( auto it = c.begin(); it != c.end(); ++it ) its.push_back( it );
    //        sw.reset();
    //        for( auto it : its ) c.erase( it );
    //        Cout( "std::list erase ms: ", sw.elapsedMillseconds() );
    //        Cout( count );
    //    }

    //    count = 0;
    //    {
    //        Links<int> c;
    //        Stopwatch sw;
    //        for( int i = 0; i < 9999999; ++i )
    //        {
    //            c.insert( i );
    //        }
    //        Cout( "Links insert ms: ", sw.elapsedMillseconds() );
    //        Cout( c.size() );

    //        count = 0;
    //        sw.reset();
    //        for( int i = 0; i < 9999999; ++i ) count += c[ i ]->value;
    //        Cout( "Links foreach ms: ", sw.elapsedMillseconds() );
    //        Cout( count );

    //        count = 0;
    //        sw.reset();
    //        for( int i = 0; i < 99999; ++i )
    //        {
    //            auto it = c.find( i );
    //            if( it ) count += it->value;
    //        }
    //        Cout( "Links find ms: ", sw.elapsedMillseconds() );
    //        Cout( count );

    //        vector<decltype( c.insert( 0 ) )> its;
    //        for( int i = 0; i < 9999999; ++i ) its.push_back( c[ i ] );
    //        sw.reset();
    //        for( auto it : its ) c.erase( it );
    //        Cout( "Links erase ms: ", sw.elapsedMillseconds() );
    //        Cout( count );
    //    }

    //    Cout( "\n\n" );
    //}

    system( "pause" );
    return 0;
}

