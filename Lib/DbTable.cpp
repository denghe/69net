#include "All.h"

namespace xxx
{
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
            if( columns[ i ].name == name ) return i;       // todo: hash ÓÅ»¯£¿
        }
        return -1;
    }

}
