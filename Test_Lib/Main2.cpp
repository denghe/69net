//#include "Lib/All.h"
//#include <conio.h>
//using namespace std;
//using namespace xxx;
//
///*
//
//todo:
//
//���ֵ��͹ֵ� AI:
//1. warrior
//2. archer
//3. priest
//
//ְҵ�ص㣺
//warrior ��ս������������Χ��С( ������ )
//archer Զ�̣�����������Χ�ϴ󣬼�ʸ����ɳ�( ������ )
//priest Զ�̣��������Ʒ�Χ�еȣ����÷�ʽΪ������Ŀ�����ϵ����Ʒ���Ч��( ˲���� )
//
//�ϸ���˵����Ŀ�����þ�����˵���ǲ���ʲôԶ�̽�ս�ģ���սҲ�п��ܲ���Ͷ����Ϊ
//����ʲô paladin ����̹����ְҵ�����ڸ����ͣ���ǰ�Ȳ���
//�����趨����Ϊ���������ͣ�����Ҳֻ��һ�֣��Է���ʵ���ر�� AI Ч��
//
//�����ԣ�
//    �뾶: ��ɫռ�����, �����Ͻ�Ҳ�������ҽӵ�ļ�������           ��λ����       ����ֵ��0.5
//    Ѫ��: ����������ֵ                                             ��λ����       ����ֵ��100
//    ����: ��Ϊ����ǿ������ֵ                                       ��λ����       ����ֵ��100
//    ��Ұ: ���䷶Χ��������Ϊ��һ�����ɫ�ص���Բ�İ뾶             ��λ����       ����ֵ��10
//    ����: ÿ���ƶ�����                                             ��λ����       ����ֵ��3.5
//
//�������ԣ�
//    ��ǿ: ��Ϊ��������ֵ( ����ǿ�ȹ�ʽ�� ��ǿ * ����/100 )         ��λ����       ����ֵ��20
//    ����: ÿ���Ӳ��� ��Ϊ�Ĵ���                                    ��λ����       ����ֵ��1
//    ���: ��Ϊ������Ŀ���������( ͨ���� ��Ұ С���� )           ��λ����       ����ֵ��5
//
//������أ�
//    ������
//        ��������������ҽ�
//        �����Ͻ���������һ��ʱ���ᡣ��ʱ��Σ��ֳɶ��״̬��ÿ״̬�߱�һЩ���ԡ�
//        ״̬��û�пհ�ʱ�䣬��һ��״̬��Ȼ������һ��״̬��
//
//    ״̬���ԣ�
//        ��㣺��ǰ״̬��ʼ��ʱ���
//        ʱ�����������һ״̬������״̬����ʱ�������
//        ���ͣ�Ч�����Լ�һЩ�������ԣ�ǿ�ȣ����ԣ��Ƿ��ܱ���ϻ����ӵȣ�
//
//    �༼�ܣ�
//        ���ܼ���ڷ��黥����������һ������ʩչ�ڼ䣬�����޷�ͬʱʩչ��ļ��ܡ�
//        �⼴��Ҫʵʩһ�����ܻ���������ȷ����ǰʱ��� ����Щ���ܿ��á�
//
//    ��ȷʱ��: 
//        ������Ҳ������һ�����ܣ�ͨ������ͨ�����ͷ���֮����ŷų����벿��/ȫ�����ܻ��⡣
//
//AI��أ�
//    ���У���������/������Ϊ�ľ�����ֵ( ��̵İٷֱ� )              ��λ���ٷֱ�   ����ֵ��80%
//    ���У�����������Ϊ�ı���Ѫ���ٷֱ�( �ݶ� )                     ��λ���ٷֱ�   ����ֵ��20%
//    ���У�����������Ϊ�� ���Ŀ�� ��һ���˺�Ѫ���ٷֱ�( �ݶ� )     ��λ���ٷֱ�   ����ֵ��10%
//
//����"����"��
//    �����ϵ�� AI ��ѡ�����Ϊʱ����
//    ��Ŀ�������̵�˲�� ��ֹ����ʱ��AI �������� ��Զ�����չ����Ϊ��
//    ���ǻ�Ҫ�ٽӽ�һЩ���Ծ���ȷ�� ����Ŀ����ʱ���Ȼ�������ˣ�Ҳ���ǹ����š�
//    Ϊȷ��������Ч�ԣ�������������Ա��Ե� AI �� "Ԥ��"��
//
//    ���絶�� 1 �ף�Ŀ�����ĵ� ����λ�����Լ������ҽӵ�� 1�� + Ŀ��뾶  ���ڣ�
//    ��ʱ��� ����( ���и�����ӿ��Ĺ��� )���ȵ�������������Ч�ж��׶Σ�ʱ��
//    Ŀ������Ѿ� �Ƴ��� ���Ĺ������룬���Ǳ㿳����Ŀ���ˣ�����Ŀ�겻���������Լ��ƶ���û���ܵ��ҽӵ㱳��
//
//    ��һ�����Ϊ����Ŀ�������̵�˲�� �������ƶ��ģ���ȡ��Ŀ����ٶȣ�
//    ��ʱ�򣬿ɽ�� ���� �������������ǰ�������� "����"��
//    ��Ȼ��͵���������ǣ�ÿ�ζ����� "����" ��������Ϊ�������������Ҳ���ñ����ȶ�������������
//
//����"����"��
//    AI �ж�Ѫ�ٵ�һ���̶ȵ� �Ա���Ϊ����Ҳ�п��ܳ��� "�Ӳ���" ���������ʱ��ɴ಻���ˡ�
//    ����һ������ǣ�����Է��� "��һ��" ����Ҫ���Լ������������Ͻ�Ҳ����ѡ����
//    �����ӻ��ӣ����ܻ�Ҫ�ٽ�һ���Ļ��ֳ� �Ը� ɶ�����Գ�������һ����ָ��ѡ��
//
//����"����"��
//    AI �ж� ���Լ���Ŀ�꣬��һ�� �����Լ���������Ѫ����һ���ٷֱ�ֵ��
//    ���ӵõ�������£� AI ѡ�� ����
//    Ӱ�� ���� ѡ������أ�����������Ϊ�������� ����ֵ
//
//    ���� archer ������ �ŷ��ݣ�����ԭ��Ӳƴ����ͺ� ����ֵ �Լ�������û���� ���Լ��Է��Ĺ����� ���й�ϵ
//
//��ν"�ӵõ�":
//    �����߽����Լ�����Ұ���Լ���δ���빥���ߵ����ʱ
//    �����Լ������ٿ��������ʱ
//
//
//ע������ֵ�Ķ�����Χ��ǰ������
//
//*/
//
//struct Skill
//{
//
//};
//struct Weapon
//{
//    float damage;           // ��ֵ
//    float attacksPerSecond; // ÿ�빥������
//    float range;            // ���( �� )
//};
//struct Monster
//{
//    float bodyRadius;       // ����뾶( �� )
//    float viewRadius;       // ��Ұ�뾶( �� )
//    float health;           // Ѫ��( �� )
//    float power;            // ����( �� )
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
//// ������ ���ܻ����ж�
//// ʹ��������T �ܺ� int ��ת( ͨ���Ǹ� enum )
//// todo: GetIndexs �ⲽ�������Ż�
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
//// ��ǰ�趨��
//// ���� �� �ԣ��������ֻ� ���⣨���Ϊ����CD��
//// ��, �� ����
//// ��, �� ��ͬʱ�� ���ֻ�
//enum class ��Ϊ
//{
//    ���ֻ�,   // 0
//    ��,       // 1
//    ��,       // 2
//    ����,     // 3
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
//    List<��Ϊ> result;
//    result.Resize( (int)��Ϊ::MaxValue );
//
//    BoolMarker<��Ϊ> bm;
//    bm.Init( (int)��Ϊ::MaxValue );
//    bm.Register( ��Ϊ::����, ��Ϊ::��, ��Ϊ::��, ��Ϊ::���ֻ� );
//    bm.Register( ��Ϊ::��, ��Ϊ::�� );
//    bm.Register( ��Ϊ::��, ��Ϊ::�� );
//
//    size_t c = 0;
//
//    Stopwatch sw;
//    for( int i = 0; i < 9999999; ++i )
//    {
//        bm.Mark( ��Ϊ::�� );
//        auto len = bm.GetIndexs( result.Data() );
//        bm.FillFalse();
//        //for( int i = 0; i < result.Size(); ++i ) CoutLine( (int)result[ i ] ); // Ӧ��� 0, 3
//        //CoutLine();
//        c += len;
//        result.Clear();
//
//        bm.Mark( ��Ϊ::���ֻ� );
//        bm.Mark( ��Ϊ::�� );
//        len = bm.GetIndexs( result.Data() );
//        bm.FillFalse();
//        //for( int i = 0; i < result.Size(); ++i ) CoutLine( (int)result[ i ] ); // Ӧ��� 3
//        //CoutLine();
//        c += len;
//        result.Clear();
//
//        bm.Mark( ��Ϊ::���� );
//        len = bm.GetIndexs( result.Data() );
//        bm.FillFalse();
//        //for( int i = 0; i < result.Size(); ++i ) CoutLine( (int)result[ i ] ); // �����
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
