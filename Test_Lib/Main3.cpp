#include "Lib/All.h"
#include <conio.h>
using namespace std;
using namespace xxx;


template<typename T>
struct BoolMarker
{
    void Init( int capacity );
    void Register( T const& k, List<T> const& vs );     // 注册互斥的相关事件
    void Begin( T const& k );                           // 事件开始前 call 这个
    void End( T const& k );                             // 事件结束后 call 这个
    int GetIndexs( T* result );                         // 取 available 的事件表：填充到 k 内存，返回个数
    bool operator[]( T const& k );                      // 以枚举方式取 check available

    template<typename PT>
    void Register( T const& k, PT&& p );
    template<typename PT, typename ...PTS>
    void Register( T const& k, PT&& p, PTS&&...ps );
private:
    List<List<T>> maps;                                 // register 容器
    List<std::pair<T, int>> marks;                      // begin -> end 容器
    List<bool> result;                                  // 结果容器
};

template<typename T>
void BoolMarker<T>::Init( int capacity )
{
    maps.Resize( capacity );
    result.Resize( capacity );
    for( int i = 0; i < capacity; ++i )
    {
        maps[ i ].Clear();
    }
}

template<typename T>
void BoolMarker<T>::Register( T const& k, List<T> const& vs )
{
    maps[ (int)k ] = vs;
}

template<typename T>
template<typename PT>
void BoolMarker<T>::Register( T const& k, PT&& p )
{
    maps[ (int)k ].Push( p );
}

template<typename T>
template<typename PT, typename ...PTS>
void BoolMarker<T>::Register( T const& k, PT&& p, PTS&&...ps )
{
    maps[ (int)k ].Push( p );
    Register( k, std::forward<PTS>( ps )... );
}


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





// 当前设定：
// 发呆 与 吃，拉，玩手机 互斥（理解为公共CD）
// 吃, 拉 互斥
// 吃, 拉 的同时能 玩手机
enum class 行为
{
    玩手机,   // 0
    吃,       // 1
    拉,       // 2
    发呆,     // 3
    MaxValue  // 4
};

int main()
{
    List<行为> result;
    result.Resize( (int)行为::MaxValue );

    BoolMarker<行为> bm;
    bm.Init( (int)行为::MaxValue );
    bm.Register( 行为::发呆, 行为::拉, 行为::吃, 行为::玩手机 );
    bm.Register( 行为::吃, 行为::拉 );
    bm.Register( 行为::拉, 行为::吃 );

    // todo

    return 0;
}
