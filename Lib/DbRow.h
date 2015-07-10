#pragma once

namespace xxx
{
    struct DbRow
    {
        DbTable*         parent;
        int              rowIndex;

        DbValue operator[]( int colIdx );
        DbValue operator[]( DbColumn const& col );
        DbValue operator[]( String const& colName );
    };

}
