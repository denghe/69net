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
            delete this;
        }
    }

    void Ref::AutoRelease()
    {
        autoReleasePool.push( this );
    }

    void Ref::ReleasePool()
    {
        for( int i = 0; i < autoReleasePool.size(); ++i )
        {
            autoReleasePool[ i ]->Release();
        }
        autoReleasePool.clear();
    }

    List<Ref*> Ref::autoReleasePool;
}
