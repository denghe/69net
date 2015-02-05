#ifndef _BYTEBUFFER_H__
#define _BYTEBUFFER_H__

namespace xxx
{
    struct ByteBuffer : Memmoveable
    {
        char*       buf;
        int         bufLen;
        int         dataLen;
        int         offset;
        bool        isRef;

        explicit ByteBuffer( int capacity = 1024 );                                 // prepare
        ByteBuffer( char* _buf, int _bufLen, int _dataLen = 0, bool isRef = false );   // Copy or ref
        ByteBuffer( ByteBuffer const& other );                                      // Copy
        ByteBuffer( ByteBuffer&& other );                                           // move
        ByteBuffer& operator=( ByteBuffer const& other );                           // Copy
        ByteBuffer& operator=( ByteBuffer&& other );                                // move
        void Dispose();                                                             // delete[] buf if need
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


        // ������Ķ�д����ʵ����
        template<typename T>
        static void Write( char* dest, T const& src );
        template<typename T>
        static void Read( T& dest, char const* src );


        // todo: VariantWrite  VariantRead ϵ��


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


        // ��������ϵ�У����� true ��ʾ�����ɹ�
        template<typename T>
        bool Read( T& v );
        bool Read( char* _buf, int _dataLen );
        template<typename T, int len>
        bool Read( T( &a )[ len ] );
        template<typename ...TS>
        bool ReadMulti( TS& ...vs );


        // interface for ByteBuffer
        void WriteTo( ByteBuffer& fb );
        void FastWriteTo( ByteBuffer& fb );
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
