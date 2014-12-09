#include "Lib/All.h"

// todo: buf / debuf calc system
namespace xxx
{
    struct BufBaseCore
    {
        // 位于容器中时的位置 for 速删
        int bufIdx = 0;

        // bufs: 到达这个 ticks 将 Destroy
        // dots: 到达这个 ticks 将 Process
        int activeTicks = 0;

        // 读类型自增编号 for 定位子pool
        virtual int GetTypeId() = 0;

        // 不应放置逻辑相关代码
        BufBaseCore() = default;

        // 生命周期 -- 初始化/ 创建( 派生类自己实现 )
        // void Init( ... );

        // 生命周期 -- 执行
        inline virtual bool Process( int ticks ) = 0;

        // 生命周期 -- 销毁
        inline virtual void Destroy() {}

        // 不应放置逻辑相关代码
        virtual ~BufBaseCore() {};
    };

    template<typename T>
    struct BufBase : public BufBaseCore
    {
        static const AutoID<BufBaseCore> typeId;
        int GetTypeId() override { return typeId.value; }
    };
    template<typename T>
    const AutoID<BufBaseCore> BufBase<T>::typeId;

    struct BufPool
    {
        List<List<BufBaseCore*>> data;
        BufPool()
        {
            data.Resize( AutoID<BufBaseCore>::maxValue + 1 );
        }
        ~BufPool()
        {
            Clear();
        }
        template<typename T, typename ...PTS>
        T* Alloc( PTS&& ...ps )
        {
            T* rtv;
            auto& bufs = data[ T::typeId.value ];
            if( bufs.Size() )
                rtv = (T*)bufs.TopPop();
            else
                rtv = new T();
            rtv->Init( std::forward<PTS>( ps )... );
            return rtv;
        }
        inline void Free( BufBaseCore* buf )
        {
            buf->Destroy();
            data[ buf->GetTypeId() ].Push( buf );
        }
        template<typename T>
        inline void Prepare( int count )
        {
            auto& bufs = data[ T::typeId.value ];
            for( int i = 0; i < data.Size(); ++i )
            {
                bufs.Push( new T() );
            }
        }
        inline void Clear()
        {
            for( int i = 0; i < data.Size(); ++i )
            {
                auto& bufs = data[ i ];
                for( int j = 0; j < bufs.Size(); ++j )
                {
                    delete bufs[ j ];
                }
                bufs.Clear();
            }
        }
    };

    struct BufContainer
    {
        // 指向公用的 buf 池
        BufPool* pool;

        // 存 buf/dot 最小的死亡 ticks for 分时段批量跳过 Process 的过程
        int bufActiveTicks;
        int dotActiveTicks;

        // bufsDirty 为 true 时将 call 它再 遍历处理 bufs
        std::function<void()> BeforeBufProcess;

        // bufsDirty 为 true 时 遍历处理 bufs 后将 call 它
        std::function<void()> AfterBufProcess;

        // 持续生效的，以改 Properties buf 系数为主的 buf
        // 当成员构成( 有增删 )变化( lazyDirty 为 true )时，遍历处理
        List<BufBaseCore*> bufs;

        // 周期性生效的，以 call 宿主函数( 形成指令流, Process 结束后依次执行 ) 为主的( dot 系 )
        // 每次 Process 都遍历处理
        List<BufBaseCore*> dots;

        BufContainer( BufPool* pool )
            : pool( pool )
            , bufActiveTicks( INT_MAX )
            , dotActiveTicks( INT_MAX )
        {
        }
        ~BufContainer()
        {
            Clear();
        }

        template<typename T, typename ...PTS>
        inline T* CreateBuf( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = bufs.Size();
            bufs.Push( rtv );
            bufActiveTicks = 0;     // 相当于标脏，全面重算
            return rtv;
        }

        template<typename T, typename ...PTS>
        inline T* CreateDot( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = dots.Size();
            dots.Push( rtv );
            if( rtv->activeTicks < dotActiveTicks )
            {
                dotActiveTicks = rtv->activeTicks;
            }
            return rtv;
        }

        inline void DestroyBuf( BufBaseCore* buf )
        {
            bufs.Top()->bufIdx = buf->bufIdx;
            bufs.EraseFast( buf->bufIdx );
            pool->Free( buf );
            bufActiveTicks = 0;     // 相当于标脏，全面重算
        }

