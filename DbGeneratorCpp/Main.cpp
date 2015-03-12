#include "Lib/All.h"
using namespace xxx;

#include "mysql.h"

char* dbuser = "sa";
char* dbpassword = "1111";
char* dbip = "192.168.1.244";
short dbport = 3306;
char* dbname = "dd";
char* charset = "utf8";




int FillData( DbTable& t, MYSQL* conn )
{
    // todo: 各种转义
    String s;
    s.Append( "select " );
    for( int i = 0; i < t.columns.Size(); ++i )
    {
        auto& c = t.columns[ i ];
        if( i ) s.Append( ", " );
        s.Append( c.name );
    }
    s.Append( " from ", t.name );

    int n = mysql_query( conn, s.C_str() );
    if( n )
    {
        return -1;
    }
    auto res = mysql_store_result( conn );
    if( !res )
    {
        return -2;
    }
    ScopeGuard sgRes( [ &] { mysql_free_result( res ); } );

    while( auto myrow = mysql_fetch_row( res ) )
    {
        auto r = t.AddRow();
        for( int i = 0; i < t.columns.Size(); ++i )
        {
            auto& c = t.columns[ i ];
            switch( c.dataType )
            {
            case DbDataTypes::Boolean:
                //FromString( r[ i ].RefValue<bool>(), myrow[ i ] );
                bool b;
                FromString( b, myrow[ i ] );
                r[ i ].Assign( b );
                break;
            case DbDataTypes::Int8:
                FromString( r[ i ].RefValue<int8>(), myrow[ i ] );
                break;
            case DbDataTypes::Int16:
                FromString( r[ i ].RefValue<int16>(), myrow[ i ] );
                break;
            case DbDataTypes::Int32:
                FromString( r[ i ].RefValue<int32>(), myrow[ i ] );
                break;
            case DbDataTypes::Int64:
                FromString( r[ i ].RefValue<int64>(), myrow[ i ] );
                break;
            case DbDataTypes::UInt8:
                FromString( r[ i ].RefValue<uint8>(), myrow[ i ] );
                break;
            case DbDataTypes::UInt16:
                FromString( r[ i ].RefValue<uint16>(), myrow[ i ] );
                break;
            case DbDataTypes::UInt32:
                FromString( r[ i ].RefValue<uint32>(), myrow[ i ] );
                break;
            case DbDataTypes::UInt64:
                FromString( r[ i ].RefValue<uint64>(), myrow[ i ] );
                break;
            case DbDataTypes::Float:
                FromString( r[ i ].RefValue<float>(), myrow[ i ] );
                break;
            case DbDataTypes::Double:
                FromString( r[ i ].RefValue<double>(), myrow[ i ] );
                break;
            case DbDataTypes::DateTime:
                //FromString( r[ i ].RefValue<DateTime>(), myrow[ i ] );
                return -3;
                break;
            case DbDataTypes::String:
                r[ i ].RefValue<String>() = myrow[ i ];
                break;
            case DbDataTypes::Bytes: break;
            default:
                return -4;
            }
        }
    }

    return 0;
}












struct HashStr
{
    char const* buf;
    size_t len;
    int hash;

    template<size_t sLen>
    HashStr( char const( &s )[ sLen ] )
        : buf( s )
        , len( sLen )
        , hash( GetHash_Lua( (byte*)s, sLen ) )
    {
    }

    HashStr( char const* s )
        : buf( s )
        , len( strlen( s ) )
    {
        hash = GetHash_Lua( (byte*)s, (int)len );
    }

    bool operator==( HashStr const& o ) const
    {
        if( len != o.len || hash != o.hash ) return false;
        return memcmp( buf, o.buf, len ) == 0;
    }

    bool operator==( char const* s ) const
    {
        if( !s ) return false;
        auto sLen = strlen( s );
        if( sLen != len ) return false;
        return memcmp( buf, s, len ) == 0;
    }

    template<size_t sLen>
    bool operator==( char const( &s )[ sLen ] ) const
    {
        if( sLen != len ) return false;
        return memcmp( buf, s, len ) == 0;
    }

    bool operator==( String const& s ) const
    {
        if( s.Size() != len ) return false;
        return memcmp( buf, s.Data(), len ) == 0;
    }

};

