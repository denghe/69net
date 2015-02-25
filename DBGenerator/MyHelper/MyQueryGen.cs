using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DBContainer;

namespace MyHelper
{

    public static class MyQueryGen
    {
        public static MyParameterDataTypes GetParmType( this DbColumn c )
        {
            switch( c._dataType )
            {
            case DbDataTypes.Boolean: return MyParameterDataTypes.Boolean;
            case DbDataTypes.Int8: return MyParameterDataTypes.Numeric;
            case DbDataTypes.Int16: return MyParameterDataTypes.Numeric;
            case DbDataTypes.Int32: return MyParameterDataTypes.Numeric;
            case DbDataTypes.Int64: return MyParameterDataTypes.Numeric;
            case DbDataTypes.UInt8: return MyParameterDataTypes.Numeric;
            case DbDataTypes.UInt16: return MyParameterDataTypes.Numeric;
            case DbDataTypes.UInt32: return MyParameterDataTypes.Numeric;
            case DbDataTypes.UInt64: return MyParameterDataTypes.Numeric;
            case DbDataTypes.Float: return MyParameterDataTypes.Numeric;
            case DbDataTypes.Double: return MyParameterDataTypes.Numeric;
            case DbDataTypes.DateTime: return MyParameterDataTypes.DateTime;
            case DbDataTypes.String: return MyParameterDataTypes.String;
            case DbDataTypes.Bytes: return MyParameterDataTypes.Binary;
            default:
                throw new Exception( "unknown data type: " + c._dataType );
            }
        }

        /// <summary>
        /// 生成 select c1, c2, ... from t
        /// 如果传入的子查询为 null，则追加生成 where pk1 = PT.type1 and pk2 = PT.type2  ...（有主键的话）
        /// 否则原样追加子查询 字串（希望 阻止生成 where 主键, 可传入 new MyQuery() ）
        /// </summary>
        /// <param name="calcFoundRows">如果使用了 limit 并希望同时返回 符合查找条件的总记录数，须传入 true</param>
        /// <param name="t">表</param>
        /// <param name="selectCols">字段列表。如果为空表示所有字段</param>
        /// <param name="sq">可追加在 from xxx 后面的查询语句</param>
        /// <param name="limit">追加最尾部的行数限制语句</param>
        /// <returns>查询</returns>
        public static MyQuery Select( bool calcFoundRows, DbTable t, IEnumerable<DbColumn> selectCols = null, MyQuery sq = null, Nullable<KeyValuePair<int, int>> limit = null )
        {
            var q = new MyQuery( "select " + ( calcFoundRows ? "SQL_CALC_FOUND_ROWS " : "" ) );
            if( selectCols == null ) selectCols = t._columns;
            foreach( var c in selectCols ) q.Append( c );
            q.Append( " from ", t );
            if( sq == null )
            {
                if( t._primaryKeys.Count > 0 )
                {
                    for( int i = 0; i < t._primaryKeys.Count; ++i )
                    {
                        q.Append( ( i == 0 ? " where " : " and " ), t._primaryKeys[ i ], " = ", t._primaryKeys[ i ].GetParmType() );
                    }
                }
            }
            else if( !sq._empty ) q.Append( sq );
            if( limit != null )
            {
                q.Append( " limit ", limit.Value.Key, ", ", limit.Value.Value );
                if( calcFoundRows )
                {
                    q.Append( "; select FOUND_ROWS();" );
                }
            }
            return q;
        }

        /// <summary>
        /// 生成 insert into t ( c1, c2, ... ) values ( PT.type1, PT.type2, ... ); select c1, c2, ... 回读，自增主键将使用 LAST_INSERT_ID() 作为条件
        /// </summary>
        /// <param name="replace">是否为替换模式</param>
        /// <param name="t">表</param>
        /// <param name="insertCols">要插入的字段列表（不能包含自增字段）。如果为空表示所有字段（跳过只读字段）</param>
        /// <param name="selectCols">回读字段列表。传入 null, 则不回读，不生成 ; select 这部分语句</param>
        /// <returns>查询</returns>
        public static MyQuery Insert( bool replace, DbTable t, IEnumerable<DbColumn> insertCols = null, IEnumerable<DbColumn> selectCols = null )
        {
            var q = new MyQuery( ( replace ? "replace" : "insert" ) + " into ", t, " ( " );
            var sq = new MyQuery( " ) values ( " );
            if( insertCols == null ) insertCols = t._columns;
            foreach( var c in insertCols )         // 跳过 AutoIncrease 字段（指定要插入哪些字段，将无视数据库默认值，除了自增不应该无视）
            {
                if( c._readonly ) continue; //throw new Exception( "只读字段无法插入" );
                q.AppendExceptTableName( c );
                sq.Append( c.GetParmType() );
            }
            q.Append( sq );
            q.Append( " )" );
            if( selectCols != null )
            {
                if( t._primaryKeys.Count == 0 ) throw new Exception( "无主键的表无法于 insert 后 select 回插入行的数据" );

                q.Append( ";" );
                if( t._primaryKeys.Count( a => a._autoIncrement ) == 0 )  // 如果主键中不含自增字段，可直接使用 Select 的拼接物来做读回
                {
                    q.Append( Select( false, t, selectCols ) );
                }
                else
                {
                    sq.Clear();
                    if( t._primaryKeys.Count > 0 )
                    {
                        for( int i = 0; i < t._primaryKeys.Count; ++i )
                        {
                            var pk = t._primaryKeys[ i ];
                            sq.Append( ( i == 0 ? " where " : " and " ), pk, " = " );
                            if( pk._autoIncrement ) sq.Append( "LAST_INSERT_ID()" );
                            else sq.Append( pk.GetParmType() );
                        }
                    }
                    q.Append( Select( false, t, selectCols, sq ) );
                }

            }
            return q;
        }

