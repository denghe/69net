using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;

namespace DBContainer
{
    public partial class DbTable
    {
        public string           name            = "";
        public string           comment         = "";
        public List<object>     columnDatas     = new List<object>();
        public List<DbColumn>   columns         = new List<DbColumn>();
        public List<List<bool>> nullflags       = new List<List<bool>>();
        public int              rowCount        = 0;

        public List<DbColumn> primaryKeys
        {
            get { return columns.Where( c => c.primaryKey ).ToList(); }
        }

        public DbTable AddColumn( DbColumn c )
        {
            columns.Add( c );
            nullflags.Add( new List<bool>() );

            switch( c.dataType )
            {
            case DbDataTypes.Boolean:
                columnDatas.Add( new List<bool>() ); break;
            case DbDataTypes.Int8:
                columnDatas.Add( new List<sbyte>() ); break;
            case DbDataTypes.Int16:
                columnDatas.Add( new List<short>() ); break;
            case DbDataTypes.Int32:
                columnDatas.Add( new List<int>() ); break;
            case DbDataTypes.Int64:
                columnDatas.Add( new List<long>() ); break;
            case DbDataTypes.UInt8:
                columnDatas.Add( new List<byte>() ); break;
            case DbDataTypes.UInt16:
                columnDatas.Add( new List<ushort>() ); break;
            case DbDataTypes.UInt32:
                columnDatas.Add( new List<uint>() ); break;
            case DbDataTypes.UInt64:
                columnDatas.Add( new List<ulong>() ); break;
            case DbDataTypes.Float:
                columnDatas.Add( new List<float>() ); break;
            case DbDataTypes.Double:
                columnDatas.Add( new List<double>() ); break;
            case DbDataTypes.DateTime:
                columnDatas.Add( new List<DateTime>() ); break;
            case DbDataTypes.String:
                columnDatas.Add( new List<string>() ); break;
            case DbDataTypes.Bytes:
                columnDatas.Add( new List<byte[]>() ); break;
            default:
                break;
            }

            return this;
        }

        public DbRow AddRow()
        {
            foreach( var col in columns )
            {
                switch( col.dataType )
                {
                case DbDataTypes.Boolean:
                    ( (List<bool>)columnDatas[ col.columnIndex ] ).Add( default( bool ) ); break;
                case DbDataTypes.Int8:
                    ( (List<sbyte>)columnDatas[ col.columnIndex ] ).Add( default( sbyte ) ); break;
                case DbDataTypes.Int16:
                    ( (List<short>)columnDatas[ col.columnIndex ] ).Add( default( short ) ); break;
                case DbDataTypes.Int32:
                    ( (List<int>)columnDatas[ col.columnIndex ] ).Add( default( int ) ); break;
                case DbDataTypes.Int64:
                    ( (List<long>)columnDatas[ col.columnIndex ] ).Add( default( long ) ); break;
                case DbDataTypes.UInt8:
                    ( (List<byte>)columnDatas[ col.columnIndex ] ).Add( default( byte ) ); break;
                case DbDataTypes.UInt16:
                    ( (List<ushort>)columnDatas[ col.columnIndex ] ).Add( default( ushort ) ); break;
                case DbDataTypes.UInt32:
                    ( (List<uint>)columnDatas[ col.columnIndex ] ).Add( default( uint ) ); break;
                case DbDataTypes.UInt64:
                    ( (List<ulong>)columnDatas[ col.columnIndex ] ).Add( default( ulong ) ); break;
                case DbDataTypes.Float:
                    ( (List<float>)columnDatas[ col.columnIndex ] ).Add( default( float ) ); break;
                case DbDataTypes.Double:
                    ( (List<double>)columnDatas[ col.columnIndex ] ).Add( default( double ) ); break;
                case DbDataTypes.DateTime:
                    ( (List<DateTime>)columnDatas[ col.columnIndex ] ).Add( default( DateTime ) ); break;
                case DbDataTypes.String:
                    ( (List<string>)columnDatas[ col.columnIndex ] ).Add( default( string ) ); break;
                case DbDataTypes.Bytes:
                    ( (List<byte[]>)columnDatas[ col.columnIndex ] ).Add( default( byte[] ) ); break;
                default:
                    break;
                }
                if( col.nullable )
                {
                    nullflags[ col.columnIndex ].Add( true );
                }
            }
            return new DbRow { parent = this, rowIndex = rowCount++ };
        }

        public DbRow this[ int rowIdx ]
        {
            get
            {
                if( rowIdx > rowCount ) throw new Exception( "out of range: rowIdx" );
                return new DbRow { parent = this, rowIndex = rowIdx };
            }
        }

        public int getIndexByColumnName( string name )
        {
            return columns.FindIndex( a => a.name == name );
        }

    }
}