HashStr txt_unsigned( "unsigned" );
HashStr txt_bigint( "bigint" );
HashStr txt_int( "int" );
HashStr txt_mediumint( "mediumint" );
HashStr txt_smallint( "smallint" );
HashStr txt_tinyint( "tinyint" );
HashStr txt_double( "double" );
HashStr txt_float( "float" );
HashStr txt_bit( "bit" );
HashStr txt_longtext( "longtext" );
HashStr txt_text( "text" );
HashStr txt_varchar( "varchar" );
HashStr txt_varbinary( "varbinary" );
HashStr txt_blob( "blob" );
HashStr txt_longblob( "longblob" );
HashStr txt_datetime( "datetime" );

HashStr txt_YES( "YES" );
HashStr txt_auto_increment( "auto_increment" );
HashStr txt_timestamp( "timestamp" );
HashStr txt_CURRENT_TIMESTAMP( "CURRENT_TIMESTAMP" );
HashStr txt_PRI( "PRI" );

HashStr txt_id( "id" );
HashStr txt_enum_name( "enum_name" );
HashStr txt_enum_desc( "enum_desc" );




DbDataTypes GetDataType( char const* dataType, char const* columnType )
{
    HashStr dt( dataType );
    HashStr ct( columnType );

    if( ct.len >= txt_unsigned.len
        && memcmp( txt_unsigned.buf, columnType + ( ct.len - txt_unsigned.len ), txt_unsigned.len ) )
    {
        if( dt == txt_bigint )
            return DbDataTypes::UInt64;
        if( dt == txt_int )
            return DbDataTypes::UInt32;
        if( dt == txt_mediumint )
            return DbDataTypes::UInt32;
        if( dt == txt_smallint )
            return DbDataTypes::UInt16;
        if( dt == txt_tinyint )
            return DbDataTypes::UInt8;
    }
    else
    {
        if( dt == txt_bigint )
            return DbDataTypes::Int64;
        if( dt == txt_int )
            return DbDataTypes::Int32;
        if( dt == txt_mediumint )
            return DbDataTypes::Int32;
        if( dt == txt_smallint )
            return DbDataTypes::Int16;
        if( dt == txt_tinyint )
            return DbDataTypes::Int8;
    }

    if( dt == txt_double )
        return DbDataTypes::Double;

    if( dt == txt_float )
        return DbDataTypes::Float;

    if( dt == txt_bit )
        return DbDataTypes::Boolean;

    if( dt == txt_longtext
        || dt == txt_text
        || dt == txt_varchar )
        return DbDataTypes::String;

    if( dt == txt_varbinary
        || dt == txt_blob
        || dt == txt_longblob )
        return DbDataTypes::Bytes;

    if( dt == txt_datetime )
        return DbDataTypes::DateTime;

    throw std::exception( "unknown/unhandled data type" );
    return DbDataTypes::Unknown;
}

char* GetCppTypeName( DbDataTypes t )
{
    switch( t )
    {
    case DbDataTypes::Boolean: return "bool";
    case DbDataTypes::Int8: return "int8";
    case DbDataTypes::Int16: return "int16";
    case DbDataTypes::Int32: return "int";
    case DbDataTypes::Int64: return "int64";
    case DbDataTypes::UInt8: return "uint8";
    case DbDataTypes::UInt16: return "uint16";
    case DbDataTypes::UInt32: return "uint";
    case DbDataTypes::UInt64: return "uint64";
    case DbDataTypes::Float: return "float";
    case DbDataTypes::Double: return "double";
    case DbDataTypes::DateTime: return "xxx::DateTime";
    case DbDataTypes::String: return "xxx::String";
    case DbDataTypes::Bytes: return "xxx::ByteBuffer";
    default: return "unknown";
    }
}


bool IsEnum( DbTable const& t )
{
    for( int i = 0; i < t.columns.Size(); ++i )
    {
        auto& c = t.columns[ i ];
        if( txt_enum_name == c.name || txt_enum_desc == c.name ) return true;
    }
    return false;
}

bool IsEmpty( String const& s )
{
    if( !s.Size() ) return true;
    for( int i = 0; i < s.Size(); ++i )
    {
        auto c = s[ i ];
        if( c != ' ' &&
            c != '\r' &&
            c != '\t' &&
            c != '\n' ) return false;
    }
    return true;
}

