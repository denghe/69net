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
#ifdef USE_STL
        autoReleasePool.push_back( this );
#else
        autoReleasePool.push( this );
#endif
    }

    void Ref::ReleasePool()
    {
#ifdef USE_STL
        for( auto p : autoReleasePool )
        {
            p->Release();
#else
        for( int i = 0; i < autoReleasePool.size(); ++i )
        {
            autoReleasePool[ i ]->Release();
#endif
        }
        autoReleasePool.clear();
    }


#ifdef USE_STL
    vector<Ref*> Ref::autoReleasePool;
#else
    List<Ref*> Ref::autoReleasePool;
#endif

}
