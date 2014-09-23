#include "Lib/All.h"
#include "GLWindow.h"
#include "Video.h"
#include "Input.h"
#include "Audio.h"
#include "Logic.h"
#include "Looper.h"

using namespace std;
using namespace std::chrono;

int main( void )
{
    GLWindow w;
    Video v( &w );
    Input i;
    Audio a;
    Logic logic( &v, &i, &a );
    Looper looper( &logic );

    auto lastTP = system_clock::now();
    w.loop( [ &]
    {
        auto currTP = system_clock::now();
        looper.update( (int)duration_cast<milliseconds>( currTP - lastTP ).count() );
        lastTP = currTP;
    } );

    return 0;
}




//// 随便写个对象池 for chipmunk
//template<typename T>
//struct cpPool : Pool
//{
//    cpPool()
//    {
//        Pool::init( sizeof( T ) );
//    }
//    inline T* alloc()
//    {
//#if __DEBUG
//        ++_counter;
//#endif
//        return (T*)Pool::alloc();
//    }
//    inline void free( T* p )
//    {
//#if __DEBUG
//        --_counter;
//#endif
//        Pool::free( p );
//    }
//#if __DEBUG
//    int _counter = 0;
//#endif
//};
//// 对象池
//cpPool<cpSpace> spacePool;
//cpPool<cpShape> shapePool;
//cpPool<cpCircleShape> circleShapePool;
//cpPool<cpSegmentShape> segmentShapePool;
//cpPool<cpBody> bodyPool;



////glClearColor( 0, 0, 0, 0 );
////glMatrixMode( GL_PROJECTION );
////glLoadIdentity();
////glOrtho( 0, 1, 0, 1, -1, 1 );

//float theta = 0;

//w.run( [ &]
//{
//    //glClear( GL_COLOR_BUFFER_BIT );
//    //glColor3f( 1, 1, 1 );
//    //glBegin( GL_POLYGON );
//    //{
//    //    glVertex3f( 0.25, 0.25, 0.0 );
//    //    glVertex3f( 0.75, 0.25, 0.0 );
//    //    glVertex3f( 0.75, 0.75, 0.0 );
//    //    glVertex3f( 0.25, 0.75, 0.0 );
//    //}
//    //glEnd();

//    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//    glClear( GL_COLOR_BUFFER_BIT );

//    glPushMatrix();
//    glRotatef( theta, 0.0f, 0.0f, 1.0f );
//    glBegin( GL_TRIANGLES );
//    glColor3f( 1.0f, 0.0f, 0.0f ); glVertex2f( 0.0f, 1.0f );
//    glColor3f( 0.0f, 1.0f, 0.0f ); glVertex2f( 0.87f, -0.5f );
//    glColor3f( 0.0f, 0.0f, 1.0f ); glVertex2f( -0.87f, -0.5f );
//    glEnd();
//    glPopMatrix();

//    theta += 1.0f;

//    cout( ++counter );
//    //std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//    //printf( "." );
//} );



/*
// logic + render 循环伪代码

// double 时长版:

单步时间 = 1 / 每秒次数;
单步时间限制 = 单步时间 * 1;              // * 1: 不跳帧，如果机能不行，游戏会被拖慢，但不会走样

起始时间 = 0;
积蓄时间 = 0;
上次时间 = 取当前时间();

while( !退出 )
{
当前时间 = 取当前时间();
经过时间 = 当前时间 - 上次时间;
if( 经过时间 > 单步时间限制 ) 经过时间 = 单步时间限制;
上次时间 = 当前时间;

积蓄时间 += 经过时间;
while( 积蓄时间 >= 单步时间 )
{
逻辑处理( 起始时间, 单步时间 );
起始时间 += 单步时间;
积蓄时间 -= 单步时间;
}

绘制();
}


// 步进版:


逻辑帧时长 = 1 / 每秒次数;         // 时长可能是整型. 比如 50ms 啥的
逻辑帧时长限制 = 逻辑帧时长 * 1;   // * 1: 不跳帧，如果机能不行，游戏会被拖慢，但不会异常

积蓄时间 = 0;
上次时间 = 取当前时间();

逻辑帧计数 = 0;

while( !退出 )                    // 这段代码有可能是经由 engine schedule 之类来 call
{
当前时间 = 取当前时间();
经过时间 = 当前时间 - 上次时间;
if( 经过时间 > 单步时间限制 ) 经过时间 = 单步时间限制;
上次时间 = 当前时间;

if( 暂停 ) 跳转到绘制();      // 经由 schedule call 就是 return 了

积蓄时间 += 经过时间;
while( 积蓄时间 >= 单步时间 )
{
逻辑处理( 逻辑帧计数++ );
积蓄时间 -= 单步时间;
}

绘制();                       // 经由 schedule call 就没有这个函数调用了
}


*/



//// 每 0.2 秒输出一次总 counter 及 每秒 count
//std::chrono::milliseconds refreshDuration( 200 );
//std::chrono::seconds durationSec1( 1 );
//std::chrono::time_point<std::chrono::system_clock> lastTime;
//auto firstTime = std::chrono::system_clock::now();
//auto cout = [ &]( long long val )
//{
//    auto now = std::chrono::system_clock::now();
//    if( now - lastTime >= refreshDuration )
//    {
//        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
//        if( !elapsedSeconds ) elapsedSeconds = 1;
//        printf( "%lld ", val / elapsedSeconds );
//        lastTime = now;
//    }
//};
//long long counter = 0;
