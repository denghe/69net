#include "Logic.h"

/*

设计尺寸：    24576 * 32768
格子划分：    12 * 16
每格尺寸：    2048 * 2048
细胞尺寸：    1024 * 1024 ( 即 1/4 格 )

物理尺寸：    768 * 1024    | 1536 * 2048
设计倍率：    32
细胞物理尺寸：32px          | 64px

输入精度：    384 * 512
设计倍率：    64


根据输入精度，查数组求角度和最小步进值:

struct xya
{
byte x;
byte y;
short a;
} data[ 384, 512 ];

// 步进值为 0 ~ 255
// 角度值为 0 ~ 65535 ( 对应 PI/2 )

*/

struct xya
{
    double x;
    double y;
    double a;
};
xya _xyas[ 384 ][ 512 ];
xya _xyas2[ 384 ][ 512 ];
//struct xya
//{
//    byte x;
//    byte y;
//    ushort a;
//} _xyas[ 384 ][ 512 ];


Logic::Logic( Video* v, Input* i, Audio* a )
    : _v( v )
    , _i( i )
    , _a( a )
{
    // todo: 填充 _xyas( 跨平台一致 check 可以填充后算校验和，看看是否一致 )
    double PI = 3.14159265358979323846264338327950288;
    double PI_2 = PI / 2;
    double PI2 = PI * 2;

    Stopwatch sw;
    for( int i = 0; i < 100; ++i )
    {
        for( int x = 0; x < _countof( _xyas ); ++x )
        {
            for( int y = 0; y < _countof( _xyas[ x ] ); ++y )
            {
                auto X = x * 64;
                auto Y = y * 64;
                auto D = sqrt( X*X + Y*Y );
                //auto frames = D / 64;           // 每 frame 至少移动的距离
                auto& xya = _xyas[ x ][ y ];
                xya.x = X * 64 / D;
                xya.y = Y * 64 / D; // / frames
                xya.a = atan2( (double)Y, (double)X ) / PI_2;// *0xFFFFu;
            }
        }
    }
    Cout( sw.elapsedMillseconds() );

    sw.reset();
    for( int i = 0; i < 100; ++i )
    {
        for( int x = 0; x < _countof( _xyas ); ++x )
        {
            for( int y = 0; y < _countof( _xyas[ x ] ); ++y )
            {
                auto& xya = _xyas[ x ][ y ];
                auto& xya2 = _xyas2[ x ][ y ];
                xya2 = xya;
            }
        }
    }
    Cout( sw.elapsedMillseconds() );
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
