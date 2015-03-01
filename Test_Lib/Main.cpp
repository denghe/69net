#include "Lib/All.h"
using namespace xxx;


// todo:

// 实现 DbContainer 系列, 补 DateTime

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
