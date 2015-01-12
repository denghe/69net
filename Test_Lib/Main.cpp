#include "Lib/All.h"
using namespace xxx;

struct A
{
    int a = 123;
};
struct C
{
};
struct C0 : public C, public A
{
};
struct C1 : public C0, public A
{
};

int main()
{
    C1 c;
    c.C0::A::a = 1;
    c.A::a = 2;
    Cout( c.C0::A::a, ", ", c.A::a );
}



//#include "Lib/All.h"
//
//#include <stack>
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <cassert>
//#include <algorithm>
//using namespace std;
//
//#include "AStar.h"
//
//enum class ItemType : short
//{
//    None,
//    Wall,
//    Space,
//};
//
//struct Item
//{
//    ItemType type;
//    int x, y;
//    bool IsWalkable( Item const& o )
//    {
//        return o.type == ItemType::Space;
//    }
//};
//
//
//Map<Item>* Fill( int& aX, int& aY, int& bX, int& bY )
//{
//    ifstream f( "map.txt" );
//    string tmp;
//    vector<string> ss;
//    while( getline( f, tmp ) )
//    {
//        ss.push_back( tmp );
//    }
//
//    auto result = new Map<Item>( (int)ss[ 0 ].size(), (int)ss.size() );
//
//    for( int y = 0; y < ss.size(); ++y )
//    {
//        auto& s = ss[ y ];
//        for( int x = 0; x < s.size(); ++x )
//        {
//            auto& c = s[ x ];
//            auto& item = result->At( x, y );
//            switch( c )
//            {
//            case '%':
//                item = { ItemType::None, x, y };
//                break;
//            case '#':
//                item = { ItemType::Wall, x, y };
//                break;
//            case ' ':
//                item = { ItemType::Space, x, y };
//                break;
//            case '@':
//                item = { ItemType::Space, x, y };
//                aX = x; aY = y;
//                break;
//            case '*':
//                item = { ItemType::Space, x, y };
//                bX = x; bY = y;
//                break;
//            }
//        }
//    }
//
//    return result;
//}
//void Dump( Map<Item>& m, int aX, int aY, int bX, int bY, AStar<Item>* astar = nullptr )
//{
//    for( int y = 0; y < m.h; ++y )
//    {
//        for( int x = 0; x < m.w; ++x )
//        {
//            auto& item = m.At( x, y );
//            switch( item.type )
//            {
//            case ItemType::None:
//                cout << "%";
//                break;
//            case ItemType::Space:
//                if( aX == x && aY == y )
//                {
//                    cout << "@";
//                }
//                else if( bX == x && bY == y )
//                {
//                    cout << "*";
//                }
//                else
//                {
//                    if( astar )
//                    {
//                        bool found = false;
//                        for( auto& o : astar->searchResults )
//                        {
//                            if( o->x == x && o->y == y )
//                            {
//                                found = true;
//                                break;
//                            }
//                        }
//                        if( found )
//                        {
//                            cout << "+";
//                        }
//                        else
//                        {
//                            cout << " ";
//                        }
//                    }
//                    else cout << " ";
//                }
//                break;
//            case ItemType::Wall:
//                cout << "#";
//                break;
//            default:
//                assert( false );
//            }
//        }
//        cout << endl;
//    }
//    cout << endl;
//}
//
//#include "Lib/All.h"
//int main()
//{
//    int aX, aY, bX, bY;
//    auto m = Fill( aX, aY, bX, bY );
//    //Dump( *m, aX, aY, bX, bY );
//
//    AStar<Item> astar( m );
//
//    xxx::Stopwatch sw;
//    int count = 0;
//    for( int i = 0; i < 100; ++i )
//    {
//        if( astar.Search( aX, aY, bX, bY ) )
//        {
//            ++count;
//        }
//    }
//    cout << "elapsed ms = " << sw.ElapsedMillseconds() << endl;
//    cout << "count = " << count << endl;
//    cout << "map width = " << m->w << ", height = " << m->h << endl;
//    //if( count ) Dump( *m, aX, aY, bX, bY, &astar );
//
//    delete m;
//
//    system( "pause" );
//    return 0;
//}
