using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using MySql.Data.MySqlClient;
using DBContainer;

namespace MyHelper
{
    public static class MyUtils
    {
        #region type ToSql

        public static string ToSql( this byte[] buf )
        {
            var sb = new StringBuilder( "0x" );
            foreach( var b in buf )
            {
                sb.Append( b.ToString( "x2" ) );
            }
            return sb.ToString();
        }

        public static string ToSql( this DateTime dt )
        {
            return dt.ToString( "yyyy-MM-dd HH:mm:ss" );
        }

        public static string ToSql( this string s )
        {
            return s.Replace( "\\", "\\\\" ).Replace( "\'", "\\'" );
        }

        public static string ToSql( this List<bool> p )
        {
            var sb = new StringBuilder( p[ 0 ] ? "1" : "0" );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + ( p[ i ] ? "1" : "0" ) );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<byte> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<sbyte> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<short> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<ushort> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<int> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<long> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<uint> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<ulong> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<double> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<float> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToString() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ] );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<DateTime> p )
        {
            var sb = new StringBuilder( p[ 0 ].ToSql() );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", " + p[ i ].ToSql() );
            }
            return sb.ToString();
        }

        public static string ToSql( this List<string> p )
        {
            var sb = new StringBuilder( "'" + p[ 0 ].ToSql() + "'" );
            for( int i = 1; i < p.Count; ++i )
            {
                sb.Append( ", '" + p[ i ].ToSql() + "'" );
            }
            return sb.ToString();
        }

        #endregion

        #region executeXxxxx 同步执行系列





        public static int ExecuteNonQuery( string connStr, string sql )
        {
            using( MySqlConnection conn = new MySqlConnection( connStr ) )
            {
                conn.Open();
                return ExecuteNonQuery( conn, sql );
            }
        }

        public static object ExecuteScalar( string connStr, string sql )
        {
            using( MySqlConnection conn = new MySqlConnection( connStr ) )
            {
                conn.Open();
                return ExecuteScalar( conn, sql );
            }
        }

        public static DbTable ExecuteDbTable( string connStr, string sql )
        {
            using( MySqlConnection conn = new MySqlConnection( connStr ) )
            {
                conn.Open();
                return ExecuteDbTable( conn, sql );
            }
        }

        public static DbQueryResult ExecuteResult( string connStr, string sql )
        {
            using( MySqlConnection conn = new MySqlConnection( connStr ) )
            {
                conn.Open();
                return ExecuteResult( conn, sql );
            }
        }






        public static int ExecuteNonQuery( MySqlConnection conn, string sql )
        {
            using( var cmd = new MySqlCommand( sql, conn ) )
            {
                return cmd.ExecuteNonQuery();
            }
        }

        public static object ExecuteScalar( MySqlConnection conn, string sql )
        {
            using( var cmd = new MySqlCommand( sql, conn ) )
            {
                return cmd.ExecuteScalar();
            }
        }

        public static DbTable ExecuteDbTable( MySqlConnection conn, string sql )
        {
            using( var cmd = new MySqlCommand( sql, conn ) )
            {
                using( var r = cmd.ExecuteReader() )
                {
                    var dt = NewDbTable( r.GetSchemaTable() );
                    if( dt != null )
                    {
                        FillDbTable( dt, r );
                    }
                    return dt;
                }
            }
        }

        public static DbQueryResult ExecuteResult( MySqlConnection conn, string sql )
        {
            var rtv = new DbQueryResult();

            using( var cmd = new MySqlCommand( sql, conn ) )
            {
                using( var r = cmd.ExecuteReader() )
                {
                    rtv.affectedRows = r.RecordsAffected;   // 这个值于多段查询来讲看上去是累计的结果
LabBegin:
                    var dt = NewDbTable( r.GetSchemaTable() );
                    if( dt != null )
                    {
                        FillDbTable( dt, r );
                        rtv.tables.Add( dt );
                    }
                    if( r.NextResult() ) goto LabBegin;
                }
            }
            return rtv;
        }

        // 将 SchemaTable 的数据转为 DbTable 的字段声明，创建 DbTable, 填充 _columns 并返回
        public static DbTable NewDbTable( DataTable t )
        {
            if( t == null ) return null;
            // 还可能关注 ColumnOrdinal, ColumnSize. ColumnOrdinal 是以 1 为起始的字段位置
            var dt = new DbTable();
            var column_ColumnName = t.Columns[ "ColumnName" ];
            var column_DataType = t.Columns[ "DataType" ];
            var column_AllowDBNull = t.Columns[ "AllowDBNull" ];
            var column_ColumnSize = t.Columns[ "ColumnSize" ];
            var column_ColumnOrdinal = t.Columns[ "ColumnOrdinal" ];
            foreach( DataRow row in t.Rows )
            {
                var cn = (string)row[ column_ColumnName ];
                var ca = (bool)row[ column_AllowDBNull ];
                var ct = (Type)row[ column_DataType ];
                var idx = (int)row[ column_ColumnOrdinal ] - 1;

                if( ct.Name == "UInt64" && (int)row[ column_ColumnSize ] == 1 )
                {
                    dt.AddColumn( new DbColumn { name = cn, dataType = DbDataTypes.Boolean, nullable = ca, columnIndex = idx } );
                }
                else
                {
                    dt.AddColumn( new DbColumn { name = cn, dataType = GetDbType( ct ), nullable = ca, columnIndex = idx } );
                }
            }
            return dt;
        }

        // 填充 rows
        public static void FillDbTable( DbTable dt, MySqlDataReader r )
        {
            while( r.Read() )
            {
                var dr = dt.AddRow();
                for( int i = 0; i < r.FieldCount; ++i )
                {
                    if( dt.columns[ i ].nullable && r.IsDBNull( i ) )
                    {
                        dr[ i ].SetNull();
                    }
                    else
                    {
                        FillValue( dr, r, i );
                    }
                }
            }
        }

        // 往 dr 填充 r 于 i 索引的值
        public static void FillValue( DbRow dr, MySqlDataReader r, int i )
        {
            switch( dr.parent.columns[ i ].dataType )
            {
            case DbDataTypes.Boolean:
                dr[ i ].Assign( r.GetBoolean( i ) ); break;
            case DbDataTypes.Int8:
                dr[ i ].Assign( r.GetSByte( i ) ); break;
            case DbDataTypes.Int16:
                dr[ i ].Assign( r.GetInt16( i ) ); break;
            case DbDataTypes.Int32:
                dr[ i ].Assign( r.GetInt32( i ) ); break;
            case DbDataTypes.Int64:
                dr[ i ].Assign( r.GetInt64( i ) ); break;
            case DbDataTypes.UInt8:
                dr[ i ].Assign( r.GetByte( i ) ); break;
            case DbDataTypes.UInt16:
                dr[ i ].Assign( r.GetUInt16( i ) ); break;
            case DbDataTypes.UInt32:
                dr[ i ].Assign( r.GetUInt32( i ) ); break;
            case DbDataTypes.UInt64:
                dr[ i ].Assign( r.GetUInt64( i ) ); break;
            case DbDataTypes.Float:
                dr[ i ].Assign( r.GetFloat( i ) ); break;
            case DbDataTypes.Double:
                dr[ i ].Assign( r.GetDouble( i ) ); break;
            case DbDataTypes.DateTime:
                dr[ i ].Assign( r.GetDateTime( i ) ); break;
            case DbDataTypes.String:
                dr[ i ].Assign( r.GetString( i ) ); break;
            case DbDataTypes.Bytes:
                var len = (int)r.GetBytes( i, 0, null, 0, 0 );
                var buf = new byte[ len ];
                r.GetBytes( i, 0, buf, 0, len );
                dr[ i ].Assign( buf );
                break;
            default:
                break;
            }
        }



        // 返回 Type 对应的 DbTypes 枚举
        public static DbDataTypes GetDbType( Type t )
        {
            switch( t.Name )
            {
            case "Boolean": return DbDataTypes.Boolean;
            case "SByte": return DbDataTypes.Int8;
            case "Int16": return DbDataTypes.Int16;
            case "Int32": return DbDataTypes.Int32;
            case "Int64": return DbDataTypes.Int64;
            case "Byte": return DbDataTypes.UInt8;
            case "UInt16": return DbDataTypes.UInt16;
            case "UInt32": return DbDataTypes.UInt32;
            case "UInt64": return DbDataTypes.UInt64;
            case "Single": return DbDataTypes.Float;
            case "Double": return DbDataTypes.Double;
            case "DateTime": return DbDataTypes.DateTime;
            case "String": return DbDataTypes.String;
            case "Byte[]": return DbDataTypes.Bytes;
            default:
                throw new Exception( "not support type: " + t.Name );
            }
        }





        // 取 dr 中某字段的数据并返回
        public static object GetValue( DbColumn col, MySqlDataReader r, int idx )
        {
            switch( col.dataType )
            {
            case DbDataTypes.Boolean:
                return r.GetBoolean( idx );
            case DbDataTypes.Int8:
                return r.GetSByte( idx );
            case DbDataTypes.Int16:
                return r.GetInt16( idx );
            case DbDataTypes.Int32:
                return r.GetInt32( idx );
            case DbDataTypes.Int64:
                return r.GetInt64( idx );
            case DbDataTypes.UInt8:
                return r.GetByte( idx );
            case DbDataTypes.UInt16:
                return r.GetUInt16( idx );
            case DbDataTypes.UInt32:
                return r.GetUInt32( idx );
            case DbDataTypes.UInt64:
                return r.GetUInt64( idx );
            case DbDataTypes.Float:
                return r.GetFloat( idx );
            case DbDataTypes.Double:
                return r.GetDouble( idx );
            case DbDataTypes.DateTime:
                return r.GetDateTime( idx );
            case DbDataTypes.String:
                return r.GetString( idx );
            case DbDataTypes.Bytes:
                var len = (int)r.GetBytes( idx, 0, null, 0, 0 );
                var buf = new byte[ len ];
                r.GetBytes( idx, 0, buf, 0, len );
                return buf;
            default:
                throw new Exception( "unsupported DbType" );
            }
        }






        #endregion
    }
}
