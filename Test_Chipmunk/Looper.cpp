#include "Precompile.h"

Looper::Looper( Logic* logic )
    : _logic( logic )
{
    assert( logic );
}

Looper::~Looper()
{
}

void Looper::update( int durationTicks )
{
    int vTicks = 0;
    if( durationTicks > _logicFrameTicksLimit ) durationTicks = _logicFrameTicksLimit;
    if( !false )
    {
        _accumulatTicks += durationTicks;
        vTicks = _accumulatTicks;
        while( _accumulatTicks >= _logicFrameTicks )
        {
            _logic->update();
            _accumulatTicks -= _logicFrameTicks;
        }
        vTicks -= _accumulatTicks;
    }
    if( _logic->_v ) _logic->_v->update( vTicks );
}
