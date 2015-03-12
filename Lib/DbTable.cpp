#include "All.h"

namespace xxx
{

    DbColumn& DbTable::AddColumn( DbDataTypes dataType )
    {
        auto& col = columns.Emplace();
        col.dataType = dataType;
        nullflags.Emplace();
        col.parent = this;
        col.columnIndex = columns.Size() - 1;
        columnDatas.Emplace( col.dataType );
        return col;
    }

    DbColumn& DbTable::AddColumn( String const& name, DbDataTypes dataType )
    {
        auto& col = AddColumn( dataType );
        col.name = name;
        return col;
    }

    void DbTable::AddRowCore( int& i, DbNull_t v )
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

    DbRow DbTable::AddRow()
    {
        for( int i = 0; i < columns.Size(); ++i )
        {
            columnDatas[ i ].Grow();
            if( columns[ i ].nullable )
            {
                nullflags[ i ].Push( true );
            }
        }
        return DbRow{ this, rowCount++ };
    }


    void DbTable::Dump( String& output ) const
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
                row[ j ].Dump( output );
            }
        }
    }


    DbRow DbTable::operator[]( int rowIdx )
    {
#if __DEBUG
        if( rowIdx > rowCount )
        {
            throw std::exception( "out of range: rowIdx" );
        }
#endif
        return DbRow{ this, rowIdx };
    }
    DbRow DbTable::operator[]( int rowIdx ) const
    {
#if __DEBUG
        if( rowIdx > rowCount )
        {
            throw std::exception( "out of range: rowIdx" );
        }
#endif
        return DbRow{ const_cast<DbTable*>( this ), rowIdx };
    }

    int DbTable::GetIndexByColumnName( String const& name )
    {
        for( int i = 0; i < columns.Size(); ++i )
        {
            if( columns[ i ].name == name ) return i;       // todo: hash 优化？
        }
        return -1;
    }

}