        inline void DestroyDot( BufBaseCore* buf )
        {
            dots.Top()->bufIdx = buf->bufIdx;
            dots.EraseFast( buf->bufIdx );
            pool->Free( buf );
        }

        inline void Clear()
        {
            // 有可能存在清不完的情况，比如 Destroy 的时候又创建了新的
            while( bufs.Size() )
            {
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    DestroyBuf( bufs[ i ] );
                }
            }
            while( dots.Size() )
            {
                for( int i = dots.Size() - 1; i >= 0; --i )
                {
                    DestroyDot( dots[ i ] );
                }
            }
            bufActiveTicks = INT_MAX;
            dotActiveTicks = INT_MAX;
        }

        inline bool Process( int ticks = 0 )
        {
            if( bufActiveTicks <= ticks )
            {
                bufActiveTicks = INT_MAX;
                if( BeforeBufProcess )
                {
                    BeforeBufProcess();
                }
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    auto& o = bufs[ i ];
                    if( !o->Process( ticks ) || o->activeTicks <= ticks )
                    {
                        DestroyBuf( o );
                    }
                    else
                    {
                        // 只扫出还活着的 buf 的下次执行 ticks
                        if( o->activeTicks < bufActiveTicks )
                        {
                            bufActiveTicks = o->activeTicks;
                        }
                    }
                }
                if( AfterBufProcess )
                {
                    AfterBufProcess();
                }
            }

            if( dotActiveTicks <= ticks )
            {
                dotActiveTicks = INT_MAX;
                for( int i = dots.Size() - 1; i >= 0; --i )
                {
                    auto& o = dots[ i ];
                    // 试扫出下次执行的最小间隔
                    // 如果被杀掉的刚好是最小间隔 也没关系 不严格, 到那个 ticks 会再扫出正确值
                    // 感觉略冗余，但省掉了 存放 或立即扫出 最小间隔的消耗
                    if( o->activeTicks < dotActiveTicks )
                    {
                        dotActiveTicks = o->activeTicks;
                    }
                    if( o->activeTicks <= ticks )
                    {
                        if( !o->Process( ticks ) )
                        {
                            DestroyDot( o );
                        }
                    }
                }
                // 当前算法：每到一个执行点，就硬扫，适合对象数较少的情况( 阈值不明确，猜测是 10 个以内 )。
                // 如果元素再多点，则需要在扫的同时将下次执行点的 dot 放队列了
                // 如果又继续扫到更小的间隔，则将先前的队列中的元素清了，再放入队列
            }

            return true;
        }
    };

    /*
    sample:
    */
}

using namespace std;
using namespace xxx;


struct Buf_MaxHP_Enhance100Percent;
struct Buf_MaxHP_Enhance10Point;
struct Dot_HP_Recover1PointPerTicks;


struct Foo
{
    // 原型系列( 理论上讲存在一个原型类 )
    int ori_maxHP;                  // 原始最大血量
    float ori_maxHPMultiple;        // 原始最大血量百分比 buf 值
    int ori_maxHPIncrease;          // 原始最大血量点数 buf 值
    int ori_HP;

    // 当前系列( 原始值与 buf 系数 )
    int cur_maxHP;
    float cur_maxHPMultiple;
    int cur_maxHPIncrease;
    int cur_HP;

    // buf 后系列
    int buf_maxHP;

    // Process 后需要依次执行的函数, 执行完后 clear
    List<std::function<void()>> funcs;

    // 初始化原型数据
    inline void InitOriginal()
    {
        ori_maxHP = 100;
        ori_maxHPMultiple = 1;
        ori_maxHPIncrease = 0;
        ori_HP = 50;
    }

    inline void InitCurrent()
    {
        cur_maxHP = ori_maxHP;
        cur_HP = ori_HP;
    }

    // 为计算 buf 而重置 buf 系数
    inline void ResetBufProperties()
    {
        cur_maxHPMultiple = ori_maxHPMultiple;
        cur_maxHPIncrease = ori_maxHPIncrease;
    }

    // 算受 buf 影响的属性( 优先级公式在此 )
    inline void CalcBufProperties()
    {
        buf_maxHP = int( ori_maxHP * cur_maxHPMultiple ) + cur_maxHPIncrease;
    }

    inline void CallFuncs()
    {
        for( int i = 0; i < funcs.Size(); ++i )
        {
            funcs[ i ]();
        }
        funcs.Clear();
    }

