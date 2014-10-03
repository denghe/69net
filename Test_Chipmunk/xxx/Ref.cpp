#include "Precompile.h"

namespace xxx
{
    Ref::Ref()
    {
    }

    Ref::~Ref()
    {
    }

    void Ref::Retain()
    {
        ++refCount;
    }

    void Ref::Release()
    {
        if( ( --refCount ) <= 0 )
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
}
