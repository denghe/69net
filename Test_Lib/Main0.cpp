//#include <stack>
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <cassert>
//#include <algorithm>
//
//
//typedef unsigned short ushort;
//typedef unsigned int uint32, uint;
//#define SAFE_DELETE(p) if( p ) { delete p; p = nullptr; }
//#include "ZS/AStar.h"
//#include "ME/Map.h"
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
//    ushort x, y;
//};
//
//struct ASN : public AStarNode < ASN >
//{
//    ushort x, y;
//    ASN( ushort x, ushort  y ) : x( x ), y( y ) {}
//    uint32 key() override
//    {
//        return ( x << 16 ) | y;
//    }
//};
//
//Map<Item>* _map;
//
//struct AS : public AStar < ASN, Item >
//{
//    float h( ASN *a, ASN *b ) override
//    {
//        return sqrtf( ( a->x - b->x ) * ( a->x - b->x ) + ( a->y - b->y ) * ( a->y - b->y ) );
//    }
//
//    void try_tile( ushort x, ushort y, ASN *parent, float base_cost )
//    {
//        if( _map->At( x, y ).type == ItemType::Space )
//        {
//            this->add_successor( new ASN{ x, y }, parent, base_cost );
//        }
//    }
//    void expand_node( ASN *node ) override
//    {
//        try_tile( node->x, node->y - 1, node, 1.0f );
//        try_tile( node->x, node->y + 1, node, 1.0f );
//        try_tile( node->x - 1, node->y, node, 1.0f );
//        try_tile( node->x + 1, node->y, node, 1.0f );
//
//        try_tile( node->x - 1, node->y - 1, node, 1.41421356f );
//        try_tile( node->x - 1, node->y + 1, node, 1.41421356f );
//        try_tile( node->x + 1, node->y - 1, node, 1.41421356f );
//        try_tile( node->x + 1, node->y + 1, node, 1.41421356f );
//    }
//    WayPointList *build_path( ASN *node ) override
//    {
//        WayPointList *p = new WayPointList();
//        while( node )
//        {
//            p->push_front( Item{ ItemType::None, node->x, node->y } );
//            node = node->parent();
//        }
//        return p;
//    }
//};
//
//
//
//
//
//Map<Item>* Fill( int& aX, int& aY, int& bX, int& bY )
//{
//    ifstream f( "map2.txt" );
//    string tmp;
//    vector<string> ss;
//    while( getline( f, tmp ) )
//    {
//        ss.push_back( tmp );
//    }
//
//    auto result = new Map<Item>( (int)ss[ 0 ].size(), (int)ss.size() );
//
//    for( size_t y = 0; y < ss.size(); ++y )
//    {
//        auto& s = ss[ y ];
//        for( size_t x = 0; x < s.size(); ++x )
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
//
//
//
//void Dump( Map<Item>& m, int aX, int aY, int bX, int bY, list<Item>* way = nullptr )
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
//                    if( way )
//                    {
//                        bool found = false;
//                        for( auto& o : *way )
//                        {
//                            if( o.x == x && o.y == y )
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
//
//int main()
//{
//    int aX, aY, bX, bY;
//    _map = Fill( aX, aY, bX, bY );
//
//    cout << aX << ", " << aY << ", " << bX << ", " << bY << endl;
//    //Dump( *_map, aX, aY, bX, bY );
//
//    AS a;
//
//    xxx::Stopwatch sw;
//    for( int i = 0; i < 100; ++i )
//    {
//        a.init( new ASN( aX, aY ), new ASN( bX, bY ) );
//        while( a.search() == 0 );
//        auto way = a.detach_path();
//        delete way;
//        a.reset();
//    }
//    //Dump( *_map, aX, aY, bX, bY, way );
//    cout << sw.ElapsedMillseconds() << endl;
//
//    //for( auto p : *way )
//    //{
//    //    cout << p.x << ", " << p.y << endl;
//    //}
//
//    system( "pause" );
//    return 0;
//}
