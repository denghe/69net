#include "All.h"

namespace xxx
{


    ScopeGuard::~ScopeGuard()
    {
        if( func ) func();
    }

    void ScopeGuard::RunAndCancel()
    {
        if( func )
        {
            func();
            func = nullptr;
        }
    }

    void ScopeGuard::Run()
    {
        if( func )
        {
            func();
        }
    }

    void ScopeGuard::Cancel()
    {
        func = nullptr;
    }

}