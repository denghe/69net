using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DBContainer;

namespace MyHelper
{
    public class MyQuery
    {
        public Action<DbQueryResult>    _callback   = null;
        protected StringBuilder         _content    = new StringBuilder();
        protected List<MyParameter>     _parms      = new List<MyParameter>();
        protected char                  _lastAppend = ' ';                      // 用来存上一个参数的类型，以便添加逗号什么的

        public bool _empty { get { return _content.Length == 0 && _parms.Count == 0; } }

        public void Clear()
        {
            _content.Clear();
            _parms.Clear();
            _callback = null;
            _lastAppend = ' ';
        }

        public MyQuery Clone()
        {
            var rtv = new MyQuery();
            rtv._content.Append( _content );
            foreach( var p in _parms )
            {
                rtv._parms.Add( new MyParameter
                {
                    _type = p._type,
                    _pos = p._pos,
                    _content = p._content
                } );
            }
            return rtv;
        }

        public MyQuery()
        {
        }

        public MyQuery( params object[] ps )
        {
            Assign( ps );
        }

        public void Assign( params object[] ps )
        {
            _content.Clear();
            _parms.Clear();

            Append( ps );
        }

        public MyQuery AppendExceptTableName( params DbColumn[] ps )
        {
            for( int i = 0; i < ps.Length; ++i )
            {
                var p = ps[ i ];
                if( _lastAppend == 'c' ) _content.Append( ", " );
                else _lastAppend = 'c';
                _content.Append( "`" + p.name + "`" );
            }
            return this;
        }


        public MyQuery Append( params object[] ps )
        {
            for( int i = 0; i < ps.Length; ++i )
            {
                var p = ps[ i ];
                if( p is DbTable )
                {
                    if( _lastAppend == 't' ) _content.Append( ", " );
                    else _lastAppend = 't';
                    _content.Append( "`" + ( (DbTable)p ).name + "`" );
                }
                else if( p is DbColumn )
                {
                    if( _lastAppend == 'c' ) _content.Append( ", " );
                    else _lastAppend = 'c';
                    _content.Append( "`" + ( (DbColumn)p ).parent.name + "`.`" + ( (DbColumn)p ).name + "`" );
                }
                else if( p is MyParameterDataTypes )
                {
                    if( _lastAppend == 'p' ) _content.Append( ", " );
                    else _lastAppend = 'p';
                    _parms.Add( new MyParameter() { _type = (MyParameterDataTypes)p, _pos = _content.Length, _content = "" } );
                }
                else if( p is string )
                {
                    _content.Append( p );
                    if( _lastAppend != 's' ) _lastAppend = 's';
                }
                else if( p is DateTime )
                {
                    if( _lastAppend == 'd' ) _content.Append( ", " );
                    else _lastAppend = 'd';
                    _content.Append( "'" + ( (DateTime)p ).ToString( "yyyy-MM-dd HH:mm:ss" ) + "'" );
                }
                else if( p is int )
                {
                    if( _lastAppend == 'i' ) _content.Append( ", " );
                    else _lastAppend = 'i';
                    _content.Append( p.ToString() );
                }
                else if( p is MyQuery )
                {
                    var q = (MyQuery)p;
                    var offset = _content.Length;
                    _content.Append( q._content );
                    foreach( var qp in q._parms )
                    {
                        _parms.Add( new MyParameter
                        {
                            _type = qp._type,
                            _pos = qp._pos + offset,
                            _content = qp._content
                        } );
                    }
                }
                else
                {
                    _content.Append( p.ToString() );
                    _lastAppend = ' ';
                }
            }
            return this;
        }

        public MyParameter this[ int idx ]
        {
            get
            {
                if( idx >= _parms.Count ) throw new Exception( "out of the range: parms" );
                return _parms[ idx ];
            }
        }

        public string ToSql()
        {
            string s = _content.ToString();
            var sb = new StringBuilder();
            int offset = 0;
            foreach( var p in _parms )
            {
                sb.Append( s.Substring( offset, p._pos - offset ) );
                offset = p._pos;
                sb.Append( p._content );
            }
            if( offset < s.Length )
            {
                sb.Append( s.Substring( offset ) );
            }
            return sb.ToString();
        }

    }

}
