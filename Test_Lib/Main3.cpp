#include "Lib/All.h"
#include <conio.h>
using namespace std;
using namespace xxx;


template<typename T>
struct BoolMarker
{
    void Init( int capacity );
    void Register( T const& k, List<T> const& vs );     // ע�ụ�������¼�
    void Begin( T const& k );                           // �¼���ʼǰ call ���
    void End( T const& k );                             // �¼������� call ���
    int GetIndexs( T* result );                         // ȡ available ���¼�����䵽 k �ڴ棬���ظ���
    bool operator[]( T const& k );                      // ��ö�ٷ�ʽȡ check available

    template<typename PT>
    void Register( T const& k, PT&& p );
    template<typename PT, typename ...PTS>
    void Register( T const& k, PT&& p, PTS&&...ps );
private:
    List<List<T>> maps;                                 // register ����
    List<std::pair<T, int>> marks;                      // begin -> end ����
    List<bool> result;                                  // �������
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





// ��ǰ�趨��
// ���� �� �ԣ��������ֻ� ���⣨���Ϊ����CD��
// ��, �� ����
// ��, �� ��ͬʱ�� ���ֻ�
enum class ��Ϊ
{
    ���ֻ�,   // 0
    ��,       // 1
    ��,       // 2
    ����,     // 3
    MaxValue  // 4
};

int main()
{
    List<��Ϊ> result;
    result.Resize( (int)��Ϊ::MaxValue );

    BoolMarker<��Ϊ> bm;
    bm.Init( (int)��Ϊ::MaxValue );
    bm.Register( ��Ϊ::����, ��Ϊ::��, ��Ϊ::��, ��Ϊ::���ֻ� );
    bm.Register( ��Ϊ::��, ��Ϊ::�� );
    bm.Register( ��Ϊ::��, ��Ϊ::�� );

    // todo

    return 0;
}
