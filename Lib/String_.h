#ifndef _STRING_H__
#define _STRING_H__

namespace xxx
{

    class FlatBuffer;
    class ByteBuffer;

    class String : Memmoveable
    {
    public:
        explicit String( int capacity );                            // prepare
        String();
        String( Pool& p );                                          // prepare with pool buffer
        template<int len>
        String( Pool& p, char const ( &s )[ len ] );                // Get _buf from pool and Copy s into
        String( Pool& p, char const* _buf, int _dataLen );          // Get _buf from pool and Copy s into
        String( char const* _buf, int _bufLen, int _dataLen, bool isRef );    // ref or Copy
        String( char const* s, bool isRef = false );                // Copy or ref
        String( String const& o );                                  // Copy
        String( String&& o );                                       // move
        String& operator=( String const& o );                       // Copy
        String& operator=( String&& o );                            // move
        ~String();                                                  // Dispose _buf
        void Assign( char const* _buf, int _bufLen, int _dataLen, bool isRef );     // ref or Copy
        void Assign( char const* s, int sLen, bool isRef );         // Copy or ref
        void Assign( char const* s, bool isRef = false );           // Copy or ref
        void Reserve( int capacity );                               // prepare memory
        void Resize( int len, bool fillZero = true );               // cut( maybe damage utf8 ) or grow up
        void Clear();                                               // Set dataLen to 0
        int Size() const;                                           // return dataLen;
        char const* C_str() const;                                  // return buf
        char* Data() const;                                         // return buf
        char operator[] ( int idx ) const;                          // return buf[ idx ]
        char& operator[] ( int idx );                               // return buf[ idx ]
        char At( int idx ) const;
        char& At( int idx );
        void Push( char c );
        void Pop();
        char& Top();
        char const& Top() const;
        bool Empty() const;
        void Insert( int idx, char const* s, int sLen = 0 );
        void Insert( int idx, String const& s );

        // todo: more util funcs like  Find, trim, split, replace, ....
        // todo: Formatter with AppendTo( String& s ) interface
        // sample: auto f = Formatter().setScale( 2 ).setWidth( 10, '0' );  s.Append( ...... f( 1.2345 ) ...... ) will be Append  0000001.23
        // 

        bool operator==( String const& o ) const;
        bool operator!=( String const& o ) const;
        bool operator<( String const& o ) const;
        bool operator>( String const& o ) const;
        bool operator<=( String const& o ) const;
        bool operator>=( String const& o ) const;

        bool operator==( char const* o ) const;

        template<size_t sLen>
        bool operator==( char const( &s )[ sLen ] ) const
        {
            if( sLen != dataLen ) return false;
            return memcmp( buf, s, dataLen ) == 0;
        }



        void ToLower();
        void ToUpper();
        void ToUpperFirstChar();
        void ToLowerUnsafe();                                       // 2x faster than ToLower(). bad chars：   @[\]^_` 

        int GetHashCode() const;


        template<typename ...TS>
        void Append( TS const & ...vs );

        template<typename ...TS>
        void AppendFormat( char const* format, TS const & ...vs );

        template<typename T>
        void AppendHex( T const& v );

        template<typename T>
        String& operator<<( T const& v );


        // like Append, return strlen.
        // dangerous: buffer overflow
        template<typename ...TS>
        static int Fill( char * _buf, TS const & ...vs );

        template<typename ...TS>
        static int FillHex( char * _buf, TS const & ...vs );



        template<typename ...TS>
        static String Make( TS const & ...vs );

        template<typename ...TS>
        static String MakeFormat( char const* format, TS const & ...vs );

        template<typename ...TS>
        static String Make( Pool& p, TS const & ...vs );

        template<typename ...TS>
        static String MakeFormat( Pool& p, char const* format, TS const & ...vs );

        template<typename ...TS, int _bufLen>
        static String Make( char( &_buf )[ _bufLen ], TS const & ...vs );

        template<typename T>
        static String ToString( T const& v );

        template<typename T>
        static String ToHexString( T const& v );




        // dangerous: Get first buffer from static circle pool char[ 32 ][ 1024 ]
        template<typename ...TS>
        static String MakeFast( TS const & ...vs );

        // dangerous: Get first buffer from static circle pool char[ 32 ][ 128 ]
        template<typename T>
        static String const ToStringFast( T const& v );

        // dangerous: Get first buffer from static circle pool char[ 32 ][ 32 ]
        template<typename T>
        static String const ToHexStringFast( T const& v );




        std::string Std_str();




        // for ByteBuffer
        void WriteTo( ByteBuffer& fb ) const;
        void FastWriteTo( ByteBuffer& fb ) const;
        bool ReadFrom( ByteBuffer& fb );




        // 这部分还要再思考。理论上讲可以直接加 ToString 函数来做到。
        //// 带 ToString( String& ) const 函数的都可以 被正常 Append
        //HAS_FUNC( HasFunc_ToString, ToString, void ( T::* )( String& ) const );
        //template<typename T>
        //typename std::enable_if<HasFunc_ToString<T>::value, void>::type AppendCoreSwitch( String& s, T const& v )
        //{
        //    v.ToString( s );
        //};
        //template<typename T>
        //typename std::enable_if<!HasFunc_ToString<T>::value, void>::type AppendCoreSwitch( String& s, T const& v )
        //{
        //    s.Append( v );
        //};
        //template<typename T>
        //void AppendCore( String& s, T const& v )
        //{
        //    AppendCoreSwitch( *this, v );
        //}





    private:

        template<typename T>
        void AppendFormatCore( String& s, int& n, T const & v );

        template<typename T, typename ...TS>
        void AppendFormatCore( String& s, int& n, T const & v, TS const & ...vs );

        template<typename T>
        static void FillCore( char * & _buf, int & offset, T const & v );

        template<typename T, typename ...TS>
        static void FillCore( char * & _buf, int & offset, T const & v, TS const & ...vs );

        template<typename T>
        static void FillHexCore( char * & _buf, int & offset, T const & v );

        template<typename T, typename ...TS>
        static void FillHexCore( char * & _buf, int & offset, T const & v, TS const & ...vs );

        template<typename T>
        static void GetFillMaxLengthCore( int & len, T const & v );

        template<typename T, typename ...TS>
        static void GetFillMaxLengthCore( int & len, T const & v, TS const & ...vs );

        template<typename ...TS>
        static int GetFillMaxLength( TS const & ...vs );

        char*       buf = nullptr;
        int         bufLen = 0;
        int         dataLen = 0;

        Pool*       disposer;// = nullptr;
        static Pool emptyPool;     // for do not need delete's buffer
        void Dispose();
    };


}

#endif
