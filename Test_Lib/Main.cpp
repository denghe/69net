#include "Main.h"

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


using namespace xxx;
int main()
{
    DbTable dt;
    dt.AddColumn( DbDataTypes::Boolean );
    dt.AddColumn( DbDataTypes::Int32 );
    dt.AddColumn( DbDataTypes::String );
    dt.AddColumn( DbDataTypes::DateTime ).nullable = true;

    dt.AddRow( true, 1, "zzz", std::chrono::system_clock::now() );
    dt.AddRow( false, 234, "qwer", DbNull );

    String s;
    dt.ToString( s );
    CoutLine( s );
    //dt.
    //auto row = dt.AddRow();
    //row[ c0 ].SetValue<bool>( true );
    //row[ c1 ].SetValue<int>( 123 );
    //row[ c2 ].SetValue<String>( "asdf" );


    return 0;
}
