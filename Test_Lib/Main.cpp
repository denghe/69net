#include "Lib/All.h"
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
    Foo( BufPool<BufBase>* bp )
        : bc( bp )
    {
        bc.BeforeBufProcess = [ this ] { ResetBufProperties(); };
        bc.AfterBufProcess = [ this ] { CalcBufProperties(); };
        Init();
    }

    inline void Process( int ticks )
    {
        bc.Process( ticks );
        // todo: foreach call funcs
    }

    void CreateBuf_最大血量加10点( int ticks, int life );
    void CreateBuf_最大血量加百分之100( int ticks, int life );
    void CreateDot_血量恢复1点( int ticks, int cd );
};

struct Buf_MaxHP_Enhance100Percent : Buf<Buf_MaxHP_Enhance100Percent>
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

struct Buf_MaxHP_Enhance10Point : Buf<Buf_MaxHP_Enhance10Point>
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

struct Dot_HP_Recover1PointPerTicks : Buf<Dot_HP_Recover1PointPerTicks>
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
        // 理论上讲，不少 dot 需要以这种方式来运作: owner->funcs.Push( .... )
        // 即相当于把事情放到 Process 之后来做. 
        // 执行过程中，有可能目标已死啥的，都需要做相应处理
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


typedef xxx::Singleton<BufPool<BufBase>> FooBufPool;
int main()
{
    FooBufPool::InitInstance();

    Stopwatch sw;
    for( int j = 0; j < 10000000; ++j )
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
            //Cout( "buf_maxHP = ", foo.buf_maxHP, ", cur_HP = ", foo.cur_HP );
        }
    }
    Cout( "ms = ", sw.ElapsedMillseconds() );

    Cout( "FooBufPool::GetInstance()->data[ 0 ].Size() = ", FooBufPool::GetInstance()->data[ 0 ].Size() );
    Cout( "FooBufPool::GetInstance()->data[ 1 ].Size() = ", FooBufPool::GetInstance()->data[ 1 ].Size() );
    Cout( "FooBufPool::GetInstance()->data[ 2 ].Size() = ", FooBufPool::GetInstance()->data[ 2 ].Size() );

    system( "pause" );
    return 0;
}
