#include "Precompile.h"


MarginBox::MarginBox()
{
    this->anchor = { 0, 0 };
}

void MarginBox::Draw( int durationTicks )
{
    if( this->dirty || this->parent->dirty )
    {
        this->size =
        {
            this->parent->size.width - this->margin.left - this->margin.right,
            this->parent->size.height - this->margin.top - this->margin.bottom,
        };
        this->offset = { this->margin.left, this->margin.bottom };
    }
    Box::Draw( durationTicks );
}
