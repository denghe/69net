#ifndef _BYTEBUFFER_H__
#define _BYTEBUFFER_H__

namespace xxx
{
    template <typename TK, typename TV>
    struct Dict;

    class ByteBuffer : Memmoveable
    {
    public:
        char*       buf;
        int         bufLen;
        int         dataLen;
        int         offset;
        bool        isRef;
        Dict<void*, int>* ptrStore = nullptr;
        Dict<int, void*>* idxStore = nullptr;

        explicit ByteBuffer( int capacity = 1024 );                                 // prepare
        ByteBuffer( char* _buf, int _bufLen, int _dataLen = 0, bool isRef = false );   // Copy or ref
        ByteBuffer( ByteBuffer const& other );                                      // Copy
        ByteBuffer( ByteBuffer&& other );                                           // move
        ByteBuffer& operator=( ByteBuffer const& other );                           // Copy
        ByteBuffer& operator=( ByteBuffer&& other );                                // move
        void Destroy();                                                             // delete[] buf if need
        ~ByteBuffer();
        void Assign( char const* _buf, int _bufLen, int _dataLen = 0, bool isRef = false );  // Copy or ref
        void Reserve( int capacity );
        void Resize( int widx );
        void Clear();                                                               // dataLen = offset = 0
        bool Empty() const;                                                         // dataLen == 0
        bool Full() const;                                                          // dataLen == offset
        char operator[] ( int idx ) const;                                          // buf[ idx ]
        char& operator[] ( int idx );                                               // buf[ idx ]
        char At( int idx ) const;                                                   // buf[ idx ]
        char& At( int idx );                                                        // buf[ idx ]
        String Dump();                                                              // return buf string for display memory

        // ptrStore, idxStore ���
        void PtrStoreInit();
        void PtrStoreDestroy();

        void IdxStoreInit();
        void IdxStoreDestroy();


        // ������Ķ�д����ʵ����

        // ������дϵ��
        template<typename T>
        static void WriteCore( char* dest, T const& src );
        template<typename T>
        static void ReadCore( T& dest, char const* src );

        // �䳤��дϵ��
        static void Write7Core( char* buf, int& offset, uint v );                   // need ensure 7 space
        static void FastRead7Core( uint& v, char* buf, int& offset );
        static bool Read7Core( uint& v, char* buf, int len, int& offset );

        // 64 λ����
        static void Write7Core( char* buf, int & offset, uint64 v );                // need ensure 9 space
        static void FastRead7Core( uint64& v, char* buf, int& offset );
        static bool Read7Core( uint64& v, char* buf, int len, int& offset );

        // negative -> ZegZag positive
        static uint32 ZegZagEncode( int32  i ) { return ( i << 1 ) ^ ( i >> 31 ); }
        static uint64 ZegZagEncode( int64  i ) { return ( i << 1 ) ^ ( i >> 63 ); }

        // ZegZag positive -> negative
        static int32  ZegZagDecode( uint32 i ) { return (int32)( i >> 1 ) ^ ( -(int32)( i & 1 ) ); }
        static int64  ZegZagDecode( uint64 i ) { return (int64)( i >> 1 ) ^ ( -(int64)( i & 1 ) ); }



        // ����д��ϵ�У���Ԥ�����ڴ棨Σ�գ����������������ã�
        void FastWrite( char const* _buf, int _dataLen );                           // do not Write _dataLen
        template<int len>
        void FastWrite( char const( &s )[ len ] );                                  // same as String( len + _buf )
        template<typename T, int len>
        void FastWrite( T const( &a )[ len ] );
        template<typename T>
        void FastWrite( T const& v );
        // ��ֵ��ΰ�
        template<typename ...TS>
        void FastWriteMulti( TS const& ...vs );


        // ���ٶ���ϵ�У����ж����ݳ����Ƿ��㹻�����ȡ������Σ�գ���ȫ���ڲ��ã�
        template<typename T>
        void FastRead( T& v );
        template<typename ...TS>
        void FastReadMulti( TS& ...vs );


        // ����д��ϵ�У���Ԥ�����ڴ�
        void Write( char const* _buf, int _dataLen );
        template<int len>
        void Write( char const( &s )[ len ] );
        template<typename T, int len>
        void Write( T const( &a )[ len ] );
        template<typename T>
        void Write( T const& v );
        // ��ֵ��ΰ�
        template<typename ...TS>
        void WriteMulti( TS const& ...vs );


        // ��ָ��д��ϵ��
        template<typename T>
        void Write( T* v );
        template<typename T>
        void RootWrite( T const& v );

        // ��ָ�����ϵ��
        template<typename T>
        bool Read( T*& v );
        template<typename T>
        bool RootRead( T& v );


        // �䳤д��ϵ�У���Ԥ�����ڴ�
        void VarWrite( int v );
        void VarWrite( uint v );
        void VarWrite( int64 v );
        void VarWrite( uint64 v );

        bool VarRead( int& v );
        bool VarRead( uint& v );
        bool VarRead( int64& v );
        bool VarRead( uint64& v );

        void FastVarWrite( int v );
        void FastVarWrite( uint v );
        void FastVarWrite( int64 v );
        void FastVarWrite( uint64 v );

        void FastVarRead( int& v );
        void FastVarRead( uint& v );
        void FastVarRead( int64& v );
        void FastVarRead( uint64& v );



        // ��������ϵ�У����� true ��ʾ�����ɹ�
        template<typename T>
        bool Read( T& v );
        bool Read( char* _buf, int _dataLen );
        template<typename T, int len>
        bool Read( T( &a )[ len ] );
        template<typename ...TS>
        bool ReadMulti( TS& ...vs );


        // interface for ByteBuffer
        void WriteTo( ByteBuffer& fb ) const;
        void FastWriteTo( ByteBuffer& fb ) const;
        bool ReadFrom( ByteBuffer& fb );

    protected:
        template<typename T>
        void FastWriteMultiCore( T const& v );
        template<typename T, typename ...TS>
        void FastWriteMultiCore( T const& v, TS const& ...vs );

        template<typename T>
        void FastReadMultiCore( T& v );
        template<typename T, typename ...TS>
        void FastReadMultiCore( T& v, TS& ...vs );

        template<typename T>
        void WriteMultiCore( T const& v );
        template<typename T, typename ...TS>
        void WriteMultiCore( T const& v, TS const& ...vs );

        template<typename T>
        bool ReadMultiCore( T& v );
        template<typename T, typename ...TS>
        bool ReadMultiCore( T& v, TS& ...vs );
    };

}

#endif
