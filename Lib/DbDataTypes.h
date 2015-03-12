#ifndef __DBDATATYPES_H__
#define __DBDATATYPES_H__

namespace xxx
{
    enum class DbDataTypes
    {
        Boolean,
        Int8,
        Int16,
        Int32,
        Int64,
        UInt8,
        UInt16,
        UInt32,
        UInt64,
        Float,
        Double,
        DateTime,
        String,
        Bytes,
        Unknown                 // for default value/ max value
    };
    const struct DbNull_t { DbNull_t(){} } DbNull;

}

#endif
