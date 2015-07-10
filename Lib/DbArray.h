#pragma once

namespace xxx
{
    
    
#ifdef __DEBUG
    template<typename T>
    static bool IsType( DbDataTypes t )
    {
        return false;
    }
    template<> inline bool IsType<bool>( DbDataTypes t ) { return t == DbDataTypes::Boolean; }
    template<> inline bool IsType<int8>( DbDataTypes t ) { return t == DbDataTypes::Int8; }
    template<> inline bool IsType<int16>( DbDataTypes t ) { return t == DbDataTypes::Int16; }
    template<> inline bool IsType<int32>( DbDataTypes t ) { return t == DbDataTypes::Int32; }
    template<> inline bool IsType<int64>( DbDataTypes t ) { return t == DbDataTypes::Int64; }
    template<> inline bool IsType<uint8>( DbDataTypes t ) { return t == DbDataTypes::UInt8; }
    template<> inline bool IsType<uint16>( DbDataTypes t ) { return t == DbDataTypes::UInt16; }
    template<> inline bool IsType<uint32>( DbDataTypes t ) { return t == DbDataTypes::UInt32; }
    template<> inline bool IsType<uint64>( DbDataTypes t ) { return t == DbDataTypes::UInt64; }
    template<> inline bool IsType<float>( DbDataTypes t ) { return t == DbDataTypes::Float; }
    template<> inline bool IsType<double>( DbDataTypes t ) { return t == DbDataTypes::Double; }
    template<> inline bool IsType<DateTime>( DbDataTypes t ) { return t == DbDataTypes::DateTime; }
    template<> inline bool IsType<String>( DbDataTypes t ) { return t == DbDataTypes::String; }
    template<> inline bool IsType<ByteBuffer>( DbDataTypes t ) { return t == DbDataTypes::Bytes; }
#endif
    
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



        template<typename T>
        void SetValue( int idx, T v )
        {
#ifdef __DEBUG
            if( !IsType<T>( dataType ) )
            {
                throw std::exception();// "wrong data type!!" );
            }
#endif
            ( ( List<T>* )list )->Set( idx, v );
        }


        inline void SetValue( int idx, String&& v )
        {
#ifdef __DEBUG
            if( !IsType<String>( dataType ) )
            {
                throw std::exception();// "wrong data type!!" );
            }
#endif
            ( ( List<String>* )list )->Set( idx, std::move( v ) );
        }

        inline void SetValue( int idx, bool v )
        {
#ifdef __DEBUG
            if( !IsType<bool>( dataType ) )
            {
                throw std::exception();// "wrong data type!!" );
            }
#endif
            ( ( List<bool>* )list )->Set( idx, v );
        }


        template<typename T>
        T GetValue( int idx ) const
        {
#ifdef __DEBUG
            if( !IsType<T>( dataType ) )
            {
                throw std::exception();// "wrong data type!!" );
            }
#endif
            return ( ( List<T>* )list )->IndexAt( idx );
        }


        // 注：bool 类型不行
        template<typename T>
        T& RefValue( int idx )
        {
#ifdef __DEBUG
            if( !IsType<T>( dataType ) )
            {
                throw std::exception();// "wrong data type!!" );
            }
#endif
            return ( ( List<T>* )list )->IndexAt( idx );
        }

    };

}
