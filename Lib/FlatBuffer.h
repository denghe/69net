#ifndef _FLATBUFFER_H__
#define _FLATBUFFER_H__

namespace xxx
{


    class FlatBuffer : Memmoveable
    {
    public:
        explicit FlatBuffer( int capacity = 1024 );                                 // prepare
        FlatBuffer( Pool& p );                                                      // prepare with pool buffer
        FlatBuffer( char* buf, int bufLen, int dataLen = 0, bool isRef = false );   // Copy or ref
        FlatBuffer( FlatBuffer const& other );                                      // Copy
        FlatBuffer( FlatBuffer&& other );                                           // move
        FlatBuffer& operator=( FlatBuffer const& other );                           // Copy
        FlatBuffer& operator=( FlatBuffer&& other );                                // move
        ~FlatBuffer();
        void Assign( char const* buf, int bufLen, int dataLen = 0, bool isRef = false );  // Copy or ref
        void Reserve( int capacity );
        void Resize( int widx );
        void Clear();                                                               // _dataLen = _offset = 0
        int Size() const;                                                           // _dataLen
        int& Size();                                                                // _dataLen
        int Offset() const;                                                         // _offset
        int& Offset();                                                              // _offset
        bool Empty() const;                                                         // _dataLen == 0
        bool Full() const;                                                          // _dataLen == _offset
        char const* Data() const;                                                   // _buf
        char* Data();                                                               // _buf
        char operator[] ( int idx ) const;                                          // _buf[ idx ]
        char& operator[] ( int idx );                                               // _buf[ idx ]
        char At( int idx ) const;                                                   // _buf[ idx ]
        char& At( int idx );                                                        // _buf[ idx ]



        // for FlatBuffer Write
        int GetWriteBufferSize();
        void WriteBuffer( FlatBuffer& fb );
        void WriteBufferDirect( FlatBuffer& fb );
        bool ReadBuffer( FlatBuffer& fb );




        // do not Reserve
        void WriteDirect( char const* buf, int dataLen );                           // do not Write dataLen
        template<int len>
        void WriteDirect( char const( &s )[ len ] );                                // same as String( len + buf )
        template<typename T, int len>
        void WriteDirect( T const( &a )[ len ] );
        template<typename T>
        void WriteDirect( T const& v );

        template<typename ...TS>
        void WritesDirect( TS const& ...vs );

        // Reserve
        void Write( char const* buf, int dataLen );
        template<int len>
        void Write( char const( &s )[ len ] );
        template<typename T, int len>
        void Write( T const( &a )[ len ] );
        template<typename T>
        void Write( T const& v );

        // Reserve once
        template<typename ...TS>
        void Writes( TS const& ...vs );



        // return true: success
        template<typename T>
        bool Read( T& v );
        bool Read( char* buf, int dataLen );
        template<typename T, int len>
        bool Read( T( &a )[ len ] );
        template<typename ...TS>
        bool Reads( TS& ...vs );


        template<typename T>
        void ReadDirect( T& v );
        template<typename ...TS>
        void ReadsDirect( TS& ...vs );




        // todo: writeVar  ReadVar support

        String Dump();

    private:
        template<typename T>
        void WritesCore( T const& v );
        template<typename T, typename ...TS>
        void WritesCore( T const& v, TS const& ...vs );

        template<typename T>
        bool ReadsCore( T& v );
        template<typename T, typename ...TS>
        bool ReadsCore( T& v, TS& ...vs );

        template<typename T>
        void ReadsDirectCore( T& v );
        template<typename T, typename ...TS>
        void ReadsDirectCore( T& v, TS& ...vs );

        char*       _buf;
        int         _bufLen;
        int         _dataLen;
        int         _offset;

        Pool*       _disposer;
        static Pool _emptyPool;     // for do not need delete's buffer
        void Dispose();
    };

}

#endif
