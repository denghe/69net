#include "All.h"

namespace xxx
{
    DbArray& DbArray::operator=( DbArray const& other )
    {
        Destroy();
        dataType = other.dataType;
        switch( dataType )
        {
        case DbDataTypes::Boolean:  list = new List<bool>( *( List<bool>* )other.list ); break;
        case DbDataTypes::Int8:     list = new List<int8>( *( List<int8>* )other.list ); break;
        case DbDataTypes::Int16:    list = new List<int16>( *( List<int16>* )other.list ); break;
        case DbDataTypes::Int32:    list = new List<int32>( *( List<int32>* )other.list ); break;
        case DbDataTypes::Int64:    list = new List<int64>( *( List<int64>* )other.list ); break;
        case DbDataTypes::UInt8:    list = new List<uint8>( *( List<uint8>* )other.list ); break;
        case DbDataTypes::UInt16:   list = new List<uint16>( *( List<uint16>* )other.list ); break;
        case DbDataTypes::UInt32:   list = new List<uint32>( *( List<uint32>* )other.list ); break;
        case DbDataTypes::UInt64:   list = new List<uint64>( *( List<uint64>* )other.list ); break;
        case DbDataTypes::Float:    list = new List<float>( *( List<float>* )other.list ); break;
        case DbDataTypes::Double:   list = new List<double>( *( List<double>* )other.list ); break;
        case DbDataTypes::DateTime: list = new List<DateTime>( *( List<DateTime>* )other.list ); break;
        case DbDataTypes::String:   list = new List<String>( *( List<String>* )other.list ); break;
        case DbDataTypes::Bytes:    list = new List<ByteBuffer>( *( List<ByteBuffer>* )other.list ); break;
        }
        return *this;
    }

    DbArray& DbArray::operator=( DbArray&& other )
    {
        Destroy();
        dataType = other.dataType;
        list = other.list;
        other.list = nullptr;
        return *this;
    }

    DbArray::DbArray( DbArray const& other ) : list( nullptr )
    {
        operator=( other );
    }


    void DbArray::Grow()
    {
        switch( dataType )
        {
        case DbDataTypes::Boolean:  ( ( List<bool>* )list )->Push( false ); break;
        case DbDataTypes::Int8:     ( ( List<int8>* )list )->Push( 0 ); break;
        case DbDataTypes::Int16:    ( ( List<int16>* )list )->Push( 0 ); break;
        case DbDataTypes::Int32:    ( ( List<int32>* )list )->Push( 0 ); break;
        case DbDataTypes::Int64:    ( ( List<int64>* )list )->Push( 0 ); break;
        case DbDataTypes::UInt8:    ( ( List<uint8>* )list )->Push( 0 ); break;
        case DbDataTypes::UInt16:   ( ( List<uint16>* )list )->Push( 0 ); break;
        case DbDataTypes::UInt32:   ( ( List<uint32>* )list )->Push( 0 ); break;
        case DbDataTypes::UInt64:   ( ( List<uint64>* )list )->Push( 0 ); break;
        case DbDataTypes::Float:    ( ( List<float>* )list )->Push( 0.0f ); break;
        case DbDataTypes::Double:   ( ( List<double>* )list )->Push( 0 ); break;
        case DbDataTypes::DateTime: ( ( List<DateTime>* )list )->Emplace(); break;
        case DbDataTypes::String:   ( ( List<String>* )list )->Emplace(); break;
        case DbDataTypes::Bytes:    ( ( List<ByteBuffer>* )list )->Emplace(); break;
        }
    }


