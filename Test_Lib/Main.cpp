#include "Main.h"
#include <ctime>
#include <iomanip>
#include <sstream>

// todo:

// ʵ�� DbContainer ϵ��, �� DateTime( ʹ�� system_clock::time_point )

// ���� Sqlite �����
// ������ Sqlite ��ĳ��࣬�ṩ Mysql ��֧��
// ��� Mysql ���ݿ������ĳ���

// ������ for ByteBuffer ?
// Nullable �����л�֧��?
// Any ���͵�����? ( ������ c++ ������������ �Լ����ó����ļ���ɶ�� )
// �򻯸��� Size()
// ָ���Ƿ�Ϊ�յ� byte �Ĺ������ã�����ɴ����л����ͣ�
// ���Ϳ���ָ���� �Ƿ�������л�������˵  index : value �����л��� ���� bit ���
// �������л���ָ��Ա������ֵ��Ĭ��ֵ���ȵ���Щ��
// ���������£��ɶԱ����ַ�ʽ�����л��󳤶Ȳѡ����ʡ�ķ�ʽ



namespace DBContainer
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

    bool DbArray::GetBoolValue( int idx ) const
    {
#ifdef __DEBUG
        if( !IsType<bool>( dataType ) )
        {
            throw std::exception( "wrong data type!!" );
        }
#endif
        return ( ( List<bool>* )list )->At( idx );
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

    bool DbValue::Nullable()
    {
        return parent->parent->columns[ columnIndex ].nullable;
    }

    List<bool>& DbValue::GetNullflag()
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

    bool DbValue::IsNull()
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

    void DbValue::ToString( String& output )
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
            // todo: �Ż�
            auto t = std::chrono::system_clock::to_time_t( GetValue<DateTime>() );
            std::ostringstream ss;
            ss << std::put_time( std::localtime( &t ), "%Y-%m-%d %X" );
            output.Append( ss.str() );
        } break;
        case DbDataTypes::Bytes:    output.Append( "ByteBuffer ToString todo" ); break; // GetValue<ByteBuffer>() ); break;
        default:
            throw std::exception( "unknown data type" );
        }
    }



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
            if( columns[ i ].name == name ) return i;       // todo: hash �Ż���
        }
        return -1;
    }

}

using namespace DBContainer;
int main()
{
    DbTable dt;
    DbColumn c;
    c.dataType = DbDataTypes::Boolean;
    dt.AddColumn( c );
    c.dataType = DbDataTypes::Int32;
    dt.AddColumn( c );
    c.dataType = DbDataTypes::String;
    dt.AddColumn( c );
    c.dataType = DbDataTypes::DateTime;
    c.nullable = true;
    dt.AddColumn( c );

    dt.AddRow( true, 1, "zzz", std::chrono::system_clock::now() );
    dt.AddRow( false, 234, "qwer", DbNull );

    String s;
    dt.ToString( s );
    CoutLine( s );
    //dt.
    //auto row = dt.AddRow();
    //row[ c0 ].SetValue<bool>( true );
    //row[ c1 ].SetValue<int>( 123 );
    //row[ c2 ].SetValue<String>( "asdf" );


    return 0;
}
