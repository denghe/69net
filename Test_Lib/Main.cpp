#include "Lib/All.h"
using namespace xxx;


// todo:

// ʵ�� DbContainer ϵ��, �� DateTime

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

namespace DBContainer
{
    enum class DbDataTypes
    {
        Boolean,
        Int8,
        Int16,
        Int32,
        Int64,
        UInt8,
        UInt16,
        UInt32,
        UInt64,
        Float,
        Double,
        DateTime,
        String,
        Bytes,
        Unknown                 // for default value/ max value
    };

    template<typename T>
    bool IsType( DbDataTypes t )
    {
        return false;
    }
    template<>    bool IsType<bool>( DbDataTypes t ) { return t == DbDataTypes::Boolean; }
    template<>    bool IsType<int8>( DbDataTypes t ) { return t == DbDataTypes::Int8; }
    template<>    bool IsType<int16>( DbDataTypes t ) { return t == DbDataTypes::Int16; }
    template<>    bool IsType<int32>( DbDataTypes t ) { return t == DbDataTypes::Int32; }
    template<>    bool IsType<int64>( DbDataTypes t ) { return t == DbDataTypes::Int64; }
    template<>    bool IsType<uint8>( DbDataTypes t ) { return t == DbDataTypes::UInt8; }
    template<>    bool IsType<uint16>( DbDataTypes t ) { return t == DbDataTypes::UInt16; }
    template<>    bool IsType<uint32>( DbDataTypes t ) { return t == DbDataTypes::UInt32; }
    template<>    bool IsType<uint64>( DbDataTypes t ) { return t == DbDataTypes::UInt64; }
    template<>    bool IsType<float>( DbDataTypes t ) { return t == DbDataTypes::Float; }
    template<>    bool IsType<double>( DbDataTypes t ) { return t == DbDataTypes::Double; }
    //template<>    bool IsType<DateTime>( DbDataTypes t ) { return t == DbDataTypes::DateTime; }
    template<>    bool IsType<String>( DbDataTypes t ) { return t == DbDataTypes::String; }
    template<>    bool IsType<ByteBuffer>( DbDataTypes t ) { return t == DbDataTypes::Bytes; }


    struct DbArray
    {
        DbDataTypes     dataType;
        void*           list;
        template<typename T>
        T& operator[]( int idx )
        {
            return ( ( List<T>* )list )->At( idx );
        }
        template<typename T>
        T const& operator[]( int idx ) const
        {
            return ( ( List<T>* )list )->At( idx );
        }
    };
    //struct DbTable
    //{
    //    String           name = "";
    //    String           comment = "";
    //    List<object>     columnDatas = new List<object>();
    //    List<DbColumn>   columns = new List<DbColumn>();
    //    List<List<bool>> nullflags = new List<List<bool>>();
    //    int              rowCount = 0;
    //};
}


int main()
{


    return 0;
}
