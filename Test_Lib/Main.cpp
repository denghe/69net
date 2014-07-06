#include "Lib/All.h"
using namespace std;
int main()
{
    //int count = 9999999;
    //string s;
    //Stopwatch sw;
    //String tmp( 64 );
    //for( int i = 0; i < count; ++i )
    //{
    //    tmp.appendFormat( "{2} {1} {0} {2} {1} {0} {2}", "World", "Hello", i );
    //    s.assign( tmp.c_str(), tmp.size() );
    //    tmp.clear();
    //}
    //cout << sw.elapsed() << "  " << s << endl;



    int num_groups = 4;
    int num_items = 200000;
    auto data = new List<List<HashString>*>();
    for( int i = 0; i < num_groups; ++i )
    {
        auto items = new List<HashString>();
        for( int j = 0; j < num_items; ++j )
        {
            items->push( String::make( i, ',', j ) );
            //items->push( String::make( i * num_items + j ) );
        }
        data->push( items );
        items->at( num_groups - 1 ) = "erererere";  // 制造一个交集
    }

    Dict<HashString, int> dict( num_groups * num_items );
    List<HashString> result( num_items );

    Stopwatch sw;
    for( int i = 0; i < data->size(); ++i )
    {
        auto items = data->at( i );
        for( int j = 0; j < items->size(); ++j )
        {
            if( auto o = dict.insert( items->at( j ), 1, false ) )
            {
                ++o->value;
            }
        }
    }
    for( int i = 0; i < dict.size(); ++i )
    {
        auto o = dict.data()[ i ];
        if( o->value == data->size() )
            result.push( o->key );
    }
    cout << sw.elapsed() << endl;
    cout << result.size() << endl;
    //for( int i = 0; i < result.size(); ++i )
    //    cout << result[ i ].c_str() << endl;



    //int num_groups = 4;
    //int num_items = 200000;
    //auto data = new vector<vector<string>*>();
    //for( int i = 0; i < num_groups; ++i )
    //{
    //    auto items = new vector<string>();
    //    for( int j = 0; j < num_items; ++j )
    //    {
    //        items->push_back( to_string( j ) + to_string( j ) + to_string( j ) + to_string( j ) + to_string( j ) );
    //        //items->push_back( to_string( i * num_items + j ) );
    //    }
    //    data->push_back( items );
    //}
    ////data->at( 0 )->at( 99 ) = "asdf";
    ////data->at( 1 )->at( 199 ) = "asdf";
    ////data->at( 2 )->at( 299 ) = "asdf";
    ////data->at( 3 )->at( 399 ) = "asdf";

    //unordered_map<string, int> dict;
    //dict.reserve( num_groups * num_items );
    //vector<string> result;
    //result.reserve( num_items );

    //Stopwatch sw;
    //for( int i = 0; i < data->size(); ++i )
    //{
    //    auto items = data->at( i );
    //    for( int j = 0; j < items->size(); ++j )
    //    {
    //        auto it = dict.insert( make_pair( items->at( j ), 1 ) );
    //        if( !it.second )
    //        {
    //            ++it.first->second;
    //        }
    //    }
    //}
    //for( auto& kv : dict )
    //{
    //    if( kv.second == data->size() )
    //        result.push_back( kv.first );
    //}
    //cout << sw.elapsed() << endl;

    //cout << result.size() << endl;
    ////for( int i = 0; i < result.size(); ++i )
    ////    cout << result[ i ].c_str() << endl;

    return 0;
}
