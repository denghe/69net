#include "Lib/All.h"
#include "Dict.h"
int main()
{
    using namespace std;
    int length = 999999;
    {
        Dict<string, int> d;
        Stopwatch sw;
        for( int i = 0; i < length; ++i )
        {
            d.insert( to_string( i ), i );
        }
        cout << "Dict insert: " << sw.elapsed() << " " << d.size() << endl;

        sw.reset();
        uint count = 0;
        for( int i = 0; i < length; ++i )
        {
            if( auto r = d.find( to_string( i ) ) )
            {
                count += r->value;
            }
        }
        cout << "Dict find: " << sw.elapsed() << " " << count << endl;

        sw.reset();
        count = 0;
        for( int i = 0; i < d.size(); ++i )
        {
            count += d.data()[ i ]->value;
        }
        cout << "Dict foreach: " << sw.elapsed() << " " << count << endl;

        sw.reset();
        count = 0;
        for( int i = 0; i < length; ++i )
        {
            if( auto r = d.find( to_string( i ) ) )
            {
                count += r->value;
                d.erase( r );
            }
        }
        cout << "Dict erase: " << sw.elapsed() << " " << count << endl;
    }

    {
        unordered_map<string, int> d;
        Stopwatch sw;
        for( int i = 0; i < length; ++i )
            d.insert( make_pair( to_string( i ), i ) );
        cout << "unordered_map insert: " << sw.elapsed() << " " << d.size() << endl;

        sw.reset();
        uint count = 0;
        for( int i = 0; i < length; ++i )
        {
            auto it = d.find( to_string( i ) );
            if( it != d.end() )
            {
                count += it->second;
            }
        }
        cout << "unordered_map find: " << sw.elapsed() << " " << count << endl;

        sw.reset();
        count = 0;
        for( auto& it : d )
        {
            count += it.second;
        }
        cout << "unordered_map foreach: " << sw.elapsed() << " " << count << endl;

        sw.reset();
        count = 0;
        for( int i = 0; i < length; ++i )
        {
            auto it = d.find( to_string( i ) );
            if( it != d.end() )
            {
                count += it->second;
                d.erase( it );
            }
        }
        cout << "unordered_map erase: " << sw.elapsed() << " " << count << endl;
    }










    //{
    //    int length = 99999;
    //    Dict<String, int> d;   // ( length );
    //    Stopwatch sw;
    //    ALIGN8( char* buf[ 32 ] );
    //    String s( (char*)buf, _countof( buf ) );
    //    for( int i = 0; i < length; ++i )
    //    {
    //        s.clear();
    //        s.append( i );
    //        d.insert( s, i );
    //    }
    //    std::cout << sw.elapsed() << " " << d.size() << std::endl;

    //    sw.reset();
    //    uint count = 0;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        s.clear();
    //        s.append( i );
    //        if( auto r = d.find( s ) ) count += r->value;
    //    }
    //    std::cout << sw.elapsed() << " " << count << std::endl;
    //}

    //{
    //    using namespace std;
    //    int length = 99999;
    //    map<string, int> d;
    //    //d.reserve ??
    //    Stopwatch sw;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        d.insert( make_pair( to_string( i ), i ) );
    //    }
    //    std::cout << sw.elapsed() << " " << d.size() << std::endl;

    //    sw.reset();
    //    uint count = 0;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        auto it = d.find( to_string( i ) );
    //        if( it != d.end() ) count += it->second;
    //    }
    //    std::cout << sw.elapsed() << " " << count << std::endl;
    //}

    /* int length = 9999;
     {
     Dict<int, String> d( length );
     Stopwatch sw;
     ALIGN8( char* buf[ 32 ] );
     String s( (char*)buf, _countof( buf ) );
     for( int i = 0; i < length; ++i )
     {
     for( int j = 0; j < length / 10; ++j )
     {
     s.clear();
     s.append( i, " ", j );
     if( auto n = d.insert( i, s ) ) n->value.append( ", ", i, " ", j );
     }
     }
     std::cout << sw.elapsed() << " " << d.size() << std::endl;

     sw.reset();
     uint count = 0;
     for( int i = 0; i < length; ++i )
     {
     if( auto r = d.find( i ) ) count += r->key;
     }
     std::cout << sw.elapsed() << " " << count << std::endl;

     std::cout << std::endl << d.find( length / 10 / 2 )->value.c_str() << std::endl;
     }

     {
     using namespace std;
     unordered_map<int, string> d;
     d.reserve( length );
     Stopwatch sw;
     for( int i = 0; i < length; ++i )
     {
     for( int j = 0; j < length / 10; ++j )
     {
     auto it = d.insert( make_pair( i, to_string( i ) + " " + to_string( j ) ) );
     if( !it.second ) it.first->second += ( ", " + to_string( i ) + " " + to_string( j ) );
     }
     }
     std::cout << sw.elapsed() << " " << d.size() << std::endl;

     sw.reset();
     uint count = 0;
     for( int i = 0; i < length; ++i )
     {
     auto it = d.find( i );
     if( it != d.end() ) count += it->first;
     }
     std::cout << sw.elapsed() << " " << count << std::endl;

     std::cout << std::endl << d.find( length / 10 / 2 )->second << std::endl;
     }*/




    //int length = 99999;
    //{
    //    Dict<int, int> d( length );
    //    Stopwatch sw;
    //    for( int i = 0; i < length; ++i )
    //        for( int j = 0; j < 1000; ++j )
    //            if( auto n = d.insert( i, 0 ) )
    //                ++n->value;
    //    std::cout << sw.elapsed() << " " << d.size() << std::endl;

    //    sw.reset();
    //    uint count = 0;
    //    for( int i = 0; i < length; ++i )
    //        if( auto r = d.find( i ) )
    //            count += r->value;
    //    std::cout << sw.elapsed() << " " << count << std::endl;
    //}

    //{
    //    using namespace std;
    //    unordered_map<int, int> d;
    //    d.reserve( length );
    //    Stopwatch sw;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        for( int j = 0; j < 1000; ++j )
    //        {
    //            auto it = d.insert( make_pair( i, 0 ) );
    //            if( !it.second )
    //                ++it.first->second;
    //        }
    //    }
    //    std::cout << sw.elapsed() << " " << d.size() << std::endl;

    //    sw.reset();
    //    uint count = 0;
    //    for( int i = 0; i < length; ++i )
    //    {
    //        auto it = d.find( i );
    //        if( it != d.end() ) count += it->second;
    //    }
    //    std::cout << sw.elapsed() << " " << count << std::endl;
    //}


    return 0;
}
