//#include "vld.h"
#include "Lib/All.h"
#include "Platform.h"
#include <chipmunk.h>

using namespace std;

// 随便写个对象池 for chipmunk
template<typename T>
struct cpPool : Pool
{
    cpPool()
    {
        Pool::init( sizeof( T ) );
    }
    inline T* alloc()
    {
#if __DEBUG
        ++_counter;
#endif
        return (T*)Pool::alloc();
    }
    inline void free( T* p )
    {
#if __DEBUG
        --_counter;
#endif
        Pool::free( p );
    }
#if __DEBUG
    int _counter = 0;
#endif
};


int main( void )
{
    // 对象池
    cpPool<cpSpace> spacePool;
    cpPool<cpShape> shapePool;
    cpPool<cpCircleShape> circleShapePool;
    cpPool<cpSegmentShape> segmentShapePool;
    cpPool<cpBody> bodyPool;


    {
        // 创建一个空白的物理世界

        auto space = spacePool.alloc();
        cpSpaceInit( space );
        ScopeGuard sgSpace( [ &]
        {
            cpSpaceDestroy( space );
            spacePool.free( space );
        } );
        cpSpaceSetIterations( space, 1 );               // 设计算迭代次数
        cpSpaceSetGravity( space, cpv( 0, -100 ) );     // 设置引力


        // 为地面创建一个静态线段形状
        // 我们稍微倾斜线段以便球可以滚下去
        // 我们将形状关联到space的默认静态刚体上，告诉Chipmunk该形状是不可移动的

        auto ground_ = segmentShapePool.alloc();
        auto ground = (cpShape*)ground_;
        cpSegmentShapeInit( ground_, space->staticBody, cpv( -20, 5 ), cpv( 20, -5 ), 0 );
        ScopeGuard sgGround( [ &]
        {
            cpSpaceRemoveShape( space, ground );
            cpShapeDestroy( ground );
            segmentShapePool.free( ground_ );
        } );
        cpShapeSetFriction( ground, 1 );
        cpSpaceAddShape( space, ground );


        // 现在让我们来构建一个球体落到线上并滚下去
        // 首先我们需要构建一个 cpBody 来容纳对象的物理属性
        // 包括对象的质量、位置、速度、角度等
        // 然后我们将碰撞形状关联到cpBody上以给它一个尺寸和形状

        cpFloat radius = 5;
        cpFloat mass = 1;

        // 转动惯量就像质量对于旋转一样
        // 使用 cpMomentFor*() 来近似计算它
        cpFloat moment = cpMomentForCircle( mass, 0, radius, cpvzero );


        auto ballBody = bodyPool.alloc();
        cpBodyInit( ballBody, mass, moment );
        cpSpaceAddBody( space, ballBody );
        ScopeGuard sgBallBody( [ &]
        {
            cpSpaceRemoveBody( space, ballBody );
            cpBodyDestroy( ballBody );
            bodyPool.free( ballBody );
        } );

        cpBodySetPos( ballBody, cpv( 0, 15 ) ); // 会自动调用 cpBodyActivate 令 sleep 或 idle 对象活动起来( 参与检测 )

        // cpGroup: 碰撞组, 理论上讲需要用到，比如玩家发射的子弹之间不判定，它们为同一 group


        // 现在我们会球体创建碰撞形状
        // 你可以为同一个刚体创建多个碰撞形状
        // 它们将会附着关联到刚体上并移动更随

        auto ballShape_ = circleShapePool.alloc();
        auto ballShape = (cpShape*)ballShape_;
        cpCircleShapeInit( ballShape_, ballBody, radius, cpvzero );
        cpSpaceAddShape( space, ballShape );
        ScopeGuard sgBallShape( [ &]
        {
            cpSpaceRemoveShape( space, ballShape );
            cpShapeDestroy( ballShape );
            circleShapePool.free( ballShape_ );
        } );

        cpShapeSetFriction( ballShape, 0.7 );           // 摩擦系数

        // 现在一切都建立起来了，我们通过称作时间步的小幅度时间增量来步进模拟空间中的所有物体
        // *高度*推荐使用固定长的时间步
        cpFloat timeStep = 1.0 / 60.0;
        for( cpFloat time = 0; time < 2; time += timeStep )
        {
            cpVect pos = cpBodyGetPos( ballBody );
            cpVect vel = cpBodyGetVel( ballBody );
            printf(
                "Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
                time, pos.x, pos.y, vel.x, vel.y
                );

            cpSpaceStep( space, timeStep );
        }

        //// 清理我们的对象并退出
        //cpShapeFree( ballShape );
        //cpSpaceRemoveBody( space, ballBody );
        //cpBodyFree( ballBody );
        //cpShapeFree( ground_ );
        ////cpSpaceFree( space );
    }

#if __DEBUG
    Cout( spacePool._counter, ", ", shapePool._counter, ", ", circleShapePool._counter, ", ", segmentShapePool._counter, ", ", bodyPool._counter, ", " );
#endif







    Platform::GLWindow w;
    w.init( L"test", 256, 256 );
    w.setVsync( false );

    // 每 0.2 秒输出一次总 counter 及 每秒 count
    std::chrono::milliseconds refreshDuration( 200 );
    std::chrono::seconds durationSec1( 1 );
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    auto firstTime = std::chrono::system_clock::now();
    auto cout = [ &]( long long val )
    {
        auto now = std::chrono::system_clock::now();
        if( now - lastTime >= refreshDuration )
        {
            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
            if( !elapsedSeconds ) elapsedSeconds = 1;
            printf( "%lld ", val / elapsedSeconds );
            lastTime = now;
        }
    };
    long long counter = 0;



    //glClearColor( 0, 0, 0, 0 );
    //glMatrixMode( GL_PROJECTION );
    //glLoadIdentity();
    //glOrtho( 0, 1, 0, 1, -1, 1 );

    float theta = 0;

    w.run( [ &]
    {
        //glClear( GL_COLOR_BUFFER_BIT );
        //glColor3f( 1, 1, 1 );
        //glBegin( GL_POLYGON );
        //{
        //    glVertex3f( 0.25, 0.25, 0.0 );
        //    glVertex3f( 0.75, 0.25, 0.0 );
        //    glVertex3f( 0.75, 0.75, 0.0 );
        //    glVertex3f( 0.25, 0.75, 0.0 );
        //}
        //glEnd();

        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        glPushMatrix();
        glRotatef( theta, 0.0f, 0.0f, 1.0f );
        glBegin( GL_TRIANGLES );
        glColor3f( 1.0f, 0.0f, 0.0f ); glVertex2f( 0.0f, 1.0f );
        glColor3f( 0.0f, 1.0f, 0.0f ); glVertex2f( 0.87f, -0.5f );
        glColor3f( 0.0f, 0.0f, 1.0f ); glVertex2f( -0.87f, -0.5f );
        glEnd();
        glPopMatrix();

        theta += 1.0f;

        cout( ++counter );
        //std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        //printf( "." );
    } );




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

    return 0;
}
