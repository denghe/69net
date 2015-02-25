using System;
using System.Collections.Generic;

namespace DBContainer
{

    public partial class DbRow
    {
        public DbTable          _parent         = null;
        public int              _rowIndex       = 0;

        public DbValue this[ int colIdx ]
        {
            get
            {
                return new DbValue { _parent = this, _columnIndex = colIdx };
            }
        }
        public DbValue this[ string colName ]
        {
            get
            {
                return new DbValue { _parent = this, _columnIndex = _parent.getIndexByColumnName( colName ) };
            }
        }

    }

    public class DbValue
    {
        public DbRow            _parent         = null;
        public int              _columnIndex    = 0;

        public bool IsNull()
        {
            if( !_parent._parent._columns[ _columnIndex ]._nullable ) return false;
            return _parent._parent._nullflags[ _columnIndex ][ _parent._rowIndex ];
        }
        public void SetNull()
        {
            if( !_parent._parent._columns[ _columnIndex ]._nullable ) return;
            _parent._parent._nullflags[ _columnIndex ][ _parent._rowIndex ] = true;
        }
        public void Assign<T>( T v )
        {
            var list = (List<T>)_parent._parent._columnDatas[ _columnIndex ];
            list[ _parent._rowIndex ] = v;
            if( _parent._parent._columns[ _columnIndex ]._nullable )
            {
                _parent._parent._nullflags[ _columnIndex ][ _parent._rowIndex ] = false;
            }
        }

        static public explicit operator byte( DbValue v )
        {
            var list = (List<byte>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator sbyte( DbValue v )
        {
            var list = (List<sbyte>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator short( DbValue v )
        {
            var list = (List<short>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator ushort( DbValue v )
        {
            var list = (List<ushort>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator int( DbValue v )
        {
            var list = (List<int>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator uint( DbValue v )
        {
            var list = (List<uint>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator long( DbValue v )
        {
            var list = (List<long>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator ulong( DbValue v )
        {
            var list = (List<ulong>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator float( DbValue v )
        {
            var list = (List<float>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator double( DbValue v )
        {
            var list = (List<double>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator bool( DbValue v )
        {
            var list = (List<bool>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator DateTime( DbValue v )
        {
            var list = (List<DateTime>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator byte[]( DbValue v )
        {
            var list = (List<byte[]>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }
        static public explicit operator string( DbValue v )
        {
            var list = (List<string>)v._parent._parent._columnDatas[ v._columnIndex ];
            return list[ v._parent._rowIndex ];
        }

        public override string ToString()
        {
            if( IsNull() ) return "[null]";
            switch( _parent._parent._columns[ _columnIndex ]._dataType )
            {
            case DbDataTypes.Boolean:
                return ( (List<bool>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.Int8:
                return ( (List<sbyte>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.Int16:
                return ( (List<short>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.Int32:
                return ( (List<int>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.Int64:
                return ( (List<long>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.UInt8:
                return ( (List<byte>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.UInt16:
                return ( (List<ushort>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.UInt32:
                return ( (List<uint>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.UInt64:
                return ( (List<ulong>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.Float:
                return ( (List<float>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.Double:
                return ( (List<double>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.DateTime:
                return ( (List<DateTime>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.String:
                return ( (List<string>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            case DbDataTypes.Bytes:
                return ( (List<byte[]>)_parent._parent._columnDatas[ _columnIndex ] )[ _parent._rowIndex ].ToString();
            // todo: dump bytes
            default:
                break;
            }
            return "unknown";
        }
    }

}
