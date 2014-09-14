#include "Lib/All.h"
#include <chipmunk.h>
#include "vld.h"

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

    return 0;
}
