#ifndef __DBROW_H__
#define __DBROW_H__

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

#endif
