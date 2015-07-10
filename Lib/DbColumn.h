#pragma once

namespace xxx
{
    struct DbTable;
    struct DbRow;

    struct DbColumn
    {
        String              name = "";
        DbDataTypes         dataType = DbDataTypes::Unknown;
        bool                nullable = false;
        bool                autoIncrement = false;
        bool                primaryKey = false;
        bool                timestamp = false;
        String              comment = "";
        DbTable*            parent = nullptr;       // 由 DbTable 填充
        int                 columnIndex = 0;        // 由 DbTable 填充
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
