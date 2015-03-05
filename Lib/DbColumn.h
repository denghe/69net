#ifndef __DBCOLUMN_H__
#define __DBCOLUMN_H__

namespace xxx
{
    struct DbTable;
    struct DbRow;

    struct DbColumn
    {
        DbTable*            parent = nullptr;  // ”… DbTable ÃÓ≥‰
        int                 columnIndex = 0;        // ”… DbTable ÃÓ≥‰
        String              name = "";
        bool                nullable = false;
        DbDataTypes         dataType = DbDataTypes::Unknown;
        bool                autoIncrement = false;
        bool                primaryKey = false;
        bool                timestamp = false;
        String              comment = "";
        inline bool isReadonly()
        {
            return autoIncrement | timestamp;
        }

        DbColumn()
        {
        }
        DbColumn( DbColumn const& other )
        {
            operator=( other );
        }
        DbColumn( DbColumn&& other )
        {
            operator=( (DbColumn&&)other );
        }
        DbColumn& operator=( DbColumn const& other )
        {
            parent = other.parent;
            columnIndex = other.columnIndex;
            name = other.name;
            nullable = other.nullable;
            dataType = other.dataType;
            autoIncrement = other.autoIncrement;
            primaryKey = other.primaryKey;
            timestamp = other.timestamp;
            comment = other.comment;
            return *this;
        }
        DbColumn& operator=( DbColumn&& other )
        {
            parent = other.parent;
            columnIndex = other.columnIndex;
            name = std::move( other.name );
            nullable = other.nullable;
            dataType = other.dataType;
            autoIncrement = other.autoIncrement;
            primaryKey = other.primaryKey;
            timestamp = other.timestamp;
            comment = std::move( other.comment );
            return *this;
        }
    };
}

#endif
