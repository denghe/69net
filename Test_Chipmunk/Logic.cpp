#include "Precompile.h"

/*

设计尺寸：    768 * 1024
格子划分：    12 * 16
每格尺寸：    64 * 64
细胞尺寸：    32 * 32 ( 即 1/4 格 )

*/





Logic::Logic( Video* v, Input* i, Audio* a )
    : _v( v )
    , _i( i )
    , _a( a )
{
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
