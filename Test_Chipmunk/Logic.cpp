#include "Logic.h"

Logic::Logic( Video* v, Input* i, Audio* a )
    : _v( v )
    , _i( i )
    , _a( a )
{
    // todo: init
}

Logic::~Logic()
{
}

void Logic::update()
{
    _i->update( _f );
    printf( "." );

    ++_f;
}
