#ifndef __DBTABLE_H__
#define __DBTABLE_H__

namespace xxx
{
    struct DbTable
    {
        String           name = "";
        String           comment = "";
        List<DbArray>    columnDatas;
        List<DbColumn>   columns;
        List<List<bool>> nullflags;
        int              rowCount = 0;

        template<typename T>
        DbColumn& AddColumn( T&& c )
        {
            columns.Push( std::forward<T>( c ) );
            auto& col = columns.Top();
            nullflags.Emplace();
            col.parent = this;
            col.columnIndex = columns.Size();
            columnDatas.Emplace( col.dataType );
            return col;
        }
        DbRow AddRow();

        void ToString( String& output ) const
        {
            for( int j = 0; j < columns.Size(); ++j )
            {
                if( j ) output.Append( "\t" );
                if( columns[ j ].name.Size() )
                {
                    output.Append( columns[ j ].name );
                }
                else
                {
                    output.Append( "col_", j );
                }
            }
            output.Append( '\n' );
            for( int i = 0; i < rowCount; ++i )
            {
                if( i ) output.Append( '\n' );
                auto row = operator[]( i );
                for( int j = 0; j < columns.Size(); ++j )
                {
                    if( j ) output.Append( ",\t" );
                    row[ j ].ToString( output );
                }
            }
        }

    private:
        template<typename T>
        void AddRowCore( int& i, T&& v )
        {
            columnDatas[ i ].Push( std::forward<T>( v ) );
            if( columns[ i ].nullable )
            {
                nullflags[ i ].Push( false );
            }
        }
        void AddRowCore( int& i, DbNull_t v )
        {
#if __DEBUG
            if( !columns[ i ].nullable )
            {
                throw std::exception( "column is not nullable" );
            }
#endif
            columnDatas[ i ].Grow();
            nullflags[ i ].Push( true );
        }
        template<typename T, typename ...TS>
        void AddRowCore( int& i, T&& v, TS&& ...vs )
        {
            AddRowCore( i, std::forward<T>( v ) );
            ++i;
            AddRowCore( i, std::forward<TS>( vs )... );
        }
    public:
        template<typename ...TS>
        DbRow AddRow( TS&& ...vs )
        {
#if __DEBUG
            if( sizeof...( vs ) != columns.Size() )
            {
                throw std::exception( "the parameters count is not match the columns count" );
            }
#endif
            int i = 0;
            AddRowCore( i, std::forward<TS>( vs )... );
            return DbRow{ this, rowCount++ };
        }

        DbRow operator[]( int rowIdx );
        DbRow operator[]( int rowIdx ) const;

        int GetIndexByColumnName( String const& name );

    };
}

#endif