int main()
{
    // 表容器
    List<DbTable> tables;

    MYSQL conn, *pConn = &conn;
    if( !mysql_init( pConn ) )
    {
        return -1;
    }
    ScopeGuard sgConn( [ &] { mysql_close( pConn ); } );

    if( !mysql_real_connect( pConn, dbip, dbuser, dbpassword, dbname, dbport, nullptr, 0 ) )
    {
        return -2;
    }
    if( mysql_set_character_set( pConn, charset ) )
    {
        return -3;
    }
    String s;
    s.AppendFormat( R"(
SELECT
    TABLE_NAME, 
    TABLE_COMMENT
FROM
    INFORMATION_SCHEMA.TABLES
WHERE
    TABLE_TYPE = 'BASE TABLE' AND TABLE_SCHEMA = '{0}'
)"
, dbname );

    if( mysql_query( pConn, s.C_str() ) )
    {
        return -4;
    }

    {
        auto res = mysql_store_result( pConn );
        if( !res )
        {
            return -5;
        }
        ScopeGuard sgRes( [ &] { mysql_free_result( res ); } );

        while( auto row = mysql_fetch_row( res ) )
        {
            auto& t = tables.Emplace();
            t.name = row[ 0 ];
            t.comment = row[ 1 ];
        }
    }

    for( int i = 0; i < tables.Size(); ++i )
    {
        s.Clear();
        auto& t = tables[ i ];
        s.AppendFormat( R"(
SELECT
    COLUMN_NAME,
    DATA_TYPE,
    COLUMN_TYPE,
    IS_NULLABLE,
    COLUMN_COMMENT,
    EXTRA,
    COLUMN_DEFAULT,
    COLUMN_KEY
FROM
    INFORMATION_SCHEMA.COLUMNS
WHERE
    TABLE_SCHEMA = '{0}' AND TABLE_NAME = '{1}'
)"
, dbname, t.name );

        int n = mysql_query( pConn, s.C_str() );
        if( n )
        {
            return -6;
        }
        auto res = mysql_store_result( pConn );
        if( !res )
        {
            return -7;
        }
        ScopeGuard sgRes( [ &] { mysql_free_result( res ); } );

        while( auto row = mysql_fetch_row( res ) )
        {
            auto cn = row[ 0 ], dt = row[ 1 ], ct = row[ 2 ], in = row[ 3 ]
                , cc = row[ 4 ], e = row[ 5 ], cd = row[ 6 ], ck = row[ 7 ];

            auto& c = t.AddColumn( GetDataType( dt, ct ) );
            c.name = cn;
            c.comment = cc;
            c.nullable = ( txt_YES == in );
            c.autoIncrement = ( txt_auto_increment == e );
            c.timestamp = ( txt_timestamp == dt && txt_CURRENT_TIMESTAMP == cd );
            c.primaryKey = ( txt_PRI == ck );
        }

        // 如果是枚举表，需要读出表的数据，填充到 table
        if( IsEnum( t ) )
        {
            FillData( t, pConn );
        }
    }


    // 开始扫表容器，生成 DbTypes.h
    s.Clear();

    // 头文件 头部
    s.Append( R"#(#ifndef __DBTYPES_H__
#define __DBTYPES_H__

namespace DbTypes
{
)#" );


    String upperName;
    for( int i = 0; i < tables.Size(); ++i )
    {
        auto& t = tables[ i ];
        upperName = t.name;
        upperName.ToUpperFirstChar();

        if( IsEnum( t ) )
        {
            // 生成枚举代码段
            s.Append( R"#(
enum class )#", upperName, R"#(
{)#" );
            DbColumn* c_id = nullptr, *c_name = nullptr, *c_desc = nullptr;
            for( int j = 0; j < t.columns.Size(); ++j )
            {
                auto& c = t.columns[ j ];
                if( txt_id == c.name ) c_id = &c;
                else if( txt_enum_name == c.name ) c_name = &c;
                else if( txt_enum_desc == c.name ) c_desc = &c;
            }
            assert( c_id && c_name );
            for( int k = 0; k < t.rowCount; ++k )
            {
                auto r = t[ k ];
                auto& id = r[ *c_id ].RefValue<int>();
                upperName = r[ *c_name ].RefValue<String>();
                upperName.ToUpperFirstChar();
                s.Append( R"#(
    )#", upperName, " = ", id, "," );
                if( c_desc )
                {
                    auto& desc = r[ *c_desc ].RefValue<String>();
                    if( !IsEmpty( desc ) )
                    {
                        s.Append( " // ", desc );       // todo: 备注换行的处理啥的
                    }
                }
            }
            s.Append( R"#(
};)#" );
        }
        else
        {
            // 生成类代码段
            s.Append( R"#(
struct )#", upperName, R"#(
{)#" );
            DbColumn* c_id = nullptr, *c_name = nullptr, *c_desc = nullptr;
            for( int j = 0; j < t.columns.Size(); ++j )
            {
                auto& c = t.columns[ j ];
                s.Append( R"#(
    )#", GetCppTypeName( c.dataType ), " ", c.name, ";" );
                if( !IsEmpty( c.comment ) )
                {
                    s.Append( " // ", c.comment );      // todo: 备注换行的处理啥的
                }
            }
            s.Append( R"#(
};)#" );
        }
    }

    // 头文件 尾部
    s.Append( R"#(
}
)#" );


    Cout( s );

    // todo: 存盘
    //Dump( tables[ 0 ] );








    // 开始扫表容器，生成 DbIds.h
    s.Clear();

    // 头文件 头部
    s.Append( R"#(#ifndef __DBIDS_H__
#define __DBIDS_H__

namespace DbIds
{)#" );

    for( int i = 0; i < tables.Size(); ++i )
    {
        auto& t = tables[ i ];
        upperName = t.name;
        upperName.ToUpperFirstChar();

        if( !IsEnum( t ) )
        {
            s.Append( R"#(
    int )#", upperName, ";" );
        }
    }

    // 头文件 尾部
    s.Append( R"#(
}
)#" );


    Cout( s );











    // 开始扫表容器，生成 DbData.h
    s.Clear();

    // 头文件 头部
    s.Append( R"#(#ifndef __DBDATA_H__
#define __DBDATA_H__

namespace DbData
{)#" );

    for( int i = 0; i < tables.Size(); ++i )
    {
        auto& t = tables[ i ];
        upperName = t.name;
        upperName.ToUpperFirstChar();

        if( !IsEnum( t ) )
        {
            s.Append( R"#(
    xxx::Dict<int, ::DbTypes::)#", upperName, "*> ", t.name, ";" );
        }
    }

    // 头文件 尾部
    s.Append( R"#(
}
)#" );


    Cout( s );














    // 开始扫表容器，生成 DbUtils.h( 先不分 cpp 文件出来 )
    s.Clear();

    // 头文件 头部
    s.Append( R"#(#ifndef __DBUTILS_H__
#define __DBUTILS_H__

namespace DbUtils
{)#" );

    // 生成 Fill 系列
    for( int i = 0; i < tables.Size(); ++i )
    {
        auto& t = tables[ i ];
        upperName = t.name;
        upperName.ToUpperFirstChar();

        if( !IsEnum( t ) )
        {
            // 拼字段名列表
            String cols;
            String codes;
            for( int j = 0; j < t.columns.Size(); ++j )
            {
                auto& c = t.columns[ j ];

                if( j ) cols.Append( ", " );
                cols.Append( c.name );

                codes.AppendFormat( R"#(
            xxx::FromString( p->{0}, row[ {1} ] );)#", c.name, j );
            }

            // 参数0: 表名
            // 参数1: 字段名1,字段名2,字段名3,...
            // 参数2: 首字大写表名
            // 参数3: p->id = std::stoi( row[ 0 ] ); 多行拼接
            s.AppendFormat( R"#(
    bool Fill_{0}( MYSQL* conn )
    {{
        static char const* sql = "select {1} from {0}";
        if( mysql_real_query( conn, sql, strlen( sql ) ) )
        {{
            return false;
        }

        auto res = mysql_store_result( conn );
        if( !res )
        {{
            return false;
        }
        xxx::ScopeGuard sgRes( [ &] {{ mysql_free_result( res ); } );

        while( auto row = mysql_fetch_row( res ) )
        {{
            auto p = new DbTypes::{2}();{3}
            DbData::{0}.Push( p->id, p );
            if( p->id > DbIds::{0} ) DbIds::{0} = p->id;
        }

        return true;
    }
)#", t.name, cols, upperName, codes );

            // todo: insert, update gen

        }
    }

    // 头文件 尾部
    s.Append( R"#(
}
)#" );


    Cout( s );








    return 0;
}