    void DbArray::Destroy()
    {
        if( !list ) return;
        switch( dataType )
        {
        case DbDataTypes::Boolean:  delete ( List<bool>* )list; break;
        case DbDataTypes::Int8:     delete ( List<int8>* )list; break;
        case DbDataTypes::Int16:    delete ( List<int16>* )list; break;
        case DbDataTypes::Int32:    delete ( List<int32>* )list; break;
        case DbDataTypes::Int64:    delete ( List<int64>* )list; break;
        case DbDataTypes::UInt8:    delete ( List<uint8>* )list; break;
        case DbDataTypes::UInt16:   delete ( List<uint16>* )list; break;
        case DbDataTypes::UInt32:   delete ( List<uint32>* )list; break;
        case DbDataTypes::UInt64:   delete ( List<uint64>* )list; break;
        case DbDataTypes::Float:    delete ( List<float>* )list; break;
        case DbDataTypes::Double:   delete ( List<double>* )list; break;
        case DbDataTypes::DateTime: delete ( List<DateTime>* )list; break;
        case DbDataTypes::String:   delete ( List<String>* )list; break;
        case DbDataTypes::Bytes:    delete ( List<ByteBuffer>* )list; break;
        }
        list = nullptr;
    }


    DbArray::~DbArray()
    {
        Destroy();
    }

    DbArray::DbArray( DbDataTypes t ) : dataType( t )
    {
        switch( t )
        {
        case DbDataTypes::Boolean:  list = new List<bool>(); break;
        case DbDataTypes::Int8:     list = new List<int8>(); break;
        case DbDataTypes::Int16:    list = new List<int16>(); break;
        case DbDataTypes::Int32:    list = new List<int32>(); break;
        case DbDataTypes::Int64:    list = new List<int64>(); break;
        case DbDataTypes::UInt8:    list = new List<uint8>(); break;
        case DbDataTypes::UInt16:   list = new List<uint16>(); break;
        case DbDataTypes::UInt32:   list = new List<uint32>(); break;
        case DbDataTypes::UInt64:   list = new List<uint64>(); break;
        case DbDataTypes::Float:    list = new List<float>(); break;
        case DbDataTypes::Double:   list = new List<double>(); break;
        case DbDataTypes::DateTime: list = new List<DateTime>(); break;
        case DbDataTypes::String:   list = new List<String>(); break;
        case DbDataTypes::Bytes:    list = new List<ByteBuffer>(); break;
        default: throw std::exception( "unknown data type" );
        }
    }

    DbArray::DbArray( DbArray&& other ) : list( nullptr )
    {
        operator=( (DbArray&&)other );
    }

    void DbArray::Push( bool v )
    {
        ( ( List<bool>* )list )->Push( v );
    }

    void DbArray::Push( const char* v )
    {
        ( ( List<String>* )list )->Emplace( v );
    }

    void DbArray::Push( ByteBuffer&& v )
    {
        ( ( List<ByteBuffer>* )list )->Push( (ByteBuffer&&)v );
    }

    void DbArray::Push( ByteBuffer const& v )
    {
        ( ( List<ByteBuffer>* )list )->Push( v );
    }

    void DbArray::Push( String&& v )
    {
        ( ( List<String>* )list )->Push( (String&&)v );
    }

    void DbArray::Push( String const& v )
    {
        ( ( List<String>* )list )->Push( v );
    }

    void DbArray::Push( DateTime v )
    {
        ( ( List<DateTime>* )list )->Push( v );
    }

    void DbArray::Push( double v )
    {
        ( ( List<double>* )list )->Push( v );
    }

    void DbArray::Push( float v )
    {
        ( ( List<float>* )list )->Push( v );
    }

    void DbArray::Push( uint64 v )
    {
        ( ( List<uint64>* )list )->Push( v );
    }

    void DbArray::Push( uint32 v )
    {
        ( ( List<uint32>* )list )->Push( v );
    }

    void DbArray::Push( uint16 v )
    {
        ( ( List<uint16>* )list )->Push( v );
    }

    void DbArray::Push( uint8 v )
    {
        ( ( List<uint8>* )list )->Push( v );
    }

    void DbArray::Push( int64 v )
    {
        ( ( List<int64>* )list )->Push( v );
    }

    void DbArray::Push( int32 v )
    {
        ( ( List<int32>* )list )->Push( v );
    }

    void DbArray::Push( int16 v )
    {
        ( ( List<int16>* )list )->Push( v );
    }

    void DbArray::Push( int8 v )
    {
        ( ( List<int8>* )list )->Push( v );
    }

}
