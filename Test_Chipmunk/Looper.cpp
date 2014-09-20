#include "Looper.h"
using namespace std;

Looper::Looper( Logic* logic, Render* render )
    : _logic( logic )
    , _render( render )
{
    assert( logic );
}

Looper::~Looper()
{
}

void Looper::update( int durationTicks )
{
    if( durationTicks > _logicFrameTickLimit ) durationTicks = _logicFrameTickLimit;
    if( !false )
    {
        _accumulatTicks += durationTicks;
        _renderTicks = _accumulatTicks;
        while( _accumulatTicks >= _logicFrameTicks )
        {
            _logic->update();
            _accumulatTicks -= _logicFrameTicks;
        }
        _renderTicks -= _accumulatTicks;
    }
    if( _render )_render->update( _renderTicks );
    _renderTicks = 0;
}
