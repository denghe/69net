#include "Precompile.h"

int main( void )
{
    Window glwindow;
    Input input;
    Audio audio;
    Game1 game1;
    Looper looper;

    printf( "%d ", sizeof( Ref ) );
    printf( "%d ", sizeof( Node ) );

    auto lastTP = system_clock::now();
    glwindow.Loop( [ &]
    {
        auto currTP = system_clock::now();
        looper.Update( (int)duration_cast<milliseconds>( currTP - lastTP ).count() );
        lastTP = currTP;
    } );

    return 0;
}
