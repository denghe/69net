#ifndef __DBARRAY_H__
#define __DBARRAY_H__

namespace xxx
{
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

        void Push( bool v );
        void Push( int8 v );
        void Push( int16 v );
        void Push( int32 v );
        void Push( int64 v );
        void Push( uint8 v );
        void Push( uint16 v );
        void Push( uint32 v );
        void Push( uint64 v );
        void Push( float v );
        void Push( double v );
        void Push( DateTime v );
        void Push( String const& v );
        void Push( String&& v );
        void Push( ByteBuffer const& v );
        void Push( ByteBuffer&& v );
        void Push( const char* v );

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

}

#endif
