#include <stack>
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

#include "AStar.h"

using namespace std;


struct Pos
{
    int x, y;
};

enum class ItemType : short
{
    None,
    Wall,
    Space,
};

struct Item
{
    ItemType type;
    int x, y;
    bool IsWalkable( Item const& o )
    {
        return o.type != ItemType::None;
    }
};

vector<string> mapConfig = {
    "%%%%%%%%%%%%%%%%%%%",
    "%######%%%########%",
    "%# @  #%%%#      #%",
    "%#    #####      #%",
    "%#    #          #%",
    "%#    #   #      #%",
    "%#        #    * #%",
    "%#        #      #%",
    "%#################%",
    "%%%%%%%%%%%%%%%%%%%",
};
Map<Item> mapData( (int)mapConfig[ 0 ].size(), (int)mapConfig.size() );


int main()
{
    Pos posBegin, posEnd;

    // fill
    for( int y = 0; y < mapData.h; ++y )
    {
        auto& mapRow = mapConfig[ y ];
        for( int x = 0; x < mapData.w; ++x )
        {
            auto& c = mapRow[ x ];
            auto& item = mapData.At( x, y );
            switch( c )
            {
            case '%':
                item = { ItemType::None, x, y };
                break;
            case '#':
                item = { ItemType::Wall, x, y };
                break;
            case ' ':
                item = { ItemType::Space, x, y };
                break;
            case '@':
                item = { ItemType::Space, x, y };
                posBegin = Pos{ x, y };
                break;
            case '*':
                item = { ItemType::Space, x, y };
                posEnd = Pos{ x, y };
                break;
            }
        }
    }

    // dump for auth
    for( int y = 0; y < mapData.h; ++y )
    {
        for( int x = 0; x < mapData.w; ++x )
        {
            auto& item = mapData.At( x, y );
            switch( item.type )
            {
            case ItemType::None:
                cout << "%";
                break;
            case ItemType::Space:
                if( posBegin.x == x && posBegin.y == y )
                {
                    cout << "@";
                }
                else if( posEnd.x == x && posEnd.y == y )
                {
                    cout << "*";
                }
                else
                {
                    cout << " ";
                }
                break;
            case ItemType::Wall:
                cout << "#";
                break;
            default:
                assert( false );
            }
        }
        cout << endl;
    }



    // todo AStar
    AStar<Item> astar( &mapData );
    std::vector<Item*> resultContainer;
    if( astar.Search( posBegin.x, posBegin.y, posEnd.x, posEnd.y, resultContainer ) )
    {
        for( auto& item : resultContainer )
        {
            cout << "x = " << item->x << ", y = " << item->y << endl;
        }
    }


    system( "pause" );
    return 0;
}
