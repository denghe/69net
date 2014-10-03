#include "Precompile.h"

namespace xxx
{
    void Looper::Update( int _durationTicks )
    {
        int drawTicks = 0;
        if( _durationTicks > logicFrameTicksLimit )
        {
            _durationTicks = logicFrameTicksLimit;
        }
        if( !false )    // end game flag ?
        {
            accumulatTicks += _durationTicks;
            drawTicks = accumulatTicks;
            while( accumulatTicks >= logicFrameTicks )
            {
                G::game->Update();
                Ref::ReleasePool();
                accumulatTicks -= logicFrameTicks;
            }
            drawTicks -= accumulatTicks;
        }
        if( G::scene )
        {
            G::scene->Draw( drawTicks );
            Ref::ReleasePool();
        }
    }
}
