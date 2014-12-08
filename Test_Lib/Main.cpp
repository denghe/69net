#include "Lib/All.h"

// todo: buf / debuf calc system
namespace xxx
{
    struct BufBaseCore
    {
        // 位于容器中时的位置 for 速删
        int bufIdx = 0;

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

        // todo: 将生命周期值暴露出来 以便 BufContainer Destroy
        // 
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

        // Create/DestroyLazy 后变 true 以令 Process 时 遍历处理 bufs
        bool bufsDirty;

        // bufsDirty 为 true 时将 call 它再 遍历处理 bufs
        std::function<void()> ResetBufProps;

        // bufsDirty 为 true 时 遍历处理 bufs 后将 call 它
        std::function<void()> CalcBufProps;

        // 持续生效的，以改 Properties buf 系数为主的 buf
        // 当成员构成( 有增删 )变化( lazyDirty 为 true )时，遍历处理
        List<BufBaseCore*> bufs;

        // 周期性生效的，以 call 宿主函数( 形成指令流, Process 结束后依次执行 ) 为主的( dot 系 )
        // 每次 Process 都遍历处理
        List<BufBaseCore*> dots;

        BufContainer( BufPool* pool )
            : pool( pool )
            , bufsDirty( false )
        {
        }

        template<typename T, typename ...PTS>
        inline T* CreateBuf( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = bufs.Size();
            bufs.Push( rtv );
            bufsDirty = true;
            return rtv;
        }

        template<typename T, typename ...PTS>
        inline T* CreateDot( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = dots.Size();
            dots.Push( rtv );
            return rtv;
        }

        inline void DestroyBuf( BufBaseCore* buf )
        {
            bufs.Top()->bufIdx = buf->bufIdx;
            bufs.EraseFast( buf->bufIdx );
            pool->Free( buf );
            bufsDirty = true;
        }

        inline void DestroyDot( BufBaseCore* buf )
        {
            dots.Top()->bufIdx = buf->bufIdx;
            dots.EraseFast( buf->bufIdx );
            pool->Free( buf );
        }

        inline void Clear()
        {
            for( int i = bufs.Size() - 1; i >= 0; --i )
            {
                DestroyBuf( bufs[ i ] );
            }
            for( int i = dots.Size() - 1; i >= 0; --i )
            {
                DestroyDot( dots[ i ] );
            }
            bufsDirty = true;
        }

        inline bool Process( int ticks = 0 )
        {
            if( bufsDirty )
            {
                bufsDirty = false;  // 放这里改，万一 Process 过程中又有改变 bufs, 它将正确的变 true
                if( ResetBufProps )
                {
                    ResetBufProps();
                }
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    auto& o = bufs[ i ];
                    if( !o->Process( ticks ) )
                    {
                        DestroyBuf( o );
                    }
                }
                if( CalcBufProps )
                {
                    CalcBufProps();
                }
            }

            for( int i = dots.Size() - 1; i >= 0; --i )
            {
                auto& o = dots[ i ];
                if( !o->Process( ticks ) )
                {
                    DestroyDot( o );
                }
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
        bc.ResetBufProps = [ this ] { ResetBufProperties(); };
        bc.CalcBufProps = [ this ] { CalcBufProperties(); };
        Init();
    }
};

struct Buf_maxHP_Enhance100Percent : BufBase<Buf_maxHP_Enhance100Percent>
{
    Foo* owner;
    inline void Init( Foo* owner )
    {
        this->owner = owner;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_maxHPMultiple += 1;
        return true;
    }
};

struct Buf_maxHP_Enhance10Point : BufBase<Buf_maxHP_Enhance10Point>
{
    Foo* owner;
    inline void Init( Foo* owner )
    {
        this->owner = owner;
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
    inline void Init( Foo* owner )
    {
        this->owner = owner;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_HP += 1;
        if( owner->cur_HP > owner->buf_maxHP )
        {
            owner->cur_HP = owner->buf_maxHP;
        }
        return true;
    }
};


int main()
{
    BufPool p;
    Foo foo( &p );

    // 来几个 buf( 加 血上限 百分比和点数 )
    foo.bc.CreateBuf<Buf_maxHP_Enhance10Point>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance100Percent>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance10Point>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance100Percent>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance10Point>( &foo );

    // 来几个 dot( 每周期不断的恢复血 )
    foo.bc.CreateDot<Dot_HP_Recover1PointPerTicks>( &foo );
    foo.bc.CreateDot<Dot_HP_Recover1PointPerTicks>( &foo );
    foo.bc.CreateDot<Dot_HP_Recover1PointPerTicks>( &foo );

    Stopwatch sw;
    for( int i = 0; i < 9999999; ++i )
    {
        foo.bc.Process();
    }
    Cout( "ms = ", sw.ElapsedMillseconds() );

    // shoud be 330, 330
    Cout( "buf_maxHP = ", foo.buf_maxHP, ", cur_HP = ", foo.cur_HP );

    //foo.CallFuncs();

    system( "pause" );
    return 0;
}
