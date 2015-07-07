#include "All.h"

namespace xxx
{
    DbValue DbRow::operator[]( int colIdx )
    {
        return DbValue{ this, colIdx };
    }
    DbValue DbRow::operator[]( String const& colName )
    {
        return DbValue{ this, parent->GetIndexByColumnName( colName ) };
    }
    DbValue DbRow::operator[]( DbColumn const& col )
    {
        return DbValue{ this, col.columnIndex };
    }
}

