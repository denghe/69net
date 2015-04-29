#include "Lib/All.h"

// todo:

// 整理 Sqlite 类相关
// 以类似 Sqlite 类的长相，提供 Mysql 的支持
// 设计 Mysql 数据库操作层的长相

// 分配器 for ByteBuffer ?
// Nullable 的序列化支持?
// Any 类型的引入? ( 可以是 c++ 内置数据类型 以及常用常见的集合啥的 )
// 简化各种 Size()
// 指针是否为空的 byte 的功能利用：比如可存序列化类型，
// 类型可能指的是 是否逐个序列化，还是说  index : value 部分列化， 或是 bit 标记
// 部分序列化，指成员变量的值与默认值不等的那些。
// 极端需求下，可对比两种方式的序列化后长度差，选出最省的方式


#include <stdio.h>
#include <stdlib.h>
using namespace std;
using namespace xxx;

//struct Player
//{
//    string name;
//    int gold;
//};
//
//struct GoldMaker1
//{
//    GoldMaker1( Player& p ) : p( &p ) {}
//    Player* p;
//    NOINLINE void Increase() { p->gold++; }
//    void Dump() { cout << p->name << "'s gold = " << p->gold << endl; }
//};
//
//struct GoldMaker2
//{
//    GoldMaker2( Player& p ) : p_gold( &p.gold ) {}
//    int* p_gold;
//    NOINLINE void Increase() { ( *p_gold )++; }
//    void Dump()
//    {
//        auto p = (Player*)( (size_t)p_gold - (size_t)( &( (Player*)0 )->gold ) );
//        cout << p->name << "'s gold = " << p->gold << endl;
//    }
//};

int main()
{

    Cout( (float)123, " ", (float)-123 );

//    Player p1 = { "p1", 0 }, p2 = { "p2", 0 };
//    GoldMaker1 gm1( p1 );
//    GoldMaker2 gm2( p2 );
//    Stopwatch sw;
//    // 预热
//    p1.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm1.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p1.gold = ", p1.gold );
//
//    p1.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm1.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p1.gold = ", p1.gold );
//
//    p2.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm2.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p2.gold = ", p2.gold );
//
//    p1.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm1.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p1.gold = ", p1.gold );
//
//    p2.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm2.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p2.gold = ", p2.gold );
//
//    p1.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm1.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p1.gold = ", p1.gold );
//
//    p2.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm2.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p2.gold = ", p2.gold );
//
//    p1.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm1.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p1.gold = ", p1.gold );
//
//    p2.gold = 0;    sw.Reset();
//    for( int i = 0; i < 1000000000; ++i ) gm2.Increase();
//    CoutLine( "ms = ", sw.ElapsedMillseconds(), " p2.gold = ", p2.gold );


    //char buf[ 256 ];
    //Stopwatch sw;

    //// 预热
    //sw.Reset();
    //for( int i = -10000000; i < 10000000; ++i )
    //{
    //    double d = (double)i + 0.12345678;
    //    buf[ ToString( buf, d ) ] = 0;
    //}
    //CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );


    //sw.Reset();
    //for( int i = -10000000; i < 10000000; ++i )
    //{
    //    int d = i;
    //    buf[ ToString( buf, d ) ] = 0;
    //}
    //CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );

    //sw.Reset();
    //for( int i = -10000000; i < 10000000; ++i )
    //{
    //    int d = i;
    //    buf[ sprintf( buf, "%d", d ) ] = 0;
    //}
    //CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );



    //sw.Reset();
    //for( int i = -10000000; i < 10000000; ++i )
    //{
    //    double d = (double)i + 0.12345678;
    //    buf[ ToString( buf, d ) ] = 0;
    //}
    //CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );

    //sw.Reset();
    //for( int i = -10000000; i < 10000000; ++i )
    //{
    //    double d = (double)i + 0.12345678;
    //    buf[ sprintf( buf, "%lf", d ) ] = 0;
    //}
    //CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );











    ////FlatQueue<String> fq;
    ////fq.Push( "asdf" );

    ////DbTable dt;
    ////dt.AddColumns(
    ////    DbDataTypes::Boolean,
    ////    DbDataTypes::Int32,
    ////    DbDataTypes::String );
    ////dt.AddColumn( DbDataTypes::DateTime ).nullable = true;
    ////dt.AddRow( true, 1, "zzz", system_clock::now() );
    ////dt.AddRow( false, 234, "qwer", DbNull );

    ////Dump( dt );









    return 0;
}
