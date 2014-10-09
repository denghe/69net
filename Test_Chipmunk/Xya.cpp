#include "Precompile.h"

Xya _xyas[ 768 * 2 ][ 1024 * 2 ];
float _xyam = 127;

void fillxya()
{
    float PI = 3.14159265359;
    float PI_2 = PI / 2;
    float PI2 = PI * 2;
    for( int x = 0; x < _countof( _xyas ); ++x )
    {
        for( int y = 0; y < _countof( _xyas[ x ] ); ++y )
        {
            auto& xya = _xyas[ x ][ y ];

            auto X = x - 768;
            auto Y = y - 1024;

            if( X == 0 && Y == 0 )
            {
                xya.x = xya.y = 1;
                xya.a = 0;
            }
            else
            {
                auto D = sqrt( X*X + Y*Y );
                xya.x = X / D * _xyam;
                xya.y = Y / D * _xyam;
                xya.a = -atan2( Y, X ) / PI2 * _xyam;
            }
        }
    }
}

Xya* getxya( Point const& a, Point const& b )
{
    return &_xyas[ (int)( b.x - a.x + 768 ) ][ (int)( b.y - a.y + 1024 ) ];
    // xya.x / _xyam * speed, xya.y / _xyam * speed
    // xya.a / _xyam * 360 + 90
}
