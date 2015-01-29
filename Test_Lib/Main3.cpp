//#include "Lib/All.h"
//#include <conio.h>
//using namespace std;
//using namespace xxx;
//
//
//// todo: Rejector �������ٰ�
//
//
//// ͨ��ע�� k ��� T������ʱ Open Close��Calc �����õ� T �б�( ͨ�������㻥�⣬����õ���ǰ���õļ����б� )
//// T Ҫ������ int ��ת��ͨ����Ӧ���Ǹ� enum ����� int ��ֵ��С��
//template<typename T>
//struct Rejector
//{
//    void Init( int capacity );                          // ��ʼ��Ϊָ�����ȵ��������ɷ���������������У�
//    template<typename PT>
//    void Register( T const& k, PT&& p );                // ע�� k ����� T
//    template<typename PT, typename ...PTS>
//    void Register( T const& k, PT&& p, PTS&&...ps );
//    void Open( T const& k );                            // ��� k ����� T ( k ��ʼǰ call ��� )
//    void Close( T const& k );                           // ����� k ����� T ( k ������ call ��� )
//    void Calc();                                        // ����( ���¿��� T �б� )
//    int GetIndexs( T* result ) const;                   // ȡ���� T �б���䵽 result�����ظ���
//    bool operator[]( T const& k ) const;                // ���±귽ʽ�ж� T �Ƿ����
//    int Size() const;
//
//protected:
//    List<List<T>> maps;                                 // �����ע��Ļ��� T
//    List<T> opened;                                     // Open ���������� T �����ݶ���
//    List<int> openedPos;                                // ����Ӧ�� opened �����е��±�
//    List<bool> result;                                  // �������
//    bool dirty = true;                                  // Open Close ������, Calc ����塣�� Calc �������
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
//// ��֪ ���� �� ɨ�� ����, ʹ�ú��� ����CD
//// ��Ź ���Լ��Ķ��� CD
//enum class С�¼���
//{
//    ����,
//    ɨ��,
//    ����CD,
//    ��Ź,
//    ��ŹCD,
//    MaxValue
//};
//
//char* ns[] =
//{
//    "����",
//    "ɨ��",
//    "����CD",
//    "��Ź",
//    "��ŹCD",
//};
//
//int main()
//{
//    Rejector<С�¼���> r;
//    int dumpNumber = 0;
//    auto dump = [ &]()
//    {
//        Cout( "dump bm ", dumpNumber++, ": " );
//        for( int i = 0; i < (int)С�¼���::MaxValue; ++i )
//        {
//            if( r[ (С�¼���)i ] ) Cout( ns[ i ], " " );
//        }
//        CoutLine();
//    };
//
//    r.Init( (int)С�¼���::MaxValue );
//    r.Register( С�¼���::����, С�¼���::ɨ��, С�¼���::����CD );
//    r.Register( С�¼���::ɨ��, С�¼���::����, С�¼���::����CD );
//    r.Register( С�¼���::����CD, С�¼���::ɨ��, С�¼���::���� );
//    r.Register( С�¼���::��Ź, С�¼���::��ŹCD );
//    r.Register( С�¼���::��ŹCD, С�¼���::��Ź );
//
//    // ���������� ͬ֡ ����
//    r.Open( С�¼���::���� );
//    r.Calc(); dump();
//    r.Open( С�¼���::��Ź );
//    r.Calc(); dump();
//
//    // 1 ֡�󣬼��ܽ��� ����CD
//    r.Close( С�¼���::���� ); r.Open( С�¼���::����CD );
//    r.Close( С�¼���::��Ź ); r.Open( С�¼���::��ŹCD );
//    r.Calc(); dump();
//
//    // 30 ֡��, ����CD ����
//    r.Close( С�¼���::����CD );
//    r.Calc(); dump();
//
//    // ��ǰֻ�����п���
//    r.Open( С�¼���::ɨ�� );
//    r.Calc(); dump();
//
//    // 1 ֡�󣬼��ܽ��� ����CD
//    r.Close( С�¼���::ɨ�� ); r.Open( С�¼���::����CD );
//    r.Calc(); dump();
//
//    // 30 ֡��, ����CD ����
//    r.Close( С�¼���::����CD );
//    r.Calc(); dump();
//
//    // 90 ֡��, ��Ź����CD ����
//    r.Close( С�¼���::��ŹCD );
//    r.Calc(); dump();
//
//    return 0;
//}