        /// <summary>
        /// 生成 insert into t ( c1, c2, ... ) values ( PT.type1, PT.type2, ... ), ( PT.typeN, ... ), ...
        /// </summary>
        /// <param name="replace">是否为替换模式</param>
        /// <param name="t">表</param>
        /// <param name="insertCols">要插入的字段列表（不能包含自增字段）。如果为空表示所有字段（跳过只读字段）</param>
        /// <returns>查询</returns>
        public static MyQuery Inserts( bool replace, DbTable t, int rowCount, IEnumerable<DbColumn> insertCols = null )
        {
            var q = new MyQuery( ( replace ? "replace" : "insert" ) + " into ", t, " ( " );
            if( insertCols == null ) insertCols = t._columns;
            foreach( var c in insertCols )
            {
                if( c._readonly ) continue;          // 跳过 AutoIncrease 字段
                q.AppendExceptTableName( c );
            }
            q.Append( " ) values " );
            for( int i = 0; i < rowCount; ++i )
            {
                if( i > 0 ) q.Append( ", " );
                q.Append( "( " );
                foreach( var c in insertCols )
                {
                    if( c._readonly ) continue;          // 跳过 AutoIncrease 字段
                    q.Append( c.GetParmType() );
                }
                q.Append( " )" );
            }
            return q;
        }



        /// <summary>
        /// 生成 update t set c1 = PT.type1, c2 = ... where pk1 = PT.type1 and pk2 = ....
        /// </summary>
        /// <param name="t">表</param>
        /// <param name="updateCols">要更新的字段列表。如果不传则为除了自增和Timespan的所有字段</param>
        /// <param name="sq">where部分的子查询。如果为 null 则按主键生成，如果 IsEmpty 将不生成 where 部分</param>
        /// <returns>查询</returns>
        public static MyQuery Update( DbTable t, IEnumerable<DbColumn> updateCols = null, MyQuery sq = null )
        {
            var q = new MyQuery( "update ", t, " set " );
            if( updateCols == null ) updateCols = t._columns;
            int i = 0;
            foreach( var c in updateCols )     // 跳过 AutoIncrease 字段
            {
                if( c._autoIncrement ) continue;
                if( i++ > 0 ) q.Append( ", " );
                q.Append( c, " = ", c.GetParmType() );
            }
            if( sq == null )
            {
                if( t._primaryKeys.Count == 0 ) throw new Exception( "无主键表无法生成更新条件" );
                i = 0;
                for( ; i < t._primaryKeys.Count; ++i )
                {
                    q.Append( ( i == 0 ? " where " : " and " ), t._primaryKeys[ i ], " = ", t._primaryKeys[ i ].GetParmType() );
                }
            }
            else if( !sq._empty ) q.Append( sq );
            return q;
        }

        /// <summary>
        /// 生成 delete from t where pk1 = PT.type1 and pk2 = ....
        /// </summary>
        /// <param name="t"></param>
        /// <param name="sq"></param>
        /// <returns></returns>
        public static MyQuery Delete( DbTable t, MyQuery sq = null )
        {
            var q = new MyQuery( "delete from ", t );
            if( sq == null )
            {
                if( t._primaryKeys.Count == 0 ) throw new Exception( "无主键表无法生成删除条件" );

                for( int i = 0; i < t._primaryKeys.Count; ++i )
                {
                    q.Append( ( i == 0 ? " where " : " and " ), t._primaryKeys[ i ], " = ", t._primaryKeys[ i ].GetParmType() );
                }
            }
            else if( !sq._empty ) q.Append( sq );
            return q;
        }


        // todo: Min, Max
        // todo: Exists

        /// <summary>
        /// 生成 select count( distinct colName ) from xxxx where .....
        /// </summary>
        /// <param name="t"></param>
        /// <param name="col"></param>
        /// <param name="distinct"></param>
        /// <param name="sq"></param>
        /// <returns></returns>
        public static MyQuery Count( DbTable t, DbColumn col = null, bool distinct = false, MyQuery sq = null )
        {
            if( col != null && col._parent != t ) throw new Exception( "参数2: 字段 不属于 参数1: 表" );
            var q = new MyQuery( "select count(" + ( distinct ? " distinct" : "" ) + @" " + ( col == null ? "*" : col._name ) + @" ) from ", t );
            if( sq != null ) q.Append( sq );
            return q;
        }

        /// <summary>
        /// 生成 select min( colName ) from xxxx where .....
        /// </summary>
        public static MyQuery Min( DbColumn col, MyQuery sq = null )
        {
            var q = new MyQuery( "select min( ", col._name, " ) from ", col._parent );
            if( sq != null ) q.Append( sq );
            return q;
        }
        /// <summary>
        /// 生成 select max( colName ) from xxxx where .....
        /// </summary>
        public static MyQuery Max( DbColumn col, MyQuery sq = null )
        {
            var q = new MyQuery( "select max( ", col._name, " ) from ", col._parent );
            if( sq != null ) q.Append( sq );
            return q;
        }
        /// <summary>
        /// 生成 select avg( colName ) from xxxx where .....
        /// </summary>
        public static MyQuery Avg( DbColumn col, MyQuery sq = null )
        {
            var q = new MyQuery( "select avg( ", col._name, " ) from ", col._parent );
            if( sq != null ) q.Append( sq );
            return q;
        }
        /// <summary>
        /// 生成 ( select 1 from xxxx limit 1 )
        /// </summary>
        public static MyQuery Exists( DbTable t, MyQuery sq = null )
        {
            var q = new MyQuery( "select 1 from ", t );
            if( sq != null ) q.Append( sq );
            return q;
        }

    }
}