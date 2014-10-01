#include "Precompile.h"

Ref::Ref()
{
}

Ref::~Ref()
{
}

void Ref::Retain()
{
    ++this->refCount;
}

void Ref::Release()
{
    if( ( --this->refCount ) <= 0 )
    {
        //if( deleter )
        //{
        //    deleter->operator()( this );
        //}
        //else
        //{
            delete this;
        //}
    }
}
