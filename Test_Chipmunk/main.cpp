#include "Precompile.h"
using namespace std::chrono;
using namespace xxx;
int main( void )
{
    Window w;
    Game3 g;
    Looper l;

    auto lastTime = system_clock::now();
    w.Loop( [ &]
    {
        auto currTime = system_clock::now();
        l.Update( (int)duration_cast<milliseconds>( currTime - lastTime ).count() );
        lastTime = currTime;
    } );

    return 0;
}
