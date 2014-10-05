#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <cassert>
#include "Lib/All.h"
using namespace std;

struct Point
{
    int x, y;
};
struct Size
{
    int w, h;
};
struct Box
{
    Point c;
    Size r;
};

bool foo( Box const& a, Box const& b )
{
    uint r;
    r = a.r.w + b.r.w;
    if( (uint)( a.c.x - b.c.x + r ) > r + r ) return false;
    r = a.r.h + b.r.h;
    if( (uint)( a.c.y - b.c.y + r ) > r + r ) return false;
    return true;
}

int main()
{
    Box b = { { 0, 0 }, { 5, 5 } };
    Box a = { { 10, 10 }, { 5, 5 } };

    auto rtv = foo( a, b );

    system( "pause" );
    return 0;
}

