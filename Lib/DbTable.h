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

        DbColumn& AddColumn( DbDataTypes dataType );
        DbColumn& AddColumn( String const& name, DbDataTypes dataType );

        template<typename T>
        DbColumn& AddColumn( T&& c )
        {
            columns.Push( std::forward<T>( c ) );
            auto& col = columns.Top();
            nullflags.Emplace();
            col.parent = this;
            col.columnIndex = columns.Size() - 1;
            columnDatas.Emplace( col.dataType );
            return col;
        }

        template<typename T>
        void AddColumnsCore( T&& v )
        {
            AddColumn( std::forward<T>( v ) );
        }
        template<typename T, typename ...TS>
        void AddColumnsCore( T&& v, TS&& ...vs )
        {
            AddColumnsCore( std::forward<T>( v ) );
            AddColumnsCore( std::forward<TS>( vs )... );
        }
        template<typename ...TS>
        List<DbColumn>& AddColumns( TS&& ...vs )
        {
            AddColumnsCore( std::forward<TS>( vs )... );
            return columns;
        }


        DbRow AddRow();

        void Dump( String& output ) const;

    private:
        void AddRowCore( int& i, DbNull_t v );

        template<typename T>
        void AddRowCore( int& i, T&& v )
        {
            columnDatas[ i ].Push( std::forward<T>( v ) );
            if( columns[ i ].nullable )
            {
                nullflags[ i ].Push( false );
            }
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
