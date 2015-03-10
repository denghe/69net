#include "Lib/All.h"
using namespace xxx;

#include "mysql.h"

char* dbuser = "sa";
char* dbpassword = "1111";
char* dbip = "192.168.1.244";
short dbport = 3306;
char* dbname = "dd";
char* charset = "utf8";

int main()
{
    //// 表容器
    //List<DbTable> tables;


    //MYSQL conn, *pConn = &conn;
    //if( !mysql_init( pConn ) )
    //{
    //    return -1;
    //}
    //ScopeGuard sgConn( [ &] { mysql_close( pConn ); } );

    //if( !mysql_real_connect( pConn, dbip, dbuser, dbpassword, dbname, dbport, nullptr, 0 ) )
    //{
    //    return -2;
    //}
    //if( mysql_set_character_set( pConn, charset ) )
    //{
    //    return -3;
    //}

    // todo: append format 会崩

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

//    if( mysql_query( pConn, s.C_str() ) != 0 )
//    {
//        return -4;
//    }
//
//    auto res = mysql_store_result( pConn );
//    if( !res )
//    {
//        return -5;
//    }
//    ScopeGuard sgResult( [ &] { mysql_free_result( res ); } );
//
//    while( auto row = mysql_fetch_row( res ) )
//    {
//        auto& t = tables.Emplace();
//        t.name = row[ 0 ];
//        t.comment = row[ 1 ];
//    }
//    sgResult.RunAndCancel();
//
//    s.Clear();
//    for( int i = 0; i < tables.Size(); ++i )
//    {
//        auto& t = tables[ i ];
//        s.AppendFormat( R"(
//SELECT
//    COLUMN_NAME,
//    DATA_TYPE,
//    COLUMN_TYPE,
//    IS_NULLABLE,
//    COLUMN_COMMENT,
//    EXTRA,
//    COLUMN_DEFAULT,
//    COLUMN_KEY
//FROM
//    INFORMATION_SCHEMA.COLUMNS
//WHERE
//    TABLE_SCHEMA = '{0}' AND TABLE_NAME = '{1}'
//)"
//, dbname, t.name );
//
//        if( mysql_query( pConn, s.C_str() ) != 0 )
//        {
//            return -6;
//        }
//        res = mysql_store_result( pConn );
//        if( !res )
//        {
//            return -7;
//        }
//        sgResult.Set( [ &] { mysql_free_result( res ); } );
//
//        // ...
//
//        sgResult.RunAndCancel();
//    }
//
//    sgConn.RunAndCancel();
//
//    //Dump( tables );
//    Cout( tables[ 0 ].name );
//
    // todo: 补 mysql 查询结果到 DbTable 的转换填充 Utils
    return 0;
}
