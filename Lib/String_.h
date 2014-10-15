#ifndef _STRING_H__
#define _STRING_H__

namespace xxx
{

    class FlatBuffer;

    class String : Memmoveable
    {
    public:
        explicit String( int capacity = 64 );                       // prepare
        String( Pool& p );                                          // prepare with pool buffer
        template<int len>
        String( Pool& p, char const ( &s )[ len ] );                // Get buf from pool and Copy s into
        String( Pool& p, char const* buf, int dataLen );            // Get buf from pool and Copy s into
        String( char const* buf, int bufLen, int dataLen, bool isRef = true );    // ref or Copy
        String( char const* s, bool isRef = false );                // Copy or ref
        String( String const& other );                              // Copy
        String( String&& other );                                   // move
        String& operator=( String const& other );                   // Copy
        String& operator=( String&& other );                        // move
        ~String();                                                  // Dispose buf
        void Assign( char const* buf, int bufLen, int dataLen, bool isRef = true );     // ref or Copy
        void Assign( char const* s, bool isRef = false );           // Copy or ref
        void Reserve( int capacity );                               // prepare memory
        void Resize( int len, bool FillZero = true );               // cut( maybe damage utf8 ) or grow up
        void Clear();                                               // Set _dataLen to 0
        int Size() const;                                           // return _dataLen;
        char const* C_str() const;                                  // return _buf
        char* Data() const;                                         // return _buf
        char operator[] ( int idx ) const;                          // return _buf[ idx ]
        char& operator[] ( int idx );                               // return _buf[ idx ]
        char At( int idx ) const;
        char& At( int idx );
        void Push( char c );
        void Pop();
        char& Top();
        char const& Top() const;

        // todo: more util funcs like  Find, trim, split, replace, ....
        // todo: Formatter with AppendTo( String& s ) interface
        // sample: auto f = Formatter().setScale( 2 ).setWidth( 10, '0' );  s.Append( ...... f( 1.2345 ) ...... ) will be Append  0000001.23
        // 

        bool operator==( String const& other ) const;
        bool operator!=( String const& other ) const;
        bool operator<( String const& other ) const;
        bool operator>( String const& other ) const;
        bool operator<=( String const& other ) const;
        bool operator>=( String const& other ) const;


        void ToLower();
        void ToUpper();
        void ToLowerUnsafe();                                       // 2x faster than ToLower(). bad chars£º   @[\]^_` 

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
        static int Fill( char * buf, TS const & ...vs );

        template<typename ...TS>
        static int FillHex( char * buf, TS const & ...vs );



        template<typename ...TS>
        static String Make( TS const & ...vs );

        template<typename ...TS>
        static String MakeFormat( char const* format, TS const & ...vs );

        template<typename ...TS>
        static String Make( Pool& p, TS const & ...vs );

        template<typename ...TS>
        static String MakeFormat( Pool& p, char const* format, TS const & ...vs );

        template<typename ...TS, int bufLen>
        static String Make( char( &buf )[ bufLen ], TS const & ...vs );

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




        // for FlatBuffer
        int GetWriteBufferSize() const;
        void WriteBuffer( FlatBuffer& fb ) const;
        void WriteBufferDirect( FlatBuffer& fb ) const;
        bool ReadBuffer( FlatBuffer& fb );










    private:

        template<typename T>
        void AppendFormatCore( String& s, int& n, T const & v );

        template<typename T, typename ...TS>
        void AppendFormatCore( String& s, int& n, T const & v, TS const & ...vs );

        template<typename T>
        static void FillCore( char * & buf, int & offset, T const & v );

        template<typename T, typename ...TS>
        static void FillCore( char * & buf, int & offset, T const & v, TS const & ...vs );

        template<typename T>
        static void FillHexCore( char * & buf, int & offset, T const & v );

        template<typename T, typename ...TS>
        static void FillHexCore( char * & buf, int & offset, T const & v, TS const & ...vs );

        template<typename T>
        static void GetFillMaxLengthCore( int & len, T const & v );

        template<typename T, typename ...TS>
        static void GetFillMaxLengthCore( int & len, T const & v, TS const & ...vs );

        template<typename ...TS>
        static int GetFillMaxLength( TS const & ...vs );

        char*       _buf;
        int         _bufLen;
        int         _dataLen;

        Pool*       _disposer;
        static Pool _emptyPool;     // for do not need delete's buffer
        void Dispose();
    };

}

#endif
