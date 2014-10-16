#ifndef _FLATBUFFER_H__
#define _FLATBUFFER_H__

namespace xxx
{


    class FlatBuffer : Memmoveable
    {
    public:
        explicit FlatBuffer( int capacity = 1024 );                                 // prepare
        FlatBuffer( Pool& p );                                                      // prepare with pool buffer
        FlatBuffer( char* _buf, int _bufLen, int _dataLen = 0, bool isRef = false );   // Copy or ref
        FlatBuffer( FlatBuffer const& other );                                      // Copy
        FlatBuffer( FlatBuffer&& other );                                           // move
        FlatBuffer& operator=( FlatBuffer const& other );                           // Copy
        FlatBuffer& operator=( FlatBuffer&& other );                                // move
        ~FlatBuffer();
        void Assign( char const* _buf, int _bufLen, int _dataLen = 0, bool isRef = false );  // Copy or ref
        void Reserve( int capacity );
        void Resize( int widx );
        void Clear();                                                               // dataLen = offset = 0
        int Size() const;                                                           // dataLen
        int& Size();                                                                // dataLen
        int Offset() const;                                                         // offset
        int& Offset();                                                              // offset
        bool Empty() const;                                                         // dataLen == 0
        bool Full() const;                                                          // dataLen == offset
        char const* Data() const;                                                   // buf
        char* Data();                                                               // buf
        char operator[] ( int idx ) const;                                          // buf[ idx ]
        char& operator[] ( int idx );                                               // buf[ idx ]
        char At( int idx ) const;                                                   // buf[ idx ]
        char& At( int idx );                                                        // buf[ idx ]



        // for FlatBuffer Write
        int GetWriteBufferSize();
        void WriteBuffer( FlatBuffer& fb );
        void WriteBufferDirect( FlatBuffer& fb );
        bool ReadBuffer( FlatBuffer& fb );




        // do not Reserve
        void WriteDirect( char const* _buf, int _dataLen );                           // do not Write _dataLen
        template<int len>
        void WriteDirect( char const( &s )[ len ] );                                // same as String( len + _buf )
        template<typename T, int len>
        void WriteDirect( T const( &a )[ len ] );
        template<typename T>
        void WriteDirect( T const& v );

        template<typename ...TS>
        void WritesDirect( TS const& ...vs );

        // Reserve
        void Write( char const* _buf, int _dataLen );
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
        bool Read( char* _buf, int _dataLen );
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

        char*       buf;
        int         bufLen;
        int         dataLen;
        int         offset;

        Pool*       disposer;
        static Pool emptyPool;     // for do not need delete's buffer
        void Dispose();
    };

}

#endif