    // 综合初始化
    inline void Init()
    {
        InitOriginal();
        InitCurrent();
        ResetBufProperties();
    }

    BufContainer bc;
    Foo( BufPool* bp )
        : bc( bp )
    {
        bc.BeforeBufProcess = [ this ] { ResetBufProperties(); };
        bc.AfterBufProcess = [ this ] { CalcBufProperties(); };
        Init();
    }

    inline void Process( int ticks )
    {
        bc.Process( ticks );
    }

    void CreateBuf_最大血量加10点( int ticks, int life );
    void CreateBuf_最大血量加百分之100( int ticks, int life );
    void CreateDot_血量恢复1点( int ticks, int cd );
};

struct Buf_MaxHP_Enhance100Percent : BufBase<Buf_MaxHP_Enhance100Percent>
{
    Foo* owner;
    inline void Init( Foo* owner, int ticks, int life )
    {
        this->owner = owner;
        this->activeTicks = ticks + life;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_maxHPMultiple += 1;
        return true;
    }
};

struct Buf_MaxHP_Enhance10Point : BufBase<Buf_MaxHP_Enhance10Point>
{
    Foo* owner;
    inline void Init( Foo* owner, int ticks, int life )
    {
        this->owner = owner;
        this->activeTicks = ticks + life;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_maxHPIncrease += 10;
        return true;
    }
};

struct Dot_HP_Recover1PointPerTicks : BufBase<Dot_HP_Recover1PointPerTicks>
{
    Foo* owner;
    int cd;
    inline void Init( Foo* owner, int ticks, int cd )
    {
        this->owner = owner;
        this->activeTicks = ticks + cd;
        this->cd = cd;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_HP += 1;
        if( owner->cur_HP > owner->buf_maxHP )
        {
            owner->cur_HP = owner->buf_maxHP;
        }
        activeTicks = ticks + cd;
        //Cout( "owner->cur_HP = ", owner->cur_HP );
        return true;
    }
};

void Foo::CreateBuf_最大血量加10点( int ticks, int life )
{
    bc.CreateBuf<Buf_MaxHP_Enhance10Point>( this, ticks, life );
}
void Foo::CreateBuf_最大血量加百分之100( int ticks, int life )
{
    bc.CreateBuf<Buf_MaxHP_Enhance100Percent>( this, ticks, life );
}
void Foo::CreateDot_血量恢复1点( int ticks, int cd )
{
    bc.CreateDot<Dot_HP_Recover1PointPerTicks>( this, ticks, cd );
}


typedef xxx::Singleton<BufPool> FooBufPool;
int main()
{
    FooBufPool::InitInstance();

    Stopwatch sw;
    //for( int j = 0; j < 10000000; ++j )
    {
        Foo foo( FooBufPool::GetInstance() );

        // 仿一个帧步进值
        int ticks = 123;

        // 来几个 buf( 加 血上限 百分比和点数, 存活时长不同 )
        foo.CreateBuf_最大血量加10点( ticks, 10 );
        foo.CreateBuf_最大血量加百分之100( ticks, 9 );
        foo.CreateBuf_最大血量加10点( ticks, 8 );
        foo.CreateBuf_最大血量加百分之100( ticks, 7 );
        foo.CreateBuf_最大血量加10点( ticks, 6 );

        // 来几个 dot( 不断的恢复血, 每跳间隔时长不同 )
        foo.CreateDot_血量恢复1点( ticks, 1 );
        foo.CreateDot_血量恢复1点( ticks, 2 );
        foo.CreateDot_血量恢复1点( ticks, 3 );

        for( int i = 0; i < 11; ++i )
        {
            ticks++;
            foo.Process( ticks );
            Cout( "buf_maxHP = ", foo.buf_maxHP, ", cur_HP = ", foo.cur_HP );
        }
    }
    Cout( "ms = ", sw.ElapsedMillseconds() );

    Cout( "FooBufPool::GetInstance()->data[ 0 ].Size() = ", FooBufPool::GetInstance()->data[ 0 ].Size() );
    Cout( "FooBufPool::GetInstance()->data[ 1 ].Size() = ", FooBufPool::GetInstance()->data[ 1 ].Size() );
    Cout( "FooBufPool::GetInstance()->data[ 2 ].Size() = ", FooBufPool::GetInstance()->data[ 2 ].Size() );

    system( "pause" );
    return 0;
}
