using System;
using System.Collections.Generic;

namespace MyHelper
{

    /// <summary>
    /// Query 参数
    /// </summary>
    public class MyParameter
    {
        public MyParameterDataTypes     _type;
        public int                      _pos;
        public string                   _content;

        #region single assign

        public void Assign( bool p )
        {
            if( _type != MyParameterDataTypes.Boolean ) throw new Exception( "wrong parm type" );
            _content = p ? "1" : "0";
        }
        public void Assign( bool? p )
        {
            if( _type != MyParameterDataTypes.Boolean ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value ? "1" : "0";
        }

        public void Assign( byte p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( byte? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( sbyte p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( sbyte? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( short p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( short? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( ushort p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( ushort? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( int p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( int? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( long p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( long? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( uint p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( uint? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( ulong p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( ulong? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( double p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( double? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( float p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            _content = p.ToString();
        }
        public void Assign( float? p )
        {
            if( _type != MyParameterDataTypes.Numeric ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = p.Value.ToString();
        }

        public void Assign( DateTime p )
        {
            if( _type != MyParameterDataTypes.DateTime ) throw new Exception( "wrong parm type" );
            _content = "'" + p.ToSql() + "'";
        }
        public void Assign( DateTime? p )
        {
            if( _type != MyParameterDataTypes.DateTime ) throw new Exception( "wrong parm type" );
            if( !p.HasValue ) _content = "null";
            else _content = "'" + p.Value.ToSql() + "'";
        }

        public void Assign( string p )
        {
            if( _type != MyParameterDataTypes.String && _type != MyParameterDataTypes.StringPart ) throw new Exception( "wrong parm type" );
            if( p == null )
            {
                _content = "null";
            }
            else if( _type == MyParameterDataTypes.String )
            {
                _content = "'" + p.ToSql() + "'";
            }
            else
            {
                _content = p.ToSql();
            }
        }

        public void Assign( byte[] p )
        {
            if( _type != MyParameterDataTypes.Binary ) throw new Exception( "wrong parm type" );
            if( p == null ) _content = "null";
            else _content = p.ToSql();
        }

        public void AssignNull()
        {
            _content = "null";
        }


        /// <summary>
        /// 不检查参数类型，直接附加一个字串
        /// </summary>
        public void AssignCustom( string p )
        {
            _content = p ?? "null";
        }

        #endregion

        #region list assign

        public void Assign( List<bool> p )
        {
            if( _type != MyParameterDataTypes.Boolean ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<byte> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<sbyte> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<short> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<ushort> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<int> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<long> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<uint> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<ulong> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<double> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<float> p )
        {
            if( _type != MyParameterDataTypes.Numerics ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<DateTime> p )
        {
            if( _type != MyParameterDataTypes.DateTimes ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        public void Assign( List<string> p )
        {
            if( _type != MyParameterDataTypes.Strings ) throw new Exception( "wrong parm type" );
            if( p == null || p.Count == 0 ) throw new Exception( "list parm can't be null or 0 items" );
            _content = p.ToSql();
        }

        #endregion
    }

}
