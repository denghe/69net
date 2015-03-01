using System;
using MySql.Data.MySqlClient;
using DBGenerator.Properties;
using DBContainer;
using MyHelper;

namespace DBGenerator
{
    public static class MyInfo
    {
        static Settings cfg = Settings.Default;
        static string connStr = new MySqlConnectionStringBuilder
        {
            Server = cfg.server,
            Port = cfg.port,
            UserID = cfg.userId,
            Password = cfg.password,
            Database = cfg.database,
        }.ToString();

        public static DbQueryResult GetInfo()
        {
            using( var conn = new MySqlConnection( connStr ) )
            {
                conn.Open();
                var result = new DbQueryResult();
                var _tables = result.tables;
                result.affectedRows = 0;

                result.schema = cfg.database;

                var q = new MyQuery( "SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_SCHEMA = ", MyParameterDataTypes.String );
                q[ 0 ].Assign( cfg.database );
                var dt = MyUtils.ExecuteDbTable( conn, q.ToSql() );

                for( int i = 0; i < dt.rowCount; ++i )
                {
                    var dr = dt[ i ];
                    _tables.Add( new DbTable
                    {
                        name = dr[ "TABLE_NAME" ].ToString(),
                        comment = dr[ "TABLE_COMMENT" ].ToString()
                    } );
                }

                q.Assign( "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = ", MyParameterDataTypes.String, " AND TABLE_NAME = ", MyParameterDataTypes.String );
                q[ 0 ].Assign( cfg.database );
                foreach( var t in _tables )
                {
                    q[ 1 ].Assign( t.name );
                    dt = MyUtils.ExecuteDbTable( conn, q.ToSql() );

                    for( int i = 0; i < dt.rowCount; ++i )
                    {
                        var dr = dt[ i ];
                        var c = new DbColumn
                        {
                            name = (string)dr[ "COLUMN_NAME" ].ToString(),
                            columnIndex = i,
                            dataType = GetDataType( (string)dr[ "DATA_TYPE" ], (string)dr[ "COLUMN_TYPE" ] ),
                            nullable = (string)dr[ "IS_NULLABLE" ] == "YES",
                            comment = (string)dr[ "COLUMN_COMMENT" ],
                            autoIncrement = (string)dr[ "EXTRA" ] == "auto_increment",
                            timestamp = (string)dr[ "DATA_TYPE" ] == "timestamp" && (string)dr[ "COLUMN_DEFAULT" ] == "CURRENT_TIMESTAMP",
                            primaryKey = (string)dr[ "COLUMN_KEY" ] == "PRI",
                            parent = t
                        };
                        t.AddColumn( c );
                    }
                }

                return result;
            }

        }

        public static DbDataTypes GetDataType( string dataType, string columnType )
        {
            switch( dataType )
            {
            case "bit":
                return DbDataTypes.Boolean;
            case "longtext":
            case "text":
            case "varchar":
                return DbDataTypes.String;
            case "varbinary":
            case "blob":
            case "longblob":
                return DbDataTypes.Bytes;
            case "datetime":
                return DbDataTypes.DateTime;
            case "double":
                return DbDataTypes.Double;
            case "float":
                return DbDataTypes.Float;
            case "bigint":
                if( columnType.LastIndexOf( "unsigned" ) > 0 ) return DbDataTypes.UInt64;
                else return DbDataTypes.Int64;
            case "int":
                if( columnType.LastIndexOf( "unsigned" ) > 0 ) return DbDataTypes.UInt32;
                else return DbDataTypes.Int32;
            case "mediumint":
                if( columnType.LastIndexOf( "unsigned" ) > 0 ) return DbDataTypes.UInt32;
                else return DbDataTypes.Int32;
            case "smallint":
                if( columnType.LastIndexOf( "unsigned" ) > 0 ) return DbDataTypes.UInt16;
                else return DbDataTypes.Int16;
            case "tinyint":
                if( columnType.LastIndexOf( "unsigned" ) > 0 ) return DbDataTypes.UInt8;
                else return DbDataTypes.Int8;
            default:
                throw new Exception( "unknown data type: " + dataType );
            }
        }


    }
}
