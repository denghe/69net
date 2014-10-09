#include "Precompile.h"


MarginBox::MarginBox()
{
    anchor = { 0, 0 };
}

void MarginBox::Drawing( int _durationTicks )
{
    if( dirty || parent->dirty )
    {
        size =
        {
            parent->size.w - margin.left - margin.right,
            parent->size.h - margin.top - margin.bottom,
        };
        pos = { margin.left, margin.bottom };
    }
}
