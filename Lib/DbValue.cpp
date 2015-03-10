#include "All.h"

namespace xxx
{
    bool DbValue::Nullable() const
    {
        return parent->parent->columns[ columnIndex ].nullable;
    }

    List<bool>& DbValue::GetNullflag()
    {
        return parent->parent->nullflags[ columnIndex ];
    }

    List<bool> const& DbValue::GetNullflag() const
    {
        return parent->parent->nullflags[ columnIndex ];
    }

    DbArray& DbValue::GetColumnData()
    {
        return parent->parent->columnDatas[ columnIndex ];
    }

    DbArray const& DbValue::GetColumnData() const
    {
        return parent->parent->columnDatas[ columnIndex ];
    }

    DbColumn& DbValue::GetColumn()
    {
        return parent->parent->columns[ columnIndex ];
    }

    DbColumn const& DbValue::GetColumn() const
    {
        return parent->parent->columns[ columnIndex ];
    }

    bool DbValue::IsNull() const
    {
#if __DEBUG
        if( !Nullable() ) return false;
#endif
        return GetNullflag()[ parent->rowIndex ];
    }

    void DbValue::SetNull()
    {
#if __DEBUG
        if( !Nullable() ) return;
#endif
        GetNullflag().SetTrue( parent->rowIndex );
    }

    bool DbValue::GetBoolValue() const
    {
        return GetColumnData().GetBoolValue( parent->rowIndex );
    }

    void DbValue::Dump( String& output ) const
    {
        if( IsNull() )
        {
            output.Append( "[null]" );
            return;
        }
        auto dt = GetColumn().dataType;
        switch( dt )
        {
        case DbDataTypes::Boolean:  output.Append( GetBoolValue() ); break;
        case DbDataTypes::Int8:     output.Append( GetValue<int8>() ); break;
        case DbDataTypes::Int16:    output.Append( GetValue<int16>() ); break;
        case DbDataTypes::Int32:    output.Append( GetValue<int32>() ); break;
        case DbDataTypes::Int64:    output.Append( GetValue<int64>() ); break;
        case DbDataTypes::UInt8:    output.Append( GetValue<uint8>() ); break;
        case DbDataTypes::UInt16:   output.Append( GetValue<uint16>() ); break;
        case DbDataTypes::UInt32:   output.Append( GetValue<uint32>() ); break;
        case DbDataTypes::UInt64:   output.Append( GetValue<uint64>() ); break;
        case DbDataTypes::Float:    output.Append( GetValue<float>() ); break;
        case DbDataTypes::Double:   output.Append( GetValue<double>() ); break;
        case DbDataTypes::String:   output.Append( GetValue<String>() ); break;
        case DbDataTypes::DateTime:
        {
            // todo: ”≈ªØ
            std::ostringstream ss;
            auto t = std::chrono::system_clock::to_time_t( GetValue<DateTime>() );
#if __WIN
            tm tim;
            localtime_s( &tim, &t );
            ss << std::put_time( &tim, "%Y-%m-%d %X" );
#else
            ss << std::put_time( std::localtime( &t ), "%Y-%m-%d %X" );
#endif
            output.Append( ss.str() );
        } break;
        case DbDataTypes::Bytes:    output.Append( "ByteBuffer ToString todo" ); break; // GetValue<ByteBuffer>() ); break;
        default:
            throw std::exception( "unknown data type" );
        }
    }

}

