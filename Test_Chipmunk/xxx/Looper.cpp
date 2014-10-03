#include "Precompile.h"

namespace xxx
{
    void Looper::Update( int durationTicks )
    {
        int drawTicks = 0;
        if( durationTicks > this->logicFrameTicksLimit )
        {
            durationTicks = this->logicFrameTicksLimit;
        }
        if( !false )
        {
            this->accumulatTicks += durationTicks;
            drawTicks = this->accumulatTicks;
            while( this->accumulatTicks >= this->logicFrameTicks )
            {
                G::logic->Update();
                this->accumulatTicks -= this->logicFrameTicks;
            }
            drawTicks -= this->accumulatTicks;
        }
        if( G::scene )
        {
            G::scene->Draw( drawTicks );
        }
    }
}