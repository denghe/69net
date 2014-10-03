#include "Precompile.h"

void Object::Update()
{
    if( xyInc.x == 0 && xyInc.y == 0 ) return;

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
}
