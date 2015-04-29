//#include "Lib/All.h"
//#include <conio.h>
//using namespace std;
//using namespace xxx;
//
//
//// todo: Rejector 定长高速版
//
//
//// 通过注册 k 相关 T，运行时 Open Close，Calc 出可用的 T 列表( 通常拿来算互斥，比如得到当前可用的技能列表 )
//// T 要求能与 int 互转（通常它应该是个 enum 或就是 int 且值域小）
//template<typename T>
//struct Rejector
//{
//    void Init( int capacity );                          // 初始化为指定长度的容器（可反复调用以清空所有）
//    template<typename PT>
//    void Register( T const& k, PT&& p );                // 注册 k 的相关 T
//    template<typename PT, typename ...PTS>
//    void Register( T const& k, PT&& p, PTS&&...ps );
//    void Open( T const& k );                            // 标记 k 及相关 T ( k 开始前 call 这个 )
//    void Close( T const& k );                           // 反标记 k 及相关 T ( k 结束后 call 这个 )
//    void Calc();                                        // 算结果( 更新可用 T 列表 )
//    int GetIndexs( T* result ) const;                   // 取可用 T 列表：填充到 result，返回个数
//    bool operator[]( T const& k ) const;                // 以下标方式判断 T 是否可用
//    int Size() const;
//
//protected:
//    List<List<T>> maps;                                 // 存放已注册的互斥 T
//    List<T> opened;                                     // Open 函数开启的 T 的数据队列
//    List<int> openedPos;                                // 所对应的 opened 队列中的下标
//    List<bool> result;                                  // 结果容器
//    bool dirty = true;                                  // Open Close 后会变脏, Calc 后会清。防 Calc 冗余计算
//};
//
//template<typename T>
//void Rejector<T>::Init( int capacity )
//{
//    maps.Resize( capacity );
//    result.Resize( capacity );
//    openedPos.Resize( capacity );
//    opened.Clear();
//    for( int i = 0; i < capacity; ++i )
//    {
//        maps[ i ].Clear();
//#if __DEBUG
//        openedPos[ i ] = -1;
//#endif
//    }
//    dirty = true;
//}
//
//template<typename T>
//template<typename PT>
//void Rejector<T>::Register( T const& k, PT&& p )
//{
//    maps[ (int)k ].Push( std::forward<PT>( p ) );
//}
//
//template<typename T>
//template<typename PT, typename ...PTS>
//void Rejector<T>::Register( T const& k, PT&& p, PTS&&...ps )
//{
//    maps[ (int)k ].Push( std::forward<PT>( p ) );
//    Register( k, std::forward<PTS>( ps )... );
//}
//
//template<typename T>
//void Rejector<T>::Open( T const& k )
//{
//    assert( openedPos[ (int)k ] == -1 );
//    openedPos[ (int)k ] = opened.Size();
//    opened.Push( k );
//    dirty = true;
//}
//
//template<typename T>
//void Rejector<T>::Close( T const& k )
//{
//    assert( openedPos[ (int)k ] != -1 );
//    auto& idx = openedPos[ (int)k ];
//    auto& top = opened.Top();
//    if( opened.Size() > 1 )
//    {
//        opened[ idx ] = top;
//        openedPos[ (int)top ] = idx;
//    }
//    opened.Pop();
//    dirty = true;
//#if __DEBUG
//    openedPos[ (int)k ] = -1;
//#endif
//}
//
//template<typename T>
//void Rejector<T>::Calc()
//{
//    if( !dirty ) return;
//    result.FillTrue();
//    for( int i = 0; i < opened.Size(); ++i )
//    {
//        auto idx = (int)opened[ i ];
//        result.SetFalse( idx );
//        auto& os = maps[ idx ];
//        for( int j = 0; j < os.Size(); ++j )
//        {
//            result.SetFalse( (int)os[ j ] );
//        }
//    }
//}
//
//template<typename T>
//int Rejector<T>::GetIndexs( T* result ) const
//{
//    int c = 0;
//    for( int i = 0; i < result.Size(); ++i )
//    {
//        if( result[ i ] ) result[ c++ ] = (T)i;
//    }
//    return c;
//}
//
//template<typename T>
//bool Rejector<T>::operator[]( T const& k ) const
//{
//    return result[ (int)k ];
//}
//
//template<typename T>
//int Rejector<T>::Size() const
//{
//    return result.Size();
//}
//
//
//
//// 已知 裂伤 与 扫击 互斥, 使用后会进 公共CD
//// 重殴 有自己的独立 CD
//enum class 小德技能
//{
//    裂伤,
//    扫击,
//    公共CD,
//    重殴,
//    重殴CD,
//    MaxValue
//};
//
//char* ns[] =
//{
//    "裂伤",
//    "扫击",
//    "公共CD",
//    "重殴",
//    "重殴CD",
//};
//
//int main()
//{
//    Rejector<小德技能> r;
//    int dumpNumber = 0;
//    auto dump = [ &]()
//    {
//        Cout( "dump bm ", dumpNumber++, ": " );
//        for( int i = 0; i < (int)小德技能::MaxValue; ++i )
//        {
//            if( r[ (小德技能)i ] ) Cout( ns[ i ], " " );
//        }
//        CoutLine();
//    };
//
//    r.Init( (int)小德技能::MaxValue );
//    r.Register( 小德技能::裂伤, 小德技能::扫击, 小德技能::公共CD );
//    r.Register( 小德技能::扫击, 小德技能::裂伤, 小德技能::公共CD );
//    r.Register( 小德技能::公共CD, 小德技能::扫击, 小德技能::裂伤 );
//    r.Register( 小德技能::重殴, 小德技能::重殴CD );
//    r.Register( 小德技能::重殴CD, 小德技能::重殴 );
//
//    // 这两个技能 同帧 发起
//    r.Open( 小德技能::裂伤 );
//    r.Calc(); dump();
//    r.Open( 小德技能::重殴 );
//    r.Calc(); dump();
//
//    // 1 帧后，技能结束 进入CD
//    r.Close( 小德技能::裂伤 ); r.Open( 小德技能::公共CD );
//    r.Close( 小德技能::重殴 ); r.Open( 小德技能::重殴CD );
//    r.Calc(); dump();
//
//    // 30 帧后, 技能CD 结束
//    r.Close( 小德技能::公共CD );
//    r.Calc(); dump();
//
//    // 当前只有这招可用
//    r.Open( 小德技能::扫击 );
//    r.Calc(); dump();
//
//    // 1 帧后，技能结束 进入CD
//    r.Close( 小德技能::扫击 ); r.Open( 小德技能::公共CD );
//    r.Calc(); dump();
//
//    // 30 帧后, 技能CD 结束
//    r.Close( 小德技能::公共CD );
//    r.Calc(); dump();
//
//    // 90 帧后, 重殴技能CD 结束
//    r.Close( 小德技能::重殴CD );
//    r.Calc(); dump();
//
//    return 0;
//}
