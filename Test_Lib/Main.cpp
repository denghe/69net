#include "Lib/All.h"

// todo:

// ���� Sqlite �����
// ������ Sqlite ��ĳ��࣬�ṩ Mysql ��֧��
// ��� Mysql ���ݿ������ĳ���

// ������ for ByteBuffer ?
// Nullable �����л�֧��?
// Any ���͵�����? ( ������ c++ ������������ �Լ����ó����ļ���ɶ�� )
// �򻯸��� Size()
// ָ���Ƿ�Ϊ�յ� byte �Ĺ������ã�����ɴ����л����ͣ�
// ���Ϳ���ָ���� �Ƿ�������л�������˵  index : value �����л��� ���� bit ���
// �������л���ָ��Ա������ֵ��Ĭ��ֵ���ȵ���Щ��
// ���������£��ɶԱ����ַ�ʽ�����л��󳤶Ȳѡ����ʡ�ķ�ʽ


#include <stdio.h>
#include <stdlib.h>

using namespace xxx;
//using namespace std::chrono;
int main()
{
    char buf[ 256 ];
    Stopwatch sw;

    // Ԥ��
    sw.Reset();
    for( int i = -10000000; i < 10000000; ++i )
    {
        double d = (double)i + 0.12345678;
        buf[ ToString( buf, d ) ] = 0;
    }
    CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );


    sw.Reset();
    for( int i = -10000000; i < 10000000; ++i )
    {
        int d = i;
        buf[ ToString( buf, d ) ] = 0;
    }
    CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );

    sw.Reset();
    for( int i = -10000000; i < 10000000; ++i )
    {
        int d = i;
        buf[ sprintf( buf, "%d", d ) ] = 0;
    }
    CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );



    sw.Reset();
    for( int i = -10000000; i < 10000000; ++i )
    {
        double d = (double)i + 0.12345678;
        buf[ ToString( buf, d ) ] = 0;
    }
    CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );

    sw.Reset();
    for( int i = -10000000; i < 10000000; ++i )
    {
        double d = (double)i + 0.12345678;
        buf[ sprintf( buf, "%lf", d ) ] = 0;
    }
    CoutLine( "ms = ", sw.ElapsedMillseconds(), " buf = ", buf );

    //FlatQueue<String> fq;
    //fq.Push( "asdf" );

    //DbTable dt;
    //dt.AddColumns(
    //    DbDataTypes::Boolean,
    //    DbDataTypes::Int32,
    //    DbDataTypes::String );
    //dt.AddColumn( DbDataTypes::DateTime ).nullable = true;
    //dt.AddRow( true, 1, "zzz", system_clock::now() );
    //dt.AddRow( false, 234, "qwer", DbNull );

    //Dump( dt );

    return 0;
}
