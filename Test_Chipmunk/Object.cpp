#include "Precompile.h"

bool Object::Update()
{
    if( xyInc.x == 0 && xyInc.y == 0 ) return false;
    
    size.w += 0.1f;
    size.h += 0.1f;

    if( size.w > 32 ) return false;


    if( pos.x <= size.w / 2 && xyInc.x < 0 )
    {
        xyInc.x = abs( xyInc.x );
    }
    else if( pos.x >= parent->size.w - size.w / 2 && xyInc.x > 0 )
    {
        xyInc.x = -abs( xyInc.x );
    }
    if( pos.y <= size.h / 2 && xyInc.y < 0 )
    {
        xyInc.y = abs( xyInc.y );
    }
    else if( pos.y >= parent->size.h - size.h / 2 && xyInc.y > 0 )
    {
        xyInc.y = -abs( xyInc.y );
    }

    pos.x += xyInc.x;
    pos.y += xyInc.y;
    dirty = true;

    return true;
}
