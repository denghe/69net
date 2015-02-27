using System;
using MySql.Data.MySqlClient;
using DBGenerator.Properties;
using DBContainer;
using MyHelper;

namespace DBGenerator
{
    public static class MyInfo
    {
        static Settings _cfg = Settings.Default;
        static string _connStr = new MySqlConnectionStringBuilder
        {
            Server = _cfg.server,
            Port = _cfg.port,
            UserID = _cfg.userId,
            Password = _cfg.password,
            Database = _cfg.database,
        }.ToString();

        public static DbQueryResult GetInfo()
        {
            using( var conn = new MySqlConnection( _connStr ) )
            {
                conn.Open();
                var result = new DbQueryResult();
                var _tables = result._tables;

                result._schema = _cfg.database;
                result._affectedRows = 0;

                var q = new MyQuery( "SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_SCHEMA = ", MyParameterDataTypes.String );
                q[ 0 ].Assign( _cfg.database );
                var dt = MyUtils.ExecuteDbTable( conn, q.ToSql() );

                for( int i = 0; i < dt._rowCount; ++i )
                {
                    var dr = dt[ i ];
                    _tables.Add( new DbTable
                    {
                        _name = dr[ "TABLE_NAME" ].ToString(),
                        _comment = dr[ "TABLE_COMMENT" ].ToString()
                    } );
                }

                q.Assign( "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = ", MyParameterDataTypes.String, " AND TABLE_NAME = ", MyParameterDataTypes.String );
                q[ 0 ].Assign( _cfg.database );
                foreach( var t in _tables )
                {
                    q[ 1 ].Assign( t._name );
                    dt = MyUtils.ExecuteDbTable( conn, q.ToSql() );

                    for( int i = 0; i < dt._rowCount; ++i )
                    {
                        var dr = dt[ i ];
                        var c = new DbColumn
                        {
                            _name = (string)dr[ "COLUMN_NAME" ].ToString(),
                            _columnIndex = i,
                            _dataType = GetDataType( (string)dr[ "DATA_TYPE" ], (string)dr[ "COLUMN_TYPE" ] ),
                            _nullable = (string)dr[ "IS_NULLABLE" ] == "YES",
                            _comment = (string)dr[ "COLUMN_COMMENT" ],
                            _autoIncrement = (string)dr[ "EXTRA" ] == "auto_increment",
                            _timestamp = (string)dr[ "DATA_TYPE" ] == "timestamp" && (string)dr[ "COLUMN_DEFAULT" ] == "CURRENT_TIMESTAMP",
                            _primaryKey = (string)dr[ "COLUMN_KEY" ] == "PRI",
                            _parent = t
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
