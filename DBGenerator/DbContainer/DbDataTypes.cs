using System;
using System.Collections.Generic;
using System.Data;

namespace DBContainer
{
    public enum DbDataTypes
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
}
