//#include "Lib/All.h"
//#include <conio.h>
//using namespace std;
//using namespace xxx;
//
///*
//
//todo:
//
//三种典型怪的 AI:
//1. warrior
//2. archer
//3. priest
//
//职业特点：
//warrior 近战，武器攻击范围较小( 持有型 )
//archer 远程，武器攻击范围较大，箭矢离体飞出( 抛射型 )
//priest 远程，辅助治疗范围中等，作用方式为出现在目标身上的治疗法术效果( 瞬移型 )
//
//严格来说，就目标作用距离来说，是不分什么远程近战的，近战也有可能产生投掷行为
//还有什么 paladin 这种坦克奶职业，属于复合型，当前先不管
//上述设定就是为了树立典型，武器也只带一种，以方便实现特别的 AI 效果
//
//精简共性：
//    半径: 角色占地面积, 理论上讲也是武器挂接点的计算依据           单位：米       典型值：0.5
//    血量: 抗击打量化值                                             单位：点       典型值：100
//    力量: 行为能力强度量化值                                       单位：点       典型值：100
//    视野: 警戒范围，姑且认为是一个与角色重叠的圆的半径             单位：米       典型值：10
//    跑速: 每秒移动米数                                             单位：点       典型值：3.5
//
//武器共性：
//    攻强: 行为能力基础值( 最终强度公式： 攻强 * 力量/100 )         单位：点       典型值：20
//    攻速: 每秒钟产生 行为的次数                                    单位：次       典型值：1
//    射程: 行为作用于目标的最大距离( 通常比 视野 小不少 )           单位：米       典型值：5
//
//技能相关：
//    描述：
//        可与人物或武器挂接
//        理论上讲，技能是一个时间轴。按时间段，分成多个状态，每状态具备一些属性。
//        状态间没有空白时间，即一个状态必然紧临另一个状态。
//
//    状态属性：
//        起点：当前状态开始的时间点
//        时长：这根据下一状态或整个状态结束时间点来算
//        类型，效果，以及一些基础属性（强度，特性，是否能被打断或拖延等）
//
//    多技能：
//        技能间存在分组互斥的情况，即一个技能施展期间，可能无法同时施展别的技能。
//        意即需要实施一个技能互斥树，以确定当前时间点 有哪些技能可用。
//
//    冷确时间: 
//        理论上也被视作一个技能，通常于普通技能释放完之后接着放出，与部分/全部技能互斥。
//
//AI相关：
//    行阈：产生攻击/治疗行为的距离阈值( 射程的百分比 )              单位：百分比   典型值：80%
//    逃阈：产生逃跑行为的保底血量百分比( 暂定 )                     单位：百分比   典型值：20%
//    伤阈：产生逃跑行为的 仇恨目标 下一击伤害血量百分比( 暂定 )     单位：百分比   典型值：10%
//
//关于"行阈"：
//    这个关系到 AI 所选择的行为时机。
//    在目标进入射程的瞬间 静止不动时，AI 并不会以 最远射程来展开行为，
//    而是还要再接近一些，以尽量确保 即便目标这时候忽然动起来了，也还是够得着。
//    为确保攻击有效性，特提出这个概念，以便显得 AI 会 "预判"。
//
//    比如刀长 1 米，目标中心点 正好位于离自己武器挂接点的 1米 + 目标半径  以内，
//    此时如果 出刀( 刀有个举起挥砍的过程 )，等刀砍出（进入有效判定阶段）时，
//    目标可能已经 移出了 刀的攻击距离，于是便砍不到目标了（除非目标不动或面向自己移动且没有跑到挂接点背后）
//
//    另一种情况为，在目标进入射程的瞬间 是正在移动的（能取到目标的速度）
//    这时候，可结合 攻速 来计算出攻击提前量，无视 "行阈"。
//    当然，偷懒的做法是，每次都根据 "行阈" 来产生行为（这样或许玩家也觉得表现稳定，便于评估）
//
//关于"逃阈"：
//    AI 判定血少到一定程度的 自保行为，但也有可能出现 "逃不掉" 的情况，这时候干脆不逃了。
//    还有一种情况是，如果对方的 "下一击" 可能要了自己的命，理论上讲也可以选择撤退
//    不过逃或不逃，可能还要再进一步的划分出 性格 啥的属性出来，进一步的指引选择
//
//关于"伤阈"：
//    AI 判定 打自己的目标，下一击 会令自己减到超出血量的一个百分比值，
//    在逃得掉的情况下， AI 选择 逃离
//    影响 逃离 选择的因素：近期治疗行为，将增大 伤阈值
//
//    比如 archer 到底是 放风筝，还是原地硬拼，这就和 伤阈值 以及附近有没有奶 ，以及对方的攻击力 都有关系
//
//何谓"逃得掉":
//    攻击者进入自己的视野但自己并未进入攻击者的射程时
//    或者自己的跑速快过攻击者时
//
//
//注：上述值的抖动范围当前不考虑
//
//*/
//
//struct Skill
//{
//
//};
//struct Weapon
//{
//    float damage;           // 伤值
//    float attacksPerSecond; // 每秒攻击次数
//    float range;            // 射程( 米 )
//};
//struct Monster
//{
//    float bodyRadius;       // 身体半径( 米 )
//    float viewRadius;       // 视野半径( 米 )
//    float health;           // 血量( 点 )
//    float power;            // 力量( 点 )
//    float moveSpeed;        // 
//};
//
//
//
///*
//
//template<typename T>
//struct BoolMarker
//{
//    void Init( int capacity, bool fillBit = false );
//    void Register( T const& k, std::vector<T> const& vs );
//    void Mark( T const& k, bool bit = true );
//    void GetIndexs( std::vector<T>& result, bool bit = false );
//    void Fill( bool bit = false );
//    bool operator[]( T const& k );
//
//    template<typename PT>
//    void Register( T const& k, PT&& p );
//    template<typename PT, typename ...PTS>
//    void Register( T const& k, PT&& p, PTS&&...ps );
//private:
//    std::vector<std::vector<T>> maps;
//    std::vector<bool> context;
//};
//
//template<typename T>
//void BoolMarker<T>::Init( int capacity, bool fillBit )
//{
//    maps.resize( capacity );
//    context.resize( capacity );
//    for( int i = 0; i < capacity; ++i )
//    {
//        maps[ i ].clear();
//        context[ i ] = fillBit;
//    }
//}
//
//template<typename T>
//void BoolMarker<T>::Register( T const& k, std::vector<T> const& vs )
//{
//    maps[ (int)k ] = vs;
//}
//
//template<typename T>
//void BoolMarker<T>::Mark( T const& k, bool bit )
//{
//    context[ (int)k ] = bit;
//    auto& vs = maps[ (int)k ];
//    for( int i = 0; i < (int)vs.size(); ++i )
//    {
//        context[ (int)vs[ i ] ] = bit;
//    }
//}
//
//template<typename T>
//void BoolMarker<T>::GetIndexs( std::vector<T>& result, bool bit )
//{
//    for( int i = 0; i < (int)context.size(); ++i )
//    {
//        if( context[ i ] == bit ) result.push_back( (T)i );
//    }
//}
//
//template<typename T>
//void BoolMarker<T>::Fill( bool bit )
//{
//    for( int i = 0; i < (int)context.size(); ++i )
//    {
//        context[ i ] = bit;
//    }
//}
//
//template<typename T>
//bool BoolMarker<T>::operator[]( T const& k )
//{
//    return context[ (int)k ];
//}
//
//
//template<typename T>
//template<typename PT>
//void BoolMarker<T>::Register( T const& k, PT&& p )
//{
//    maps[ (int)k ].push_back( p );
//}
//
//template<typename T>
//template<typename PT, typename ...PTS>
//void BoolMarker<T>::Register( T const& k, PT&& p, PTS&&...ps )
//{
//    maps[ (int)k ].push_back( p );
//    Register( k, std::forward<PTS>( ps )... );
//}
//
//*/
//
//
//// 主用于 技能互斥判断
//// 使用条件：T 能和 int 互转( 通常是个 enum )
//// todo: GetIndexs 这步的性能优化
//template<typename T>
//struct BoolMarker
//{
//    void Init( int capacity );
//    void Register( T const& k, List<T> const& vs );
//    void Mark( T const& k, bool bit = true );
//    int GetIndexs( T* result, bool bit = false );
//    void FillTrue();
//    void FillFalse();
//    bool operator[]( T const& k );
//
//    template<typename PT>
//    void Register( T const& k, PT&& p );
//    template<typename PT, typename ...PTS>
//    void Register( T const& k, PT&& p, PTS&&...ps );
//private:
//    List<List<T>> maps;
//    List<bool> context;
//};
//
//template<typename T>
//void BoolMarker<T>::Init( int capacity )
//{
//    maps.Resize( capacity );
//    context.Resize( capacity );
//    for( int i = 0; i < capacity; ++i )
//    {
//        maps[ i ].Clear();
//    }
//}
//
//template<typename T>
//void BoolMarker<T>::Register( T const& k, List<T> const& vs )
//{
//    maps[ (int)k ] = vs;
//}
//
//template<typename T>
//void BoolMarker<T>::Mark( T const& k, bool bit )
//{
//    context.Set( (int)k, bit );
//    auto& vs = maps[ (int)k ];
//    for( int i = 0; i < vs.Size(); ++i )
//    {
//        context.Set( (int)vs[ i ], bit );
//    }
//}
//
//template<typename T>
//int BoolMarker<T>::GetIndexs( T* result, bool bit )
//{
//    int c = 0;
//    for( int i = 0; i < context.Size(); ++i )
//    {
//        if( context[ i ] == bit ) result[ c++ ] = (T)i;
//    }
//    return c;
//}
//
//template<typename T>
//void BoolMarker<T>::FillTrue()
//{
//    context.FillTrue();
//}
//template<typename T>
//void BoolMarker<T>::FillFalse()
//{
//    context.FillFalse();
//}
//
//template<typename T>
//bool BoolMarker<T>::operator[]( T const& k )
//{
//    return context[ (int)k ];
//}
//
//
//template<typename T>
//template<typename PT>
//void BoolMarker<T>::Register( T const& k, PT&& p )
//{
//    maps[ (int)k ].Push( p );
//}
//
//template<typename T>
//template<typename PT, typename ...PTS>
//void BoolMarker<T>::Register( T const& k, PT&& p, PTS&&...ps )
//{
//    maps[ (int)k ].Push( p );
//    Register( k, std::forward<PTS>( ps )... );
//}
//
//
//
//
//// 当前设定：
//// 发呆 与 吃，拉，玩手机 互斥（理解为公共CD）
//// 吃, 拉 互斥
//// 吃, 拉 的同时能 玩手机
//enum class 行为
//{
//    玩手机,   // 0
//    吃,       // 1
//    拉,       // 2
//    发呆,     // 3
//    e1,
//    e2,
//    e3,
//    e4,
//    e5,
//    e6,
//    e7,
//    e8,
//    e9,
//    e10,
//    e11,
//    e12,
//    e13,
//    e14,
//    e15,
//    e16,
//    e17,
//    e18,
//    e19,
//    e20,
//    e21,
//    e22,
//    e23,
//    e24,
//    e25,
//    e26,
//    e27,
//    e28,
//    e29,
//    e30,
//    MaxValue  // 4
//};
//
//int main()
//{
//    List<行为> result;
//    result.Resize( (int)行为::MaxValue );
//
//    BoolMarker<行为> bm;
//    bm.Init( (int)行为::MaxValue );
//    bm.Register( 行为::发呆, 行为::拉, 行为::吃, 行为::玩手机 );
//    bm.Register( 行为::吃, 行为::拉 );
//    bm.Register( 行为::拉, 行为::吃 );
//
//    size_t c = 0;
//
//    Stopwatch sw;
//    for( int i = 0; i < 9999999; ++i )
//    {
//        bm.Mark( 行为::吃 );
//        auto len = bm.GetIndexs( result.Data() );
//        bm.FillFalse();
//        //for( int i = 0; i < result.Size(); ++i ) CoutLine( (int)result[ i ] ); // 应输出 0, 3
//        //CoutLine();
//        c += len;
//        result.Clear();
//
//        bm.Mark( 行为::玩手机 );
//        bm.Mark( 行为::拉 );
//        len = bm.GetIndexs( result.Data() );
//        bm.FillFalse();
//        //for( int i = 0; i < result.Size(); ++i ) CoutLine( (int)result[ i ] ); // 应输出 3
//        //CoutLine();
//        c += len;
//        result.Clear();
//
//        bm.Mark( 行为::发呆 );
//        len = bm.GetIndexs( result.Data() );
//        bm.FillFalse();
//        //for( int i = 0; i < result.Size(); ++i ) CoutLine( (int)result[ i ] ); // 无输出
//        //CoutLine();
//        c += len;
//        result.Clear();
//    }
//    CoutLine( "", c, ", elapsed ms = ", sw.ElapsedMillseconds() );
//
//    return 0;
//}
//
//
//
//
////
////
////
////
////
////struct Foo;
////struct FooState1 : public CorStateBase
////{
////    Foo* owner;
////    String name;
////    int sleepTicks = 0;
////    FooState1( Foo* owner );
////    void Init( String const& name, int sleepTicks );
////    bool Process( int ticks ) override;
////};
////struct FooState2 : public CorStateBase
////{
////    Foo* owner;
////    String name;
////    int sleepTicks = 0;
////    FooState2( Foo* owner );
////    void Init( String const& name, int sleepTicks );
////    bool Process( int ticks ) override;
////};
////struct Foo : public CorBase, public AutoIDAttacher < Foo, CorBase >
////{
////    CorStateBase* s = nullptr;
////    String name;
////    FooState1 s1;
////    FooState2 s2;
////    Foo();
////    void Init( String const& name, int sleepTicks );
////    bool Process( int ticks ) override;
////};
////
////struct Bar : public CorBase, public AutoIDAttacher < Bar, CorBase >
////{
////    void Init( String const& name );
////    bool Process( int ticks ) override;
////    String name;
////    List<PoolPtr<Foo>> childs;
////    void AddChild( Foo* foo );
////};
////
////
////
////
////Foo::Foo()
////    : s1( this )
////    , s2( this )
////{
////}
////void Foo::Init( String const& name, int sleepTicks )
////{
////    this->name = name;
////    s1.Init( String::Make( name, "_state_1" ), sleepTicks );
////}
////bool Foo::Process( int ticks )
////{
////    CoutLine( name, " Process" );
////    s->Process( ticks );
////    return s != nullptr;
////}
////
////
////
////
////FooState1::FooState1( Foo* owner )
////{
////    this->owner = owner;
////}
////void FooState1::Init( String const& name, int sleepTicks )
////{
////    this->name = name;
////    this->sleepTicks = sleepTicks;
////    owner->s = this;
////}
////bool FooState1::Process( int ticks )
////{
////    COR_BEGIN;
////    CoutLine( name, " Process: before sleep( ", sleepTicks, " )" );
////    COR_SLEEP( sleepTicks );
////    CoutLine( name, " Process: sleeped" );
////    owner->s2.Init( String::Make( name, "_state_2" ), sleepTicks );
////    COR_END;
////}
////
////
////
////
////FooState2::FooState2( Foo* owner )
////{
////    this->owner = owner;
////}
////void FooState2::Init( String const& name, int sleepTicks )
////{
////    this->name = name;
////    this->sleepTicks = sleepTicks;
////    owner->s = this;
////}
////bool FooState2::Process( int ticks )
////{
////    COR_BEGIN;
////    CoutLine( name, " Process: before sleep( ", sleepTicks, " )" );
////    COR_SLEEP( sleepTicks );
////    CoutLine( name, " Process: sleeped" );
////    owner->s = nullptr;
////    COR_END;
////}
////
////
////
////
////
////
////void Bar::Init( String const& name )
////{
////    this->name = name;
////}
////
////bool Bar::Process( int ticks )
////{
////    CoutLine( name, " Process begin" );
////    Cout( "alive childs: {" );
////    for( int i = childs.Size() - 1; i >= 0; --i )
////    {
////        auto& c = childs[ i ];
////        if( auto p = c.Ptr() )
////        {
////            Cout( p->name, ", " );
////        }
////        else
////        {
////            childs.EraseFast( i );
////        }
////    }
////    CoutLine( "}" );
////    CoutLine( name, " Process end" );
////    return true;
////}
////
////void Bar::AddChild( Foo* foo )
////{
////    childs.Push( foo );
////}
////
////
////
////
////int main()
////{
////    CorManager cm;
////    auto b = cm.CreateItem<Bar>( "bar" );
////    b->AddChild( cm.CreateItem<Foo>( "foo1", 1 ) );
////    b->AddChild( cm.CreateItem<Foo>( "foo2", 2 ) );
////    b->AddChild( cm.CreateItem<Foo>( "foo3", 3 ) );
////    while( cm.Process() )
////    {
////        _getch();
////        CoutLine();
////    };
////    system( "pause" );
////    return 0;
////}
