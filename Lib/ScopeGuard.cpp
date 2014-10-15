#include "All.h"

namespace xxx
{


    ScopeGuard::~ScopeGuard()
    {
        if( _f ) _f();
    }

    void ScopeGuard::RunAndCancel()
    {
        if( _f )
        {
            _f();
            _f = nullptr;
        }
    }

    void ScopeGuard::Run()
    {
        if( _f )
        {
            _f();
        }
    }

    void ScopeGuard::Cancel()
    {
        _f = nullptr;
    }

}