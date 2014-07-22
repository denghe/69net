#include "All.h"

ScopeGuard::~ScopeGuard()
{
    if( _f ) _f();
}

void ScopeGuard::runAndCancel()
{
    if( _f )
    {
        _f();
        _f = nullptr;
    }
}

void ScopeGuard::run()
{
    if( _f )
    {
        _f();
    }
}

void ScopeGuard::cancel()
{
    _f = nullptr;
}
