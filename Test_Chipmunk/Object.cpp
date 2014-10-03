#include "Precompile.h"

bool Object::Update()
{
    if( xyInc.x == 0 && xyInc.y == 0 ) return false;
    
    size.w += 1;
    size.h += 1;

    if( size.w > 32 ) return false;


    if( offset.x <= size.w / 2 && xyInc.x < 0 )
    {
        xyInc.x = abs( xyInc.x );
    }
    else if( offset.x >= parent->size.w - size.w / 2 && xyInc.x > 0 )
    {
        xyInc.x = -abs( xyInc.x );
    }
    if( offset.y <= size.h / 2 && xyInc.y < 0 )
    {
        xyInc.y = abs( xyInc.y );
    }
    else if( offset.y >= parent->size.h - size.h / 2 && xyInc.y > 0 )
    {
        xyInc.y = -abs( xyInc.y );
    }

    offset.x += xyInc.x;
    offset.y += xyInc.y;
    dirty = true;

    return true;
}
