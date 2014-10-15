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
        autoReleasePool.Push( this );
    }

    void Ref::ReleasePool()
    {
        for( int i = 0; i < autoReleasePool.Size(); ++i )
        {
            autoReleasePool[ i ]->Release();
        }
        autoReleasePool.Clear();
    }

    List<Ref*> Ref::autoReleasePool;
}
