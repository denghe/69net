#pragma once
#define ENABLE_BYTEBUFFER_PTR_SUPPORT 0

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

#if ENABLE_BYTEBUFFER_PTR_SUPPORT
		Dict<void*, uint>* ptrStore = nullptr;                                      // for 序列化含指针的类实例
        Dict<uint, void*>* idxStore = nullptr;                                      // for 反序列化含指针的类实例
#endif

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
        char IndexAt( int idx ) const;                                              // buf[ idx ]
        char& IndexAt( int idx );                                                   // buf[ idx ]
        String Dump();                                                              // return buf string for display memory

#if ENABLE_BYTEBUFFER_PTR_SUPPORT
        // ptrStore, idxStore 相关
        void PtrStoreInit();                                                        // RootWrite 时自动调用 
        void PtrStoreDestroy();

        void IdxStoreInit();                                                        // RootRead 时自动调用
        void IdxStoreDestroy();
#endif

        // 最基础的读写函数实现体

        // 基本读写系列
        template<typename T>
        static void WriteCore( char* dest, T const& src );
        template<typename T>
        static void ReadCore( T& dest, char const* src );

        // 变长读写系列
        static void Write7Core( char* buf, int& offset, uint v );                   // need ensure 7 space
        static void FastRead7Core( uint& v, char* buf, int& offset );
        static bool Read7Core( uint& v, char* buf, int len, int& offset );

        // 64 位长版
        static void Write7Core( char* buf, int & offset, uint64 v );                // need ensure 9 space
        static void FastRead7Core( uint64& v, char* buf, int& offset );
        static bool Read7Core( uint64& v, char* buf, int len, int& offset );

        // negative -> ZigZag positive
        // 效果：负数变正，正数 *= 2
        static uint32 ZigZagEncode( int32  i ) { return ( i << 1 ) ^ ( i >> 31 ); }
        static uint64 ZigZagEncode( int64  i ) { return ( i << 1 ) ^ ( i >> 63 ); }

        // ZigZag positive -> negative
        static int32  ZigZagDecode( uint32 i ) { return (int32)( i >> 1 ) ^ ( -(int32)( i & 1 ) ); }
        static int64  ZigZagDecode( uint64 i ) { return (int64)( i >> 1 ) ^ ( -(int64)( i & 1 ) ); }



        // 快速写入系列：不预申请内存（危险，主供生成物代码调用）
        void FastWrite( char const* _buf, int _dataLen );                           // do not Write _dataLen
        template<int len>
        void FastWrite( char const( &s )[ len ] );                                  // same as String( len + _buf )
        template<typename T, int len>
        void FastWrite( T const( &a )[ len ] );
        template<typename T>
        void FastWrite( T const& v );
        // 多值变参版
        template<typename ...TS>
        void FastWriteMulti( TS const& ...vs );


        // 快速读出系列，不判断数据长度是否足够满足读取操作（危险，安全环节才用）
        template<typename T>
        void FastRead( T& v );
        template<typename ...TS>
        void FastReadMulti( TS& ...vs );


        // 正常写入系列：会预申请内存
        void Write( char const* _buf, int _dataLen );
        template<typename T, int len>
        void Write( T const( &a )[ len ] );
        template<typename T>
        void Write( T const& v );
        // 多值变参版
        template<typename ...TS>
        void WriteMulti( TS const& ...vs );

#if ENABLE_BYTEBUFFER_PTR_SUPPORT
        // 带指针写入系列
        template<typename T>
        void Write( T* v );
        template<typename T>
        void RootWrite( T const& v );

        // 带指针读出系列
        template<typename T>
        bool Read( T*& v );
        template<typename T>
        bool RootRead( T& v );
#endif

        // 变长写入系列：会预申请内存
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



        // 正常读出系列：返回 true 表示读出成功
        template<typename T>
        bool Read( T& v );
        bool Read( char* _buf, int _dataLen );
        template<typename T, int len>
        bool Read( T( &a )[ len ] );
        template<typename ...TS>
        bool ReadMulti( TS& ...vs );


        // interface for ByteBuffer
        void WriteTo( ByteBuffer& bb ) const;
        void FastWriteTo( ByteBuffer& bb ) const;
        bool ReadFrom( ByteBuffer& bb );

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

