using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;

namespace DBContainer
{
    public partial class DbTable
    {
        public string           _name           = "";
        public string           _comment        = "";
        public List<object>     _columnDatas    = new List<object>();
        public List<DbColumn>   _columns        = new List<DbColumn>();
        public List<List<bool>> _nullflags      = new List<List<bool>>();
        public int              _rowCount       = 0;

        public List<DbColumn> _primaryKeys
        {
            get { return _columns.Where( c => c._primaryKey ).ToList(); }
        }

        public DbTable AddColumn( DbColumn c )
        {
            _columns.Add( c );
            _nullflags.Add( new List<bool>() );

            switch( c._dataType )
            {
            case DbDataTypes.Boolean:
                _columnDatas.Add( new List<bool>() ); break;
            case DbDataTypes.Int8:
                _columnDatas.Add( new List<sbyte>() ); break;
            case DbDataTypes.Int16:
                _columnDatas.Add( new List<short>() ); break;
            case DbDataTypes.Int32:
                _columnDatas.Add( new List<int>() ); break;
            case DbDataTypes.Int64:
                _columnDatas.Add( new List<long>() ); break;
            case DbDataTypes.UInt8:
                _columnDatas.Add( new List<byte>() ); break;
            case DbDataTypes.UInt16:
                _columnDatas.Add( new List<ushort>() ); break;
            case DbDataTypes.UInt32:
                _columnDatas.Add( new List<uint>() ); break;
            case DbDataTypes.UInt64:
                _columnDatas.Add( new List<ulong>() ); break;
            case DbDataTypes.Float:
                _columnDatas.Add( new List<float>() ); break;
            case DbDataTypes.Double:
                _columnDatas.Add( new List<double>() ); break;
            case DbDataTypes.DateTime:
                _columnDatas.Add( new List<DateTime>() ); break;
            case DbDataTypes.String:
                _columnDatas.Add( new List<string>() ); break;
            case DbDataTypes.Bytes:
                _columnDatas.Add( new List<byte[]>() ); break;
            default:
                break;
            }

            return this;
        }

        public DbRow AddRow()
        {
            foreach( var col in _columns )
            {
                switch( col._dataType )
                {
                case DbDataTypes.Boolean:
                    ( (List<bool>)_columnDatas[ col._columnIndex ] ).Add( default( bool ) ); break;
                case DbDataTypes.Int8:
                    ( (List<sbyte>)_columnDatas[ col._columnIndex ] ).Add( default( sbyte ) ); break;
                case DbDataTypes.Int16:
                    ( (List<short>)_columnDatas[ col._columnIndex ] ).Add( default( short ) ); break;
                case DbDataTypes.Int32:
                    ( (List<int>)_columnDatas[ col._columnIndex ] ).Add( default( int ) ); break;
                case DbDataTypes.Int64:
                    ( (List<long>)_columnDatas[ col._columnIndex ] ).Add( default( long ) ); break;
                case DbDataTypes.UInt8:
                    ( (List<byte>)_columnDatas[ col._columnIndex ] ).Add( default( byte ) ); break;
                case DbDataTypes.UInt16:
                    ( (List<ushort>)_columnDatas[ col._columnIndex ] ).Add( default( ushort ) ); break;
                case DbDataTypes.UInt32:
                    ( (List<uint>)_columnDatas[ col._columnIndex ] ).Add( default( uint ) ); break;
                case DbDataTypes.UInt64:
                    ( (List<ulong>)_columnDatas[ col._columnIndex ] ).Add( default( ulong ) ); break;
                case DbDataTypes.Float:
                    ( (List<float>)_columnDatas[ col._columnIndex ] ).Add( default( float ) ); break;
                case DbDataTypes.Double:
                    ( (List<double>)_columnDatas[ col._columnIndex ] ).Add( default( double ) ); break;
                case DbDataTypes.DateTime:
                    ( (List<DateTime>)_columnDatas[ col._columnIndex ] ).Add( default( DateTime ) ); break;
                case DbDataTypes.String:
                    ( (List<string>)_columnDatas[ col._columnIndex ] ).Add( default( string ) ); break;
                case DbDataTypes.Bytes:
                    ( (List<byte[]>)_columnDatas[ col._columnIndex ] ).Add( default( byte[] ) ); break;
                default:
                    break;
                }
                if( col._nullable )
                {
                    _nullflags[ col._columnIndex ].Add( true );
                }
            }
            return new DbRow { _parent = this, _rowIndex = _rowCount++ };
        }

        public DbRow this[ int rowIdx ]
        {
            get
            {
                if( rowIdx > _rowCount ) throw new Exception( "out of range: rowIdx" );
                return new DbRow { _parent = this, _rowIndex = rowIdx };
            }
        }

        public int getIndexByColumnName( string name )
        {
            return _columns.FindIndex( a => a._name == name );
        }

    }
}

