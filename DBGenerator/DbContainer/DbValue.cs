using System;
using System.Collections.Generic;

namespace DBContainer
{

    public class DbValue
    {
        public DbRow            parent          = null;
        public int              columnIndex     = 0;

        public bool IsNull()
        {
            if( !parent.parent.columns[ columnIndex ].nullable ) return false;
            return parent.parent.nullflags[ columnIndex ][ parent.rowIndex ];
        }
        public void SetNull()
        {
            if( !parent.parent.columns[ columnIndex ].nullable ) return;
            parent.parent.nullflags[ columnIndex ][ parent.rowIndex ] = true;
        }
        public void Assign<T>( T v )
        {
            var list = (List<T>)parent.parent.columnDatas[ columnIndex ];
            list[ parent.rowIndex ] = v;
            if( parent.parent.columns[ columnIndex ].nullable )
            {
                parent.parent.nullflags[ columnIndex ][ parent.rowIndex ] = false;
            }
        }

        static public explicit operator byte( DbValue v )
        {
            var list = (List<byte>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator sbyte( DbValue v )
        {
            var list = (List<sbyte>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator short( DbValue v )
        {
            var list = (List<short>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator ushort( DbValue v )
        {
            var list = (List<ushort>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator int( DbValue v )
        {
            var list = (List<int>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator uint( DbValue v )
        {
            var list = (List<uint>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator long( DbValue v )
        {
            var list = (List<long>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator ulong( DbValue v )
        {
            var list = (List<ulong>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator float( DbValue v )
        {
            var list = (List<float>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator double( DbValue v )
        {
            var list = (List<double>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator bool( DbValue v )
        {
            var list = (List<bool>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator DateTime( DbValue v )
        {
            var list = (List<DateTime>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator byte[]( DbValue v )
        {
            var list = (List<byte[]>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }
        static public explicit operator string( DbValue v )
        {
            var list = (List<string>)v.parent.parent.columnDatas[ v.columnIndex ];
            return list[ v.parent.rowIndex ];
        }

        public override string ToString()
        {
            if( IsNull() ) return "[null]";
            switch( parent.parent.columns[ columnIndex ].dataType )
            {
            case DbDataTypes.Boolean:
                return ( (List<bool>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.Int8:
                return ( (List<sbyte>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.Int16:
                return ( (List<short>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.Int32:
                return ( (List<int>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.Int64:
                return ( (List<long>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.UInt8:
                return ( (List<byte>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.UInt16:
                return ( (List<ushort>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.UInt32:
                return ( (List<uint>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.UInt64:
                return ( (List<ulong>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.Float:
                return ( (List<float>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.Double:
                return ( (List<double>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.DateTime:
                return ( (List<DateTime>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.String:
                return ( (List<string>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            case DbDataTypes.Bytes:
                return ( (List<byte[]>)parent.parent.columnDatas[ columnIndex ] )[ parent.rowIndex ].ToString();
            // todo: dump bytes
            default:
                break;
            }
            return "unknown";
        }
    }

}
