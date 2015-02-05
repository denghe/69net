//#include "Sqlite.h"
//#include <iostream>
//
//int main()
//{
//    Sqlite::Connection conn( "D:\\erere.db" );
//    conn.open();
//    conn.execute( "DROP TABLE IF EXISTS 'ttt'" );
//    conn.execute( R"--(CREATE TABLE "ttt" (
//"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
//"name" TEXT NOT NULL, 
//"double" REAL
//);)--" );
//
//    conn.beginTransaction();
//    auto& q = *conn.newQuery( "INSERT INTO `ttt` ( `name`, `double` ) VALUES ( ?, ? )" );
//    q.add( "eeeeeee" );
//    q.add( 123.4 );
//    q.execute();
//    q.add( "qwer" );
//    q.add( 11.0 );
//    q.execute();
//    q.add( "zxcv" );
//    q.add( ( char const* )nullptr );
//    q.execute();
//    conn.commit();
//
//    q.assign( "SELECT `id`,`name`,`double` FROM `ttt`" );
//    q.execute( []( Sqlite::DataReader& dr )
//    {
//        std::cout << dr.readInt32();
//        std::cout << ", " << dr.readString();
//        std::cout << ", " << dr.readDouble();
//        std::cout << std::endl;
//    } );
//
//    conn.close();
//
//
//    return 0;
//}
