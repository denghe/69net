#include "Lib/All.h"
using namespace xxx;

namespace DBContainer
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

    // 先写在这
    typedef std::chrono::system_clock::time_point DateTime;



    struct DbArray
    {
        DbDataTypes     dataType;
        void*           list;

        explicit DbArray( DbDataTypes t );
        void Destroy();
        ~DbArray();

        DbArray( DbArray const& other );
        DbArray( DbArray&& other );

        DbArray& operator=( DbArray const& other );
        DbArray& operator=( DbArray&& other );

        void Grow();

        void Push( bool v ) { ( ( List<bool>* )list )->Push( v ); }
        void Push( int8 v ) { ( ( List<int8>* )list )->Push( v ); }
        void Push( int16 v ) { ( ( List<int16>* )list )->Push( v ); }
        void Push( int32 v ) { ( ( List<int32>* )list )->Push( v ); }
        void Push( int64 v ) { ( ( List<int64>* )list )->Push( v ); }
        void Push( uint8 v ) { ( ( List<uint8>* )list )->Push( v ); }
        void Push( uint16 v ) { ( ( List<uint16>* )list )->Push( v ); }
        void Push( uint32 v ) { ( ( List<uint32>* )list )->Push( v ); }
        void Push( uint64 v ) { ( ( List<uint64>* )list )->Push( v ); }
        void Push( float v ) { ( ( List<float>* )list )->Push( v ); }
        void Push( double v ) { ( ( List<double>* )list )->Push( v ); }
        void Push( DateTime v ) { ( ( List<DateTime>* )list )->Push( v ); }
        void Push( String const& v ) { ( ( List<String>* )list )->Push( v ); }
        void Push( String&& v ) { ( ( List<String>* )list )->Push( (String&&)v ); }
        void Push( ByteBuffer const& v ) { ( ( List<ByteBuffer>* )list )->Push( v ); }
        void Push( ByteBuffer&& v ) { ( ( List<ByteBuffer>* )list )->Push( ( ByteBuffer&&)v ); }
        void Push( const char* v ) { ( ( List<String>* )list )->Emplace( v ); }

        // todo: Reserve

#ifdef __DEBUG
        template<typename T>
        static bool IsType( DbDataTypes t )
        {
            return false;
        }
        template<> static bool IsType<bool>( DbDataTypes t ) { return t == DbDataTypes::Boolean; }
        template<> static bool IsType<int8>( DbDataTypes t ) { return t == DbDataTypes::Int8; }
        template<> static bool IsType<int16>( DbDataTypes t ) { return t == DbDataTypes::Int16; }
        template<> static bool IsType<int32>( DbDataTypes t ) { return t == DbDataTypes::Int32; }
        template<> static bool IsType<int64>( DbDataTypes t ) { return t == DbDataTypes::Int64; }
        template<> static bool IsType<uint8>( DbDataTypes t ) { return t == DbDataTypes::UInt8; }
        template<> static bool IsType<uint16>( DbDataTypes t ) { return t == DbDataTypes::UInt16; }
        template<> static bool IsType<uint32>( DbDataTypes t ) { return t == DbDataTypes::UInt32; }
        template<> static bool IsType<uint64>( DbDataTypes t ) { return t == DbDataTypes::UInt64; }
        template<> static bool IsType<float>( DbDataTypes t ) { return t == DbDataTypes::Float; }
        template<> static bool IsType<double>( DbDataTypes t ) { return t == DbDataTypes::Double; }
        template<> static bool IsType<DateTime>( DbDataTypes t ) { return t == DbDataTypes::DateTime; }
        template<> static bool IsType<String>( DbDataTypes t ) { return t == DbDataTypes::String; }
        template<> static bool IsType<ByteBuffer>( DbDataTypes t ) { return t == DbDataTypes::Bytes; }
#endif


        template<typename T>
        void SetValue( int idx, T&& v )
        {
#ifdef __DEBUG
            if( !IsType<T>( dataType ) )
            {
                throw std::exception( "wrong data type!!" );
            }
#endif
            ( ( List<T>* )list )->Set( idx, std::forward<T>( v ) );
        }

        // 注：bool 类型不行
        template<typename T>
        T const& GetValue( int idx ) const
        {
#ifdef __DEBUG
            if( !IsType<T>( dataType ) )
            {
                throw std::exception( "wrong data type!!" );
            }
#endif
            return ( ( List<T>* )list )->At( idx );
        }

        bool GetBoolValue( int idx ) const;

        // 注：bool 类型不行
        template<typename T>
        T& RefValue( int idx )
        {
#ifdef __DEBUG
            if( !IsType<T>( dataType ) )
            {
                throw std::exception( "wrong data type!!" );
                return false;
            }
#endif
            return ( ( List<T>* )list )->At( idx );
        }

    };


    struct DbTable;
    struct DbRow;

    struct DbColumn
    {
        DbTable*            parent          = nullptr;  // 由 DbTable 填充
        int                 columnIndex     = 0;        // 由 DbTable 填充
        String              name            = "";
        bool                nullable        = false;
        DbDataTypes         dataType        = DbDataTypes::Unknown;
        bool                autoIncrement   = false;
        bool                primaryKey      = false;
        bool                timestamp       = false;
        String              comment         = "";
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
            parent          = other.parent;
            columnIndex     = other.columnIndex;
            name            = other.name;
            nullable        = other.nullable;
            dataType        = other.dataType;
            autoIncrement   = other.autoIncrement;
            primaryKey      = other.primaryKey;
            timestamp       = other.timestamp;
            comment         = other.comment;
            return *this;
        }
        DbColumn& operator=( DbColumn&& other )
        {
            parent          = other.parent;
            columnIndex     = other.columnIndex;
            name            = std::move( other.name );
            nullable        = other.nullable;
            dataType        = other.dataType;
            autoIncrement   = other.autoIncrement;
            primaryKey      = other.primaryKey;
            timestamp       = other.timestamp;
            comment         = std::move( other.comment );
            return *this;
        }
    };



    struct DbValue
    {
        DbRow*           parent;
        int              columnIndex;

    private:
        bool Nullable();
        List<bool>& GetNullflag();
        DbArray& GetColumnData();
        DbArray const& GetColumnData() const;
        DbColumn& GetColumn();

    public:
        bool IsNull();
        void SetNull();

        template<typename T>
        void Assign( T v )
        {
            GetColumnData()[ parent->rowIndex ] = v;
#if __DEBUG
            if( Nullable() )
            {
#endif
                GetNullflag().SetFalse( parent->rowIndex );
#if __DEBUG
            }
#endif
        }

        template<typename T>
        T const& GetValue() const
        {
            return GetColumnData().GetValue<T>( parent->rowIndex );
        }

        bool GetBoolValue() const;

        template<typename T>
        T&& RefValue()
        {
            return GetColumnData().RefValue<T>( parent->rowIndex );
        }

        template<typename T>
        void SetValue( T&& v )
        {
            return GetColumnData().SetValue<T>( parent->rowIndex, std::forward<T>( v ) );
        }

        inline explicit operator uint8() { return GetValue<uint8>(); }
        inline explicit operator uint16() { return GetValue<uint16>(); }
        inline explicit operator uint32() { return GetValue<uint32>(); }
        inline explicit operator uint64() { return GetValue<uint64>(); }
        inline explicit operator int8() { return GetValue<int8>(); }
        inline explicit operator int16() { return GetValue<int16>(); }
        inline explicit operator int32() { return GetValue<int32>(); }
        inline explicit operator int64() { return GetValue<int64>(); }
        inline explicit operator float() { return GetValue<float>(); }
        inline explicit operator double() { return GetValue<double>(); }
        inline explicit operator bool() { return GetBoolValue(); }
        inline explicit operator DateTime() { return GetValue<DateTime>(); }
        inline explicit operator ByteBuffer() { return GetValue<ByteBuffer>(); }
        inline explicit operator String() { return GetValue<String>(); }


        void ToString( String& output );
    };



    struct DbRow
    {
        DbTable*         parent;
        int              rowIndex;

        DbValue operator[]( int colIdx );
        DbValue operator[]( DbColumn const& col );
        DbValue operator[]( String const& colName );
    };

    const struct DbNull_t {} DbNull;

    struct DbTable
    {
        String           name = "";
        String           comment = "";
        List<DbArray>    columnDatas;
        List<DbColumn>   columns;
        List<List<bool>> nullflags;
        int              rowCount = 0;

        template<typename T>
        DbColumn& AddColumn( T&& c )
        {
            columns.Push( std::forward<T>( c ) );
            auto& col = columns.Top();
            nullflags.Emplace();
            col.parent = this;
            col.columnIndex = columns.Size();
            columnDatas.Emplace( col.dataType );
            return col;
        }
        DbRow AddRow();

        void ToString( String& output ) const
        {
            for( int j = 0; j < columns.Size(); ++j )
            {
                if( j ) output.Append( "\t" );
                if( columns[ j ].name.Size() )
                {
                    output.Append( columns[ j ].name );
                }
                else
                {
                    output.Append( "col_", j );
                }
            }
            output.Append( '\n' );
            for( int i = 0; i < rowCount; ++i )
            {
                if( i ) output.Append( '\n' );
                auto row = operator[]( i );
                for( int j = 0; j < columns.Size(); ++j )
                {
                    if( j ) output.Append( ",\t" );
                    row[ j ].ToString( output );
                }
            }
        }

    private:
        template<typename T>
        void AddRowCore( int& i, T&& v )
        {
            columnDatas[ i ].Push( std::forward<T>( v ) );
            if( columns[ i ].nullable )
            {
                nullflags[ i ].Push( false );
            }
        }
        void AddRowCore( int& i, DbNull_t v )
        {
#if __DEBUG
            if( !columns[ i ].nullable )
            {
                throw std::exception( "column is not nullable" );
            }
#endif
            columnDatas[ i ].Grow();
            nullflags[ i ].Push( true );
        }
        template<typename T, typename ...TS>
        void AddRowCore( int& i, T&& v, TS&& ...vs )
        {
            AddRowCore( i, std::forward<T>( v ) );
            ++i;
            AddRowCore( i, std::forward<TS>( vs )... );
        }
    public:
        template<typename ...TS>
        DbRow AddRow( TS&& ...vs )
        {
#if __DEBUG
            if( sizeof...( vs ) != columns.Size() )
            {
                throw std::exception( "the parameters count is not match the columns count" );
            }
#endif
            int i = 0;
            AddRowCore( i, std::forward<TS>( vs )... );
            return DbRow{ this, rowCount++ };
        }

        DbRow operator[]( int rowIdx );
        DbRow operator[]( int rowIdx ) const;

        int GetIndexByColumnName( String const& name );

    };
}
